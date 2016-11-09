#include "Game.h"

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")

PFNWGLSWAPINTERVALEXTPROC		wglSwapIntervalEXT = NULL;
PFNWGLGETSWAPINTERVALEXTPROC	wglGetSwapIntervalEXT = NULL;

CGame::CGame() :
  m_pWindow(NULL),
  m_pGLContext(NULL),
  m_cDInput(NULL),
  m_cDIKey(NULL),
  m_cDIMouse(NULL),
  m_bShutdown(false),
  m_bGamePause(false),
  m_bTakeScreen(false),
  m_iLastTick(0),
  m_iFreq(0),
  m_fDT(0.0f),
  m_fBlurTexAlpha(0.3f),
  m_uGameState(GS_INTRO),
  m_uBlurTexture(0),
  m_uBlurTexSize(64) 
{
  SDL_Init(0);

  GInst = this;
}

bool CGame::Init(std::string strCmdLine) {
  Log_Init("main.log", "BIT_RACE_LOG");

  m_strConfigFile = "config.ini";

  CIniFile ini;
  ini.Open(m_strConfigFile);

  ScrParam.uWidth = UINT(ini.ReadInt("GRAPHIC", "uWidth", 640));
  ScrParam.uHeight = UINT(ini.ReadInt("GRAPHIC", "uHeight", 480));
  ScrParam.uColorBits = UINT(ini.ReadInt("GRAPHIC", "uColorBits", 32));
  ScrParam.uRefreshRate = UINT(ini.ReadInt("GRAPHIC", "uRefreshRate", 60));
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
    Log_Error("Can't Create Window...");
    Free();
    return false;
  }
  if (!this->InitDevice()) {
    Log_Error("Can't Initialize Devices");
    Free();
    return false;
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
                                     0, 0,
                                     int(ScrParam.uWidth), int(ScrParam.uHeight),
                                     winFlags);
  if(this->m_pWindow == nullptr) {
    Log_Error("Failed to create window.");
    return false;
  }

  SDL_ShowWindow(this->m_pWindow);
  return true;
}

bool CGame::InitDevice() {
  if (ScrParam.bFullscreen)
    ChangeDispMode();

  if (DI_OK != DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&this->m_cDInput, NULL)) {
    this->FreeDevice();
    return false;
  }
  if (DI_OK != this->m_cDInput->CreateDevice(GUID_SysKeyboard, &this->m_cDIKey, NULL)) {
    this->FreeDevice();
    return false;
  }
  if (DI_OK != this->m_cDInput->CreateDevice(GUID_SysMouse, &this->m_cDIMouse, NULL)) {
    this->FreeDevice();
    return false;
  }

  HWND pWindow = reinterpret_cast<HWND>(SDL_GetWindowID(this->m_pWindow));

  this->m_cDIKey->SetDataFormat(&c_dfDIKeyboard);
  this->m_cDIKey->SetCooperativeLevel(pWindow, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
  this->m_cDIKey->Acquire();

  this->m_cDIMouse->SetDataFormat(&c_dfDIMouse2);
  this->m_cDIMouse->SetCooperativeLevel(pWindow, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
  this->m_cDIMouse->Acquire();

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

  m_cGUI.Init(NULL);
  srand(GetTickCount());

  return true;
}

bool CGame::InitOpenGL() {
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

  CHECK_GL_ERRORS();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_cGUI.GUIMode_Start();
  glColor3f(1.0f, 1.0f, 1.0f);
  m_cGUI.Print(100.0f, 200.0f, "Please wait, loading game...");
  m_cGUI.GUIMode_End();

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

  if (!QueryPerformanceFrequency((LARGE_INTEGER*)&m_iFreq)) {
    Log_Error("Can't recover CPU Timer Frequency - can't continue without a Timer.");
    Free();
    return false;
  }
  else QueryPerformanceCounter((LARGE_INTEGER*)&m_iLastTick);

  this->ScanDispModes();

  CModel::InitModels();

  this->m_Racer.Init(CModel::MT_HTTP20);
  this->m_Racer.SetColor(0x90FF0000);
  this->m_RaceTrack.Init();
  this->m_RaceTrack.SetRacer(&m_Racer);
  this->m_HS.LoadScores("score.hsf");

  wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
  wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
  if (wglSwapIntervalEXT != NULL) {
    if (ScrParam.bVSync)
      wglSwapIntervalEXT(1);
    else wglSwapIntervalEXT(0);
  }

  CGUIMenu* Menu = this->m_MMag.AddMenu(MENU_MAIN, "BitRace");
  Menu->AddMenuItem(MI_RETURN, "Return to Game", vec2(40.0f, 70.0f), 0)->SetEnable(false);
  Menu->AddMenuItem(MI_NEWGAME, "New Game", vec2(40.0f, 100.0f), 0);
  Menu->AddMenuItem(MI_HIGH, "High Scores", vec2(40.0f, 130.0f), MENU_HIGH);
  Menu->AddMenuItem(MI_OPTIONS, "Options", vec2(40.0f, 160.0f), MENU_OPTIONS);
  Menu->AddMenuItem(MI_EXIT, "Exit Game", vec2(40.0f, 190.0f), 0);
  Menu = this->m_MMag.AddMenu(MENU_OPTIONS, "Options");
  sprintf_s(szBuffer, 1000, "Resolution: %d X %d", ScrParam.uWidth, ScrParam.uHeight);
  Menu->AddMenuItem(MI_RESOLUTION, szBuffer, vec2(40.0f, 100.0f), ScrParam.uDevID);
  Menu->AddMenuItem(MI_FULLSCREEN, (ScrParam.bFullscreen) ? "Fullscreen: Enabled" : "FullScreen: Disabled", vec2(40.0f, 130.0f), UINT(ScrParam.bFullscreen));
  Menu->AddMenuItem(MI_SMOOTHSHADE, (ScrParam.bSmoothShade) ? "Smooth Shading: Enabled" : "Smooth Shading: Disabled", vec2(40.0f, 160.0f), 0);
  Menu->AddMenuItem(MI_SMOOTHLINE, (ScrParam.bSmoothLines) ? "Smooth Lines: Enabled" : "Smooth Lines: Disabled", vec2(40.0f, 190.0f), 0);
  Menu->AddMenuItem(MI_FPSCOUNTER, (ScrParam.bFPSCount) ? "FPS Counter: Enabled" : "FPS Counter: Disabled", vec2(40.0f, 220.0f), 0);
  Menu->AddMenuItem(MI_VSYNC, (ScrParam.bVSync) ? "VSync: Enabled" : "VSync: Disabled", vec2(40.0f, 250.0f), 0);
  if (wglSwapIntervalEXT == NULL)
    Menu->GetMenuItem(MI_VSYNC)->SetEnable(false);
  Menu->AddMenuItem(MI_OPWARNING, "WARNING: You must restart the game, to apply changes", vec2(20.0f, 300.0f), 0)->SetEnable(false);
  Menu->AddMenuItem(MI_GOBACK, "Return to Main Menu", vec2(40.0f, 330.0f), MENU_MAIN);
  Menu = this->m_MMag.AddMenu(MENU_HIGH, "High Scores");
  Menu->AddMenuItem(MI_HS1, "1. --EMPTY SCORE--", vec2(40.0f, 100.0f), 0);
  Menu->AddMenuItem(MI_HS2, "2. --EMPTY SCORE--", vec2(40.0f, 120.0f), 0);
  Menu->AddMenuItem(MI_HS3, "3. --EMPTY SCORE--", vec2(40.0f, 140.0f), 0);
  Menu->AddMenuItem(MI_HS4, "4. --EMPTY SCORE--", vec2(40.0f, 160.0f), 0);
  Menu->AddMenuItem(MI_HS5, "5. --EMPTY SCORE--", vec2(40.0f, 180.0f), 0);
  Menu->AddMenuItem(MI_HS6, "6. --EMPTY SCORE--", vec2(40.0f, 200.0f), 0);
  Menu->AddMenuItem(MI_HS7, "7. --EMPTY SCORE--", vec2(40.0f, 220.0f), 0);
  Menu->AddMenuItem(MI_HS8, "8. --EMPTY SCORE--", vec2(40.0f, 240.0f), 0);
  Menu->AddMenuItem(MI_HS9, "9. --EMPTY SCORE--", vec2(40.0f, 260.0f), 0);
  Menu->AddMenuItem(MI_HS10, "10. --EMPTY SCORE--", vec2(40.0f, 280.0f), 0);
  Menu->AddMenuItem(MI_HSRESET, "Reset Hight Scores", vec2(40.0f, 400.0f), 0);
  Menu->AddMenuItem(MI_GOBACK, "Return to Main Menu", vec2(40.0f, 440.0f), MENU_MAIN);

  UpdateHS();

  m_Intro.Init("logos.fgx");

  return true;
}

void CGame::ScanDispModes() {
  DEVMODEA	CurMode;
  DEVMODEA	DMode;

  memset(&CurMode, 0, sizeof(DEVMODEA));
  memset(&DMode, 0, sizeof(DEVMODEA));
  this->m_aMode.clear();

  CurMode.dmSize = sizeof(DEVMODEA);
  DMode.dmSize = sizeof(DEVMODEA);

  EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &CurMode);

  DISPMODEID	DMI;
  DWORD		i = 0;
  while (EnumDisplaySettingsA(NULL, i, &DMode)) {
    if (DMode.dmBitsPerPel != 32 || DMode.dmDisplayFrequency != CurMode.dmDisplayFrequency) {
      i++;
      continue;
    }

    DMI.uWidth = DMode.dmPelsWidth;
    DMI.uHeight = DMode.dmPelsHeight;
    DMI.uRefreshRate = DMode.dmDisplayFrequency;
    DMI.uID = i;

    if (DMI.uWidth == ScrParam.uWidth && DMI.uHeight == ScrParam.uHeight)
      ScrParam.uDevID = DMI.uID;

    this->m_aMode.push_back(DMI);

    memset(&DMode, 0, sizeof(DEVMODEA));
    DMode.dmSize = sizeof(DEVMODEA);
    i++;
  }
}

void CGame::ChangeDispMode() {
  DEVMODEA	CurMode;

  memset(&CurMode, 0, sizeof(DEVMODEA));
  CurMode.dmSize = sizeof(DEVMODEA);

  //if( !EnumDisplaySettingsA( NULL, ENUM_CURRENT_SETTINGS, &CurMode ) )
  //{
  //	Log_Error( "Can't read current display settings" );
  //	return;
  //}

  CurMode.dmBitsPerPel = 32;
  CurMode.dmPelsWidth = ScrParam.uWidth;
  CurMode.dmPelsHeight = ScrParam.uHeight;
  CurMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;// | DM_DISPLAYFREQUENCY;

  if (ChangeDisplaySettingsExA(NULL, &CurMode, NULL, CDS_FULLSCREEN, NULL) != DISP_CHANGE_SUCCESSFUL) {
    Log_Error("Can't change display settings to %ux%u", ScrParam.uWidth, ScrParam.uHeight);
    return;
  }
}