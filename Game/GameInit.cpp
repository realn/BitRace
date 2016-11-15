#include "Game.h"

#include <GL/wglew.h>

bool CGame::Init(std::string strCmdLine) {
  Log_Init("main.log", "BIT_RACE_LOG");

  m_strConfigFile = "config.ini";

  CIniFile ini;
  ini.Open(m_strConfigFile);

  ScrParam.uWidth = Uint32(ini.ReadInt("GRAPHIC", "uWidth", 640));
  ScrParam.uHeight = Uint32(ini.ReadInt("GRAPHIC", "uHeight", 480));
  ScrParam.uColorBits = Uint32(ini.ReadInt("GRAPHIC", "uColorBits", 32));
  ScrParam.uRefreshRate = Uint32(ini.ReadInt("GRAPHIC", "uRefreshRate", 60));
  ScrParam.bFullscreen = ini.ReadBool("GRAPHIC", "bFullscreen", true);
  ScrParam.bSmoothShade = ini.ReadBool("GRAPHIC", "bSmoothShade", true);
  ScrParam.bSmoothLines = ini.ReadBool("GRAPHIC", "bSmoothLines", true);
  ScrParam.bFPSCount = ini.ReadBool("GRAPHIC", "bFPSCount", false);
  ScrParam.bVSync = ini.ReadBool("GRAPHIC", "bVSync", true);
  ScrParam.bBlur = ini.ReadBool("GRAPHIC", "bBlur", false);
  m_uBlurTexSize = ini.ReadInt("GRAPHIC", "uBlurTexSize", 64);
  m_fBlurTexAlpha = ini.ReadFloat("GRAPHIC", "fBlurTexAlpha", 0.3f);

  ini.Close();

  if (!this->InitWindow(GAME_FULLNAME)) {
    Log_Error("Can't initialize window.");
    Free();
    return false;
  }
  if (!this->InitRender()) {
    Log_Error("Can't Initialize render.");
    Free();
    return false;
  }
  if(!this->InitInput()) {
    Log_Error("Can't initialize input.");
  }

  if (!this->InitOpenGL()) {
    Log_Error("Can't Initialize OpenGL");
    Free();
    return false;
  }
  if (!this->InitGame()) {
    Log_Error("Can't Initialize Final Game Settings");
    Free();
    return false;
  }

  return true;
}

bool CGame::InitWindow(std::string strTitle) {
  if(SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
    Log_Error("Failed initializing SDL.");
    return false;
  }

  int winFlags = SDL_WINDOW_OPENGL;
  if(ScrParam.bFullscreen) {
    winFlags |= SDL_WINDOW_FULLSCREEN;
    winFlags |= SDL_WINDOW_BORDERLESS;
  }
  this->m_pWindow = SDL_CreateWindow(strTitle.c_str(),
                                     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     int(ScrParam.uWidth), int(ScrParam.uHeight),
                                     winFlags);
  if(this->m_pWindow == nullptr) {
    Log_Error("Failed to create window.");
    return false;
  }

  SDL_ShowWindow(this->m_pWindow);
  return true;
}

bool CGame::InitRender() {
  if (ScrParam.bFullscreen)
    ChangeDispMode();

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, int(ScrParam.uColorBits));
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

  this->m_pGLContext = SDL_GL_CreateContext(this->m_pWindow);
  if(this->m_pGLContext == nullptr) {
    Log_Error("Failed to create OpenGL Context.");
    return false;
  }

  SDL_GL_MakeCurrent(this->m_pWindow, this->m_pGLContext);


  return true;
}

bool  CGame::InitInput() {
  SDL_InitSubSystem(SDL_INIT_EVENTS);

  memset(this->m_KeyState, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
  memset(this->m_KeyStatePrev, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);

  this->m_MouseButtonState = 0;
  this->m_MouseButtonStatePrev = 0;

  this->m_MousePos = glm::ivec2(0);
  this->m_MousePosPrev = glm::ivec2(0);

  SDL_SetRelativeMouseMode(SDL_TRUE);

  return true;
}

bool CGame::InitOpenGL() {
  if (glewInit() != GLEW_OK) {
    return false;
  }

  m_GUI.Init();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glClearStencil(0);

  if (ScrParam.bSmoothShade)
    glShadeModel(GL_SMOOTH);
  else glShadeModel(GL_FLAT);
  glDepthFunc(GL_LEQUAL);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  if (ScrParam.bSmoothLines)
    glEnable(GL_LINE_SMOOTH);
  else glDisable(GL_LINE_SMOOTH);

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_BLEND);

  glLineWidth(2.0f);
  glPointSize(1.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50.0, double(ScrParam.uWidth) / double(ScrParam.uHeight), 1.0, 50000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_GUI.Begin(glm::vec2(640.0f, 480.0f));
  m_GUI.Print(glm::vec2(100.0f, 200.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Please wait, loading game...");
  m_GUI.End();

  SDL_GL_SwapWindow(this->m_pWindow);

  glGenTextures(1, &m_uBlurTexture);
  glBindTexture(GL_TEXTURE_2D, m_uBlurTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, 4,
               m_uBlurTexSize, m_uBlurTexSize,
               0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  return true;
}

bool CGame::InitGame() {
  char		szBuffer[1000];

  this->m_iFreq = SDL_GetPerformanceFrequency();
  this->m_iLastTick = SDL_GetPerformanceCounter();

  srand((Uint32)this->m_iLastTick);


  this->ScanDispModes();

  CModel::InitModels();

  this->m_Racer.Init(CModel::MT_HTTP20);
  this->m_Racer.SetColor(0x90FF0000);
  this->m_RaceTrack.Init(&m_GUI, glm::vec2(640.0f, 480.0f));
  this->m_RaceTrack.SetRacer(&m_Racer);
  this->m_HS.LoadScores("score.hsf");

  if (WGLEW_EXT_swap_control) {
    if (ScrParam.bVSync)
      wglSwapIntervalEXT(1);
    else wglSwapIntervalEXT(0);
  }

  m_MenuMng.SetSize(ScrParam.GetSize());

	CGUIMenu* Menu = this->m_MenuMng.AddMenu(MENU_MAIN, "BitRace");
  Menu->AddMenuItem(MI_RETURN, "Return to Game")->SetEnable(false);
  Menu->AddMenuItem(MI_NEWGAME, "New Game");
  Menu->AddMenuItem(MI_HIGH, "High Scores", MENU_HIGH);
  Menu->AddMenuItem(MI_OPTIONS, "Options", MENU_OPTIONS);
  Menu->AddMenuItem(MI_EXIT, "Exit Game");
  Menu = this->m_MenuMng.AddMenu(MENU_OPTIONS, "Options");
  sprintf_s(szBuffer, 1000, "Resolution: %d X %d", ScrParam.uWidth, ScrParam.uHeight);
  Menu->AddMenuItem(MI_RESOLUTION, szBuffer, ScrParam.uDevID);
  Menu->AddMenuItem(MI_FULLSCREEN, (ScrParam.bFullscreen) ? "Fullscreen: Enabled" : "FullScreen: Disabled", Uint32(ScrParam.bFullscreen));
  Menu->AddMenuItem(MI_SMOOTHSHADE, (ScrParam.bSmoothShade) ? "Smooth Shading: Enabled" : "Smooth Shading: Disabled");
  Menu->AddMenuItem(MI_SMOOTHLINE, (ScrParam.bSmoothLines) ? "Smooth Lines: Enabled" : "Smooth Lines: Disabled");
  Menu->AddMenuItem(MI_FPSCOUNTER, (ScrParam.bFPSCount) ? "FPS Counter: Enabled" : "FPS Counter: Disabled");
  Menu->AddMenuItem(MI_VSYNC, (ScrParam.bVSync) ? "VSync: Enabled" : "VSync: Disabled");
  if (WGLEW_EXT_swap_control) {
    Menu->GetMenuItem(MI_VSYNC)->SetEnable(false);
  }
  Menu->AddMenuItem(MI_OPWARNING, "WARNING: You must restart the game, to apply changes")->SetEnable(false);
  Menu->AddMenuItem(MI_GOBACK, "Return to Main Menu", MENU_MAIN);
  Menu = this->m_MenuMng.AddMenu(MENU_HIGH, "High Scores");
  Menu->AddMenuItem(MI_HS1, "1. --EMPTY SCORE--");
  Menu->AddMenuItem(MI_HS2, "2. --EMPTY SCORE--");
  Menu->AddMenuItem(MI_HS3, "3. --EMPTY SCORE--");
  Menu->AddMenuItem(MI_HS4, "4. --EMPTY SCORE--");
  Menu->AddMenuItem(MI_HS5, "5. --EMPTY SCORE--");
  Menu->AddMenuItem(MI_HS6, "6. --EMPTY SCORE--");
  Menu->AddMenuItem(MI_HS7, "7. --EMPTY SCORE--");
  Menu->AddMenuItem(MI_HS8, "8. --EMPTY SCORE--");
  Menu->AddMenuItem(MI_HS9, "9. --EMPTY SCORE--");
  Menu->AddMenuItem(MI_HS10, "10. --EMPTY SCORE--");
  Menu->AddMenuItem(MI_HSRESET, "Reset Hight Scores");
  Menu->AddMenuItem(MI_GOBACK, "Return to Main Menu", MENU_MAIN);

  UpdateHS();

  m_Intro.Init("logos.fgx", ScrParam.GetSize());

  return true;
}

void CGame::ScanDispModes() {
  SDL_DisplayMode	curDispMode;
  memset(&curDispMode, 0, sizeof(SDL_DisplayMode));

  if (SDL_GetCurrentDisplayMode(0, &curDispMode) != 0)
    return;

  int dispNum = SDL_GetNumDisplayModes(0);
  for (int i = 0; i < dispNum; i++) {
    SDL_DisplayMode mode;
    memset(&mode, 0, sizeof(SDL_DisplayMode));
    if (SDL_GetDisplayMode(0, i, &mode) != 0) {
      continue;
    }

    if (mode.format != curDispMode.format || mode.refresh_rate != curDispMode.refresh_rate) {
      continue;
    }

    if (mode.w == curDispMode.w && mode.h == curDispMode.h) {
      ScrParam.uDevID = i;
    }

    this->m_ModeList.push_back(mode);
  }
}

void CGame::ChangeDispMode() {
  SDL_DisplayMode mode;

  if (SDL_GetCurrentDisplayMode(0, &mode) != 0)
    return;

  mode.w = ScrParam.uWidth;
  mode.h = ScrParam.uHeight;

  if (SDL_SetWindowDisplayMode(this->m_pWindow, &mode) != 0) {
    Log_Error("Can't change display settings to %ux%u", ScrParam.uWidth, ScrParam.uHeight);
    return;
  }
}
