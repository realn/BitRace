#include "stdafx.h"
#include "Engine.h"
#include "BasicFileSystem.h"
#include "UIMenu.h"

CEngine::CEngine()
  : mpFileSystem(NULL)
  , mpWindow(NULL)
  , mpGLContext(NULL)
  , mUIFont()
  , mUIText()
  , m_GUI()
  , mIntroProcess()
  , mMenuProcess(m_GUI, mIDevMap)
  , mGameProcess(mConfig, mIDevMap)
  , mIntroView(mIntroProcess)
  , mMenuView(mMenuProcess)
  , mGameView(mGameProcess)
  , mHS(mIDevMap)
  , mFrameTime(0.0f)
  , mFrameStepTime(0.01f)
  , m_bShutdown(false)
  , m_uGameState(GS_INTRO) {
  mLogFile.open(L"game.log", std::ios::out | std::ios::trunc);
  mLogger.AddStream(&mLogFile);

  cb::CLogger::SetInstance(&mLogger);

  SDL_Init(0);

  mpFileSystem = new CBasicFileSystem();
}

CEngine::~CEngine() {
  Free();
  SDL_Quit();

  cb::CLogger::SetInstance(nullptr);

  delete mpFileSystem;
}

const bool CEngine::Init(const cb::string& cmdLine) {
  mConfigFilePath = L"main.cfg";
  LoadConfig();

  if(!InitDisplay(GAME_FULLNAME)) {
    cb::error(L"Failed to initialize main display.");
    Free();
    return false;
  }
  if(!this->InitInput()) {
    cb::error(L"Can't initialize input.");
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

bool  CEngine::InitInput() {
  SDL_InitSubSystem(SDL_INIT_EVENTS);

  mIDevMap.AddDevice(InputDevice::Keyboard, new CKeyboardInputDevice());
  mIDevMap.AddDevice(InputDevice::Mouse, new CMouseInputDevice(mConfig.Screen.GetSize()));

  return true;
}

bool CEngine::InitGame() {
  if(!mTimer.Init()) {
    return false;
  }


  std::srand((Uint32)mTimer.GetLastTick());

  m_GUI.Init();

  mGameProcess.Init();
  this->mHS.LoadScores(L"score.hsf");

  if(!mMenuProcess.Init(mConfig)) {
    cb::error(L"Failed to initialize menu process.");
    return false;
  }
  mMenuProcess.AddObserver(this);

  mIntroView.Init(L"logos.fgx");
  mMenuView.Init(mConfig.Screen.GetSize());

  return true;
}

void CEngine::Free() {
  this->FreeGame();
  this->FreeInput();
  FreeDisplay();
}

void CEngine::FreeInput() {
  SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void CEngine::FreeGame() {
  mMenuProcess.RemoveObserver(this);
  mGameProcess.Free();
  m_GUI.Free();
  mIntroView.Free();
  CModelRepository::Instance.Clear();
}

int CEngine::MainLoop() {
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

const bool CEngine::InitDisplay(const cb::string & title) {
  if(SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
    cb::error(L"Failed initializing SDL.");
    return false;
  }

  int winFlags = SDL_WINDOW_OPENGL;
  if(mConfig.Screen.Fullscreen) {
    winFlags |= SDL_WINDOW_FULLSCREEN;
    winFlags |= SDL_WINDOW_BORDERLESS;
  }

  cb::charvector szTitle = cb::toUtf8(title);
  mpWindow = SDL_CreateWindow(cb::vectorptr(szTitle),
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               int(mConfig.Screen.Width),
                               int(mConfig.Screen.Height),
                               winFlags);

  if(mpWindow == nullptr) {
    cb::error(L"Failed to create window.");
    return false;
  }

  SDL_ShowWindow(mpWindow);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, int(mConfig.Screen.ColorBits));
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

  mpGLContext = SDL_GL_CreateContext(mpWindow);
  if(mpGLContext == nullptr) {
    cb::error(L"Failed to create OpenGL Context.");
    return false;
  }

  SDL_GL_MakeCurrent(mpWindow, mpGLContext);

  if(glewInit() != GLEW_OK) {
    return false;
  }

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glClearStencil(0);

  glDepthFunc(GL_LEQUAL);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  if(mConfig.Render.SmoothShade)
    glShadeModel(GL_SMOOTH);
  else
    glShadeModel(GL_FLAT);


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

  return true;
}

void CEngine::FreeDisplay() {
  if(mpGLContext) {
    SDL_GL_DeleteContext(mpGLContext);
    mpGLContext = nullptr;
  }

  if(mpWindow) {
    SDL_DestroyWindow(mpWindow);
    mpWindow = nullptr;
  }

  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void CEngine::SaveConfig() {
  cb::info(cb::format(L"Saving config file to path {0}.", mConfigFilePath));
  if(!mConfig.Write(*mpFileSystem, mConfigFilePath)) {
    cb::error(L"Failed to save config file.");
  }
}

void CEngine::LoadConfig() {
  cb::info(cb::format(L"Loading config file from path {0}.", mConfigFilePath));
  if(!mConfig.Read(*mpFileSystem, mConfigFilePath)) {
    cb::error(L"Failed to load config file, reseting fo default.");
    mConfig = CConfig();
  }
}

const CEngine::GAME_STATE CEngine::GetNextState() const {
  switch(m_uGameState) {
  case GS_INTRO:  return GS_MENU;
  case GS_MENU:   return GS_MENU;
  case GS_GAME:
    {
      if(mGameProcess.IsGameOver())
        return GS_HIGH;
      else
        return GS_MENU;
    }
  case GS_HIGH: return GS_MENU;

  default:
    return GS_INTRO;
  }
}

void CEngine::Update() {
  mTimer.Update();

  mFrameTime += mTimer.GetTimeDelta();
  while(mFrameTime > mFrameStepTime) {
    mIDevMap.Update(mFrameStepTime);
    UpdateLogic(mFrameStepTime);

    mFrameTime -= mFrameStepTime;
  }
}

void CEngine::UpdateLogic(const float timeDelta) {
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
      mGameProcess.Update(timeDelta);
    break;

  case GS_HIGH:
    mHS.Update(timeDelta);
    if(mHS.IsEnded()) {
      mHS.SaveScores(L"score.hsf");
      mMenuProcess.GetMenuManager().ForceSwitchToMenu(CMenuProcess::MENU_HIGH);
      m_uGameState = GS_MENU;
    }
    break;
  };
}

void CEngine::UpdateGame(const float timeDelta) {}

void CEngine::MenuItemAction(CGUIMenuManager& menuMng, CGUIMenu& menu, CGUIMenuItem& item) {
  switch(menu.GetClickedID()) {
  case CMenuProcess::MI_RETURN:
    m_uGameState = GS_GAME;
    break;

  case CMenuProcess::MI_NEWGAME:
    mGameProcess.ResetGame();
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
    mHS.SaveScores(L"score.hsf");
    break;
  };
}

void CEngine::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  RenderGame();
  RenderGUI();

  SDL_GL_SwapWindow(mpWindow);
}

void CEngine::RenderGame() {
  switch(m_uGameState) {
  case GS_INTRO:
    mIntroView.Render(mProjMatrix);
    break;

  case GS_MENU:
    mMenuView.Render(mProjMatrix);
    break;

  case GS_GAME:
    mGameView.Render(mProjMatrix);
    break;

  case GS_HIGH:
    mHS.Render(mProjMatrix);
    break;

  default:
    break;
  }
}

void CEngine::RenderGUI() {
  m_GUI.Begin(mConfig.Screen.GetSize());
  switch(m_uGameState) {
  case GS_INTRO:
    mIntroView.RenderUI(m_GUI);
    break;

  case GS_MENU:
    mMenuView.RenderUI(m_GUI);
    break;

  case GS_GAME:
    mGameView.RenderUI(m_GUI);
    break;

  case GS_HIGH:
    mHS.RenderUI(m_GUI);
    break;
  };
  m_GUI.End();
}