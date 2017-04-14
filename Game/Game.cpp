#include "stdafx.h"
#include "Game.h"
#include "BasicFileSystem.h"

CGame::CGame()
  : mpFileSystem(NULL)
  , m_pWindow(NULL)
  , m_pGLContext(NULL)
  , mUIFont()
  , mUIText(mUIFont)
  , m_GUI()
  , mIntroProcess()
  , mMenuProcess(m_GUI, mIDevMap)
  , mIntroView(mIntroProcess)
  , mMenuView(mMenuProcess, m_RaceTrack)
  , mHS(mIDevMap)
  , m_bShutdown(false)
  , m_bGamePause(false)
  , m_bTakeScreen(false)
  , m_uGameState(GS_INTRO)
  , m_uBlurTexture(0) {
  mLogFile.open(L"game.log", std::ios::out | std::ios::trunc);
  mLogger.AddStream(&mLogFile);

  cb::CLogger::SetInstance(&mLogger);

  SDL_Init(0);

  mpFileSystem = new CBasicFileSystem();
}

CGame::~CGame() {
  Free();
  SDL_Quit();

  cb::CLogger::SetInstance(nullptr);

  delete mpFileSystem;
}

bool CGame::Init(std::string strCmdLine) {
  mConfigFilePath = L"main.cfg";
  LoadConfig();

  if(!this->InitWindow(GAME_FULLNAME)) {
    cb::error(L"Can't initialize window.");
    Free();
    return false;
  }
  if(!this->InitRender()) {
    cb::error(L"Can't Initialize render.");
    Free();
    return false;
  }
  if(!this->InitInput()) {
    cb::error(L"Can't initialize input.");
    Free();
    return false;
  }

  if(!this->InitOpenGL()) {
    cb::error(L"Can't Initialize OpenGL");
    Free();
    return false;
  }
  if(!this->InitGame()) {
    cb::error(L"Can't Initialize Final Game Settings");
    Free();
    return false;
  }

  mTimer.Update();
  return true;
}

bool CGame::InitWindow(std::string strTitle) {
  if(SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
    cb::error(L"Failed initializing SDL.");
    return false;
  }

  int winFlags = SDL_WINDOW_OPENGL;
  if(mConfig.Screen.Fullscreen) {
    winFlags |= SDL_WINDOW_FULLSCREEN;
    winFlags |= SDL_WINDOW_BORDERLESS;
  }
  this->m_pWindow = SDL_CreateWindow(strTitle.c_str(),
                                     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     int(mConfig.Screen.Width),
                                     int(mConfig.Screen.Height),
                                     winFlags);
  if(this->m_pWindow == nullptr) {
    cb::error(L"Failed to create window.");
    return false;
  }

  SDL_ShowWindow(this->m_pWindow);
  return true;
}

bool CGame::InitRender() {
  if(mConfig.Screen.Fullscreen)
    ChangeDispMode();

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, int(mConfig.Screen.ColorBits));
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

  this->m_pGLContext = SDL_GL_CreateContext(this->m_pWindow);
  if(this->m_pGLContext == nullptr) {
    cb::error(L"Failed to create OpenGL Context.");
    return false;
  }

  SDL_GL_MakeCurrent(this->m_pWindow, this->m_pGLContext);


  return true;
}

bool  CGame::InitInput() {
  SDL_InitSubSystem(SDL_INIT_EVENTS);

  mIDevMap.AddDevice(InputDevice::Keyboard, new CKeyboardInputDevice());
  mIDevMap.AddDevice(InputDevice::Mouse, new CMouseInputDevice(mConfig.Screen.GetSize()));

  return true;
}

bool CGame::InitOpenGL() {
  if(glewInit() != GLEW_OK) {
    return false;
  }

  m_GUI.Init();

  if(!mUIText.Init(L"font.fgx")) {
    cb::error(L"Failed to initialize UI Text.");
    return false;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glClearStencil(0);

  if(mConfig.Render.SmoothShade)
    glShadeModel(GL_SMOOTH);
  else
    glShadeModel(GL_FLAT);

  glDepthFunc(GL_LEQUAL);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  if(mConfig.Render.SmoothLines)
    glEnable(GL_LINE_SMOOTH);
  else
    glDisable(GL_LINE_SMOOTH);

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_BLEND);

  glLineWidth(2.0f);
  glPointSize(1.0f);

  mProjMatrix = glm::perspectiveFov(glm::radians(50.0f),
                                    float(mConfig.Screen.Width),
                                    float(mConfig.Screen.Height),
                                    1.0f, 50000.0f);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  mUIText.Bind(mConfig.Screen.GetSize());
  mUIText.SetColor(1.0f, 1.0f, 1.0f);
  mUIText.Render(glm::vec2(100.0f, 200.f), L"Please wait, loadng game...");
  mUIText.UnBind();
  SDL_GL_SwapWindow(this->m_pWindow);

  glGenTextures(1, &m_uBlurTexture);
  glBindTexture(GL_TEXTURE_2D, m_uBlurTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, 4,
               mConfig.Render.BlurTexSize,
               mConfig.Render.BlurTexSize,
               0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  return true;
}

bool CGame::InitGame() {
  if(!mTimer.Init()) {
    return false;
  }

  std::srand((Uint32)mTimer.GetLastTick());

  this->ScanDispModes();

  CModel::InitModels();

  this->m_Racer.Init(CModel::MT_HTTP20);
  this->m_Racer.SetColor(0x90FF0000);
  this->m_RaceTrack.Init();
  this->m_RaceTrack.SetRacer(&m_Racer);
  this->mHS.LoadScores(L"score.hsf");

  //if(WGLEW_EXT_swap_control) {
  //  if(ScrParam.bVSync)
  //    wglSwapIntervalEXT(1);
  //  else wglSwapIntervalEXT(0);
  //}

  if(!mMenuProcess.Init(mConfig)) {
    cb::error(L"Failed to initialize menu process.");
    return false;
  }
  mMenuProcess.AddObserver(this);

  UpdateHS();

  mIntroView.Init(L"logos.fgx");
  mMenuView.Init(mConfig.Screen.GetSize());

  return true;
}

void CGame::Free() {
  this->FreeGame();
  this->FreeOpenGL();
  this->FreeInput();
  this->FreeRender();
  this->FreeWindow();
}

void CGame::FreeWindow() {
  if(this->m_pWindow) {
    SDL_DestroyWindow(this->m_pWindow);
    this->m_pWindow = nullptr;
  }
}

void CGame::FreeRender() {
  if(this->m_pGLContext) {
    SDL_GL_DeleteContext(this->m_pGLContext);
    this->m_pGLContext = nullptr;
  }

  if(mConfig.Screen.Fullscreen) {
    SDL_SetWindowDisplayMode(this->m_pWindow, &this->m_ModeOryginal);
  }

  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void CGame::FreeInput() {
  SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void CGame::FreeOpenGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  if(glIsTexture(m_uBlurTexture))
    glDeleteTextures(1, &m_uBlurTexture);
}

void CGame::FreeGame() {
  mMenuProcess.RemoveObserver(this);
  m_RaceTrack.Free();
  m_Racer.Free();
  m_GUI.Free();
  mIntroView.Free();
  CModel::FreeModels();
}

void CGame::ScanDispModes() {
  SDL_DisplayMode	curDispMode;
  memset(&curDispMode, 0, sizeof(SDL_DisplayMode));

  if(SDL_GetCurrentDisplayMode(0, &curDispMode) != 0)
    return;

  int dispNum = SDL_GetNumDisplayModes(0);
  for(int i = 0; i < dispNum; i++) {
    SDL_DisplayMode mode;
    memset(&mode, 0, sizeof(SDL_DisplayMode));
    if(SDL_GetDisplayMode(0, i, &mode) != 0) {
      continue;
    }

    if(mode.format != curDispMode.format || mode.refresh_rate != curDispMode.refresh_rate) {
      continue;
    }

    if(mode.w == curDispMode.w && mode.h == curDispMode.h) {
      mConfig.Screen.DevId = i;
    }

    this->m_ModeList.push_back(mode);
  }
}

void CGame::ChangeDispMode() {
  SDL_DisplayMode mode;

  if(SDL_GetCurrentDisplayMode(0, &mode) != 0)
    return;

  mode.w = mConfig.Screen.Width;
  mode.h = mConfig.Screen.Height;

  if(SDL_SetWindowDisplayMode(this->m_pWindow, &mode) != 0) {
    cb::error(cb::format(L"Can't change display settings to {0}x{1}",
                         mConfig.Screen.Width,
                         mConfig.Screen.Height));
    return;
  }
}


int CGame::MainLoop() {
  SDL_Event event;

  while(!m_bShutdown) {

    if(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT)
        this->m_bShutdown = true;
    }

    this->Update();
    this->Render();
  }

  Free();
  return 0;
}

void CGame::SaveConfig() {
  cb::info(cb::format(L"Saving config file to path {0}.", mConfigFilePath));
  if(!mConfig.Write(*mpFileSystem, mConfigFilePath)) {
    cb::error(L"Failed to save config file.");
  }
}

void CGame::LoadConfig() {
  cb::info(cb::format(L"Loading config file from path {0}.", mConfigFilePath));
  if(!mConfig.Read(*mpFileSystem, mConfigFilePath)) {
    cb::error(L"Failed to load config file, reseting fo default.");
    mConfig = CConfig();
  }
}

float CGame::s_fMaxDT = 0.02f;

void CGame::Update() {
  mTimer.Update();

  float timeDelta = mTimer.GetTimeDelta();
  while(timeDelta > this->s_fMaxDT) {
    mIDevMap.Update(timeDelta);

    this->UpdateLogic(this->s_fMaxDT);
    timeDelta -= this->s_fMaxDT;
  }

  mIDevMap.Update(timeDelta);
  this->UpdateLogic(timeDelta);
}

void CGame::UpdateLogic(const float timeDelta) {
  static bool down2 = false;
  switch(m_uGameState) {
  case GS_INTRO:
    mIntroProcess.Update(timeDelta);
    if(mIntroProcess.IsIntroEnded() ||
       mIDevMap.GetState(InputDevice::Keyboard, (Uint32)KeyboardType::KeyPress, SDL_SCANCODE_ESCAPE)) {
      m_uGameState = GS_MENU;
    }
    break;

  case GS_MENU:
    mMenuProcess.Update(timeDelta);
    break;

  case GS_GAME:
    if(!mMenuProcess.GetMenuManager().GetCurrentMenu()->IsHidden()) {
      CGUIMenu* M = mMenuProcess.GetMenuManager().GetCurrentMenu();
      if(!M->IsHiding())
        M->Hide();
      mMenuProcess.Update(timeDelta);
    }
    else
      this->UpdateGame(timeDelta);
    break;

  case GS_HIGH:
    mHS.Update(timeDelta);
    if(mHS.IsEnded()) {
      mHS.SaveScores(L"score.hsf");
      UpdateHS();
      mMenuProcess.GetMenuManager().ForceSwitchToMenu(CMenuProcess::MENU_HIGH);
      m_uGameState = GS_MENU;
    }
    break;
  };

  if(mIDevMap.GetState(InputDevice::Keyboard, (Uint32)KeyboardType::KeyPress, SDL_SCANCODE_F11)) {
    SDL_MinimizeWindow(this->m_pWindow);

    if(mConfig.Screen.Fullscreen)
      SDL_SetWindowDisplayMode(this->m_pWindow, &this->m_ModeOryginal);

    SDL_WaitEvent(NULL);

    if(mConfig.Screen.Fullscreen)
      this->ChangeDispMode();
  }

  if(mIDevMap.GetState(InputDevice::Keyboard, (Uint32)KeyboardType::KeyPress, SDL_SCANCODE_F12)) {
    this->m_bTakeScreen = true;
  }
}

void CGame::UpdateGame(const float timeDelta) {
  static bool down = false;
  if(m_RaceTrack.IsGameRuning()) {
    float xdelta = mIDevMap.GetRange(InputDevice::Mouse, (Uint32)MouseType::AxisDelta, (Uint32)MouseAxisId::AxisX) * mConfig.Screen.Width;
    this->m_Racer.ModRotation(xdelta);
    if(mIDevMap.GetState(InputDevice::Mouse, (Uint32)MouseType::ButtonPress, SDL_BUTTON_LEFT)) {
      this->m_RaceTrack.FireWeapon();
    }
  }
  if(m_RaceTrack.IsGameOver()) {
    mMenuProcess.GetMenuManager().ForceSwitchToMenu(CMenuProcess::MENU_MAIN);
    mMenuProcess.GetMenuManager().GetMenu(CMenuProcess::MENU_MAIN)->GetMenuItem(CMenuProcess::MI_RETURN)->SetEnable(false);
    mHS.SetTempScore(m_RaceTrack.GetPoints());
    m_uGameState = GS_HIGH;
    return;
  }

  if(mIDevMap.GetState(InputDevice::Keyboard, (Uint32)KeyboardType::KeyPress, SDL_SCANCODE_ESCAPE)) {
    mMenuProcess.GetMenuManager().ForceSwitchToMenu(CMenuProcess::MENU_MAIN);
    mMenuProcess.GetMenuManager().GetMenu(CMenuProcess::MENU_MAIN)->GetMenuItem(CMenuProcess::MI_RETURN)->SetEnable(true);
    m_uGameState = GS_MENU;
    return;
  }

  m_RaceTrack.Update(timeDelta);
}

void CGame::MenuItemAction(CGUIMenuManager& menuMng, CGUIMenu& menu, CGUIMenuItem& item) {
  Uint32 id = 0;
  char szBuffer[1000];
  switch(menu.GetClickedID()) {
  case CMenuProcess::MI_RETURN:
    m_uGameState = GS_GAME;
    break;

  case CMenuProcess::MI_NEWGAME:
    m_RaceTrack.ResetGame();
    m_bGamePause = true;
    m_uGameState = GS_GAME;
    break;

  case CMenuProcess::MI_EXIT:
    m_bShutdown = true;
    break;

  case CMenuProcess::MI_HIGH:
  case CMenuProcess::MI_OPTIONS:
  case CMenuProcess::MI_GOBACK:
    menuMng.SwitchToMenu(item.GetUserDefID());
    break;

  case CMenuProcess::MI_RESOLUTION:
    {
      id = item.GetUserDefID();
      if(++id >= Uint32(this->m_ModeList.size()))
        id = 0;

      mConfig.Screen.DevId = id;
      SDL_DisplayMode& disp = m_ModeList[id];

      if(disp.w == mConfig.Screen.Width && disp.h == mConfig.Screen.Height)
        menu.GetMenuItem(CMenuProcess::MI_OPWARNING)->SetEnable(false);
      else
        menu.GetMenuItem(CMenuProcess::MI_OPWARNING)->SetEnable(true);

      mConfig.Screen.Width = disp.w;
      mConfig.Screen.Height = disp.h;
      mConfig.Screen.RefreshRate = disp.refresh_rate;
      SaveConfig();

      sprintf_s(szBuffer, 1000, "Resolution: %u X %u", m_ModeList[id].w, m_ModeList[id].h);

      item.SetName(szBuffer);
      item.SetUserDefID(id);
    }
    break;

  case CMenuProcess::MI_SMOOTHSHADE:
    mConfig.Render.SmoothShade = !mConfig.Render.SmoothShade;
    if(mConfig.Render.SmoothShade) {
      glShadeModel(GL_SMOOTH);
      item.SetName("Smooth Shading: Enabled");
    }
    else {
      glShadeModel(GL_FLAT);
      item.SetName("Smooth Shading: Disabled");
    }
    SaveConfig();
    break;

  case CMenuProcess::MI_SMOOTHLINE:
    mConfig.Render.SmoothLines = !mConfig.Render.SmoothLines;
    if(mConfig.Render.SmoothLines) {
      glEnable(GL_LINE_SMOOTH);
      item.SetName("Smooth Lines: Enabled");
    }
    else {
      glDisable(GL_LINE_SMOOTH);
      item.SetName("Smooth Lines: Disabled");
    }
    SaveConfig();
    break;

  case CMenuProcess::MI_FULLSCREEN:
    mConfig.Screen.Fullscreen = !mConfig.Screen.Fullscreen;
    if(mConfig.Screen.Fullscreen)
      item.SetName("Fullscreen: Enabled");
    else
      item.SetName("Fullscreen: Disabled");
    menu.GetMenuItem(CMenuProcess::MI_OPWARNING)->SetEnable(true);
    SaveConfig();
    break;

  case CMenuProcess::MI_FPSCOUNTER:
    mConfig.Diag.FPSCounter = !mConfig.Diag.FPSCounter;
    item.SetName((mConfig.Diag.FPSCounter) ? "FPS Counter: Enabled" : "FPS Counter: Disabled");
    SaveConfig();
    break;

  case CMenuProcess::MI_VSYNC:
    mConfig.Screen.VSync = !mConfig.Screen.VSync;
    if(mConfig.Screen.VSync) {
      //wglSwapIntervalEXT(1);
      item.SetName("VSync: Enabled");
    }
    else {
      //wglSwapIntervalEXT(0);
      item.SetName("VSync: Disabled");
    }
    SaveConfig();
    break;

  case CMenuProcess::MI_HSRESET:
    mHS.ResetAllScores();
    UpdateHS();
    mHS.SaveScores(L"score.hsf");
    break;
  };
}

void CGame::UpdateHS() {
  mHS.FillHSMenu(mMenuProcess.GetMenuManager());
}

void CGame::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  if(m_bGamePause) {
    //if (ScrParam.bBlur) {
    //  glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
    //  glViewport(0, 0, m_uBlurTexSize, m_uBlurTexSize);
    //  glPushMatrix();
    //  glMatrixMode(GL_PROJECTION);
    //  glPushMatrix();
    //  glLoadIdentity();
    //  gluPerspective(50.0, ScrParam.GetAspectRatio(), 1.0, 50000.0);
    //  glMatrixMode(GL_MODELVIEW);
    //  glLoadIdentity();

    //  RenderGame();

    //  glBindTexture(GL_TEXTURE_2D, m_uBlurTexture);
    //  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0,
    //                   m_uBlurTexSize, m_uBlurTexSize, 0);

    //  glMatrixMode(GL_PROJECTION);
    //  glPopMatrix();
    //  glMatrixMode(GL_MODELVIEW);
    //  glPopMatrix();
    //  glPopAttrib();

    //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //}


    //if (ScrParam.bBlur) {
    //  m_cGUI.Begin(glm::vec2(1.0f, 1.0f));
    //  glBindTexture(GL_TEXTURE_2D, m_uBlurTexture);
    //  glColor4f(1.0f, 1.0f, 1.0f, m_fBlurTexAlpha);
    //  m_cGUI.RenderFSQuadTex(glm::vec2(1.0f, 1.0f));
    //  m_cGUI.End();
    //}
  }

  RenderGame();
  RenderGUI();

  if(m_bTakeScreen) {
    this->TakeScreenshot();
    m_bTakeScreen = false;
  }

  SDL_GL_SwapWindow(this->m_pWindow);
}

void CGame::RenderGame() {
  switch(m_uGameState) {
  case GS_INTRO:
    mIntroView.Render(mProjMatrix);
    break;

  case GS_MENU:
    if(m_bGamePause)
      m_RaceTrack.Render(mProjMatrix);
    break;

  case GS_GAME:
    m_RaceTrack.Render(mProjMatrix);
    break;

  case GS_HIGH:
    mHS.Render(mProjMatrix);
    break;

  default:
    break;
  }
}

void CGame::RenderGUI() {
  m_GUI.Begin(mConfig.Screen.GetSize());
  switch(m_uGameState) {
  case GS_INTRO:
    mIntroView.RenderUI(m_GUI);
    break;

  case GS_MENU:
    mMenuView.RenderUI(m_GUI);
    break;

  case GS_GAME:
    m_RaceTrack.RenderUI(m_GUI);
    break;

  case GS_HIGH:
    mHS.RenderUI(m_GUI);
    break;
  };
  if(mConfig.Diag.FPSCounter && m_uGameState != GS_INTRO) {
    glColor3f(1.0f, 1.0f, 1.0f);
    int fps = (int)mTimer.GetFramesPerSecond();
    m_GUI.Print(glm::vec2(530.0f, 5.0f), "FPS: %d", fps);
  }
  m_GUI.End();
}

void CGame::RenderMenu() {}

void CGame::TakeScreenshot() {
  //std::vector<BYTE> Buffer;
  //Buffer.resize(ScrParam.uWidth * ScrParam.uHeight * 4);
  //glReadPixels(0, 0, ScrParam.uWidth, ScrParam.uHeight, GL_BGRA, GL_UNSIGNED_BYTE, &Buffer[0]);
  //int i = 0;

  //BITMAPFILEHEADER bfh;
  //BITMAPINFOHEADER bih;

  //bfh.bfType = MAKEWORD('B', 'M');
  //bfh.bfSize = DWORD(sizeof(bfh) + sizeof(bih) + Buffer.size());
  //bfh.bfReserved1 = 0;
  //bfh.bfReserved2 = 0;
  //bfh.bfOffBits = sizeof(bfh) + sizeof(bih);

  //bih.biSize = sizeof(BITMAPINFOHEADER);
  //bih.biWidth = ScrParam.uWidth;
  //bih.biHeight = ScrParam.uHeight;
  //bih.biBitCount = ScrParam.uColorBits;
  //bih.biPlanes = 1;
  //bih.biCompression = BI_RGB;
  //bih.biSizeImage = DWORD(Buffer.size());
  //bih.biXPelsPerMeter = 0;
  //bih.biYPelsPerMeter = 0;
  //bih.biClrUsed = 0;
  //bih.biClrImportant = 0;

  //CFile fp;
  //std::string filename = "";

  //CIniFile ini;
  //ini.Open(m_strConfigFile);
  //i = ini.ReadInt("GENERAL", "uScreenShotNum", 1);
  //std::string scrname = ini.ReadString("GENERAL", "strScreenShotName", "ScreenShot");
  //int len = _scprintf("%s%02d.bmp", scrname.c_str(), i);
  //filename.resize(len + 1);
  //sprintf(&filename[0], "%s%02d.bmp", scrname.c_str(), i);
  //ini.WriteInt("GENERAL", "uScreenShotNum", i + 1);
  //ini.Close();

  //fp.Open(filename, "wb");

  //fp.Write(&bfh, sizeof(bfh));
  //fp.Write(&bih, sizeof(bih));
  //fp.Write(&Buffer[0], sizeof(BYTE), unsigned(Buffer.size()));

  //fp.Close();
  //Buffer.clear();
}