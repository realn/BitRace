#include "stdafx.h"
#include "Game.h"
#include "IniFiles.h"
#include "GLDefines.h"

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

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50.0, 
                 double(mConfig.Screen.Width) / 
                 double(mConfig.Screen.Height), 
                 1.0, 50000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_GUI.Begin(glm::vec2(640.0f, 480.0f));
  glColor3f(1.0f, 1.0f, 1.0f);
  m_GUI.Print(glm::vec2(100.0f, 200.0f), "Please wait, loading game...");
  m_GUI.End();

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

  char szBuffer[1000];

  std::srand((Uint32)mTimer.GetLastTick());


  this->ScanDispModes();

  CModel::InitModels();

  this->m_Racer.Init(CModel::MT_HTTP20);
  this->m_Racer.SetColor(0x90FF0000);
  this->m_RaceTrack.Init();
  this->m_RaceTrack.SetRacer(&m_Racer);
  this->m_HS.LoadScores(L"score.hsf");

  //if(WGLEW_EXT_swap_control) {
  //  if(ScrParam.bVSync)
  //    wglSwapIntervalEXT(1);
  //  else wglSwapIntervalEXT(0);
  //}

  m_MenuMng.SetSize(mConfig.Screen.GetSize());

  CGUIMenu* Menu = this->m_MenuMng.AddMenu(MENU_MAIN, "BitRace");
  Menu->AddMenuItem(MI_RETURN, "Return to Game", glm::vec2(40.0f, 70.0f), 0)->SetEnable(false);
  Menu->AddMenuItem(MI_NEWGAME, "New Game", glm::vec2(40.0f, 100.0f), 0);
  Menu->AddMenuItem(MI_HIGH, "High Scores", glm::vec2(40.0f, 130.0f), MENU_HIGH);
  Menu->AddMenuItem(MI_OPTIONS, "Options", glm::vec2(40.0f, 160.0f), MENU_OPTIONS);
  Menu->AddMenuItem(MI_EXIT, "Exit Game", glm::vec2(40.0f, 190.0f), 0);
  Menu = this->m_MenuMng.AddMenu(MENU_OPTIONS, "Options");
  sprintf_s(szBuffer, 1000, "Resolution: %d X %d", 
            mConfig.Screen.Width, mConfig.Screen.Height);

  Menu->AddMenuItem(MI_RESOLUTION, szBuffer, glm::vec2(40.0f, 100.0f), mConfig.Screen.DevId);
  
  Menu->AddCheckBox(MI_FULLSCREEN, "Fullscreen", mConfig.Screen.Fullscreen, glm::vec2(40.0f, 130.0f));  
  Menu->AddCheckBox(MI_SMOOTHSHADE, "Smooth Shading", mConfig.Render.SmoothShade, glm::vec2(40.0f, 160.0f), 0);
  Menu->AddCheckBox(MI_SMOOTHLINE, "Smooth Lines", mConfig.Render.SmoothLines, glm::vec2(40.0f, 190.0f), 0);
  Menu->AddCheckBox(MI_FPSCOUNTER, "FPS Counter", mConfig.Diag.FPSCounter, glm::vec2(40.0f, 220.0f), 0);
  Menu->AddCheckBox(MI_VSYNC, "Vertical Sync", mConfig.Screen.VSync, glm::vec2(40.0f, 250.0f), 0);

  //if(WGLEW_EXT_swap_control) {
  //  Menu->GetMenuItem(MI_VSYNC)->SetEnable(false);
  //}

  Menu->AddMenuItem(MI_OPWARNING, "WARNING: You must restart the game, to apply changes", glm::vec2(20.0f, 300.0f), 0)->SetEnable(false);
  Menu->AddMenuItem(MI_GOBACK, "Return to Main Menu", glm::vec2(40.0f, 330.0f), MENU_MAIN);
  Menu = this->m_MenuMng.AddMenu(MENU_HIGH, "High Scores");
  Menu->AddMenuItem(MI_HS1, "1. --EMPTY SCORE--", glm::vec2(40.0f, 100.0f), 0);
  Menu->AddMenuItem(MI_HS2, "2. --EMPTY SCORE--", glm::vec2(40.0f, 120.0f), 0);
  Menu->AddMenuItem(MI_HS3, "3. --EMPTY SCORE--", glm::vec2(40.0f, 140.0f), 0);
  Menu->AddMenuItem(MI_HS4, "4. --EMPTY SCORE--", glm::vec2(40.0f, 160.0f), 0);
  Menu->AddMenuItem(MI_HS5, "5. --EMPTY SCORE--", glm::vec2(40.0f, 180.0f), 0);
  Menu->AddMenuItem(MI_HS6, "6. --EMPTY SCORE--", glm::vec2(40.0f, 200.0f), 0);
  Menu->AddMenuItem(MI_HS7, "7. --EMPTY SCORE--", glm::vec2(40.0f, 220.0f), 0);
  Menu->AddMenuItem(MI_HS8, "8. --EMPTY SCORE--", glm::vec2(40.0f, 240.0f), 0);
  Menu->AddMenuItem(MI_HS9, "9. --EMPTY SCORE--", glm::vec2(40.0f, 260.0f), 0);
  Menu->AddMenuItem(MI_HS10, "10. --EMPTY SCORE--", glm::vec2(40.0f, 280.0f), 0);
  Menu->AddMenuItem(MI_HSRESET, "Reset Hight Scores", glm::vec2(40.0f, 400.0f), 0);
  Menu->AddMenuItem(MI_GOBACK, "Return to Main Menu", glm::vec2(40.0f, 440.0f), MENU_MAIN);

  UpdateHS();

  m_Intro.Init(L"logos.fgx");

  return true;
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
