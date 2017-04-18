#include "stdafx.h"
#include "Engine.h"
#include "BasicFileSystem.h"
#include "Model.h"
#include "GraphicView.h"
#include "LogicProcess.h"
#include "Menu.h"

CEngine::CEngine()
  : mpFileSystem(NULL)
  , mpWindow(NULL)
  , mpGLContext(NULL)
  , mFrameTime(0.0f)
  , mFrameStepTime(0.01f)
  , mRun(true) 
  , mState(0)
{

  mLogFile.open(L"game.log", std::ios::out | std::ios::trunc);
  mLogger.AddStream(&mLogFile);

  cb::CLogger::SetInstance(&mLogger);

  SDL_Init(0);

  mpFileSystem = new CBasicFileSystem();
}

CEngine::~CEngine() {
  Free();

  delete mpFileSystem;

  SDL_Quit();

  cb::CLogger::SetInstance(nullptr);
}

const bool CEngine::Init(const cb::string& cmdLine) {
  mConfigFilePath = L"main.cfg";
  LoadConfig();

  if(!InitDisplay(GAME_FULLNAME)) {
    cb::error(L"Failed to initialize main display.");
    Free();
    return false;
  }
  if(!InitInput()) {
    cb::error(L"Can't initialize input.");
    Free();
    return false;
  }
  if(!InitGame()) {
    cb::error(L"Can't Initialize Final Game Settings");
    Free();
    return false;
  }

  mTimer.Update();
  return true;
}

void CEngine::Free() {
  FreeGame();
  FreeInput();
  FreeDisplay();
}


int CEngine::MainLoop() {
  SDL_Event event;

  while(mRun) {

    if(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        mRun = false;
      }
    }

    Update();
    Render();
  }

  Free();
  return 0;
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
  szTitle.push_back(0);
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

  return true;
}

const bool  CEngine::InitInput() {
  SDL_InitSubSystem(SDL_INIT_EVENTS);

  mIDevMap.AddDevice(InputDevice::Keyboard, new CKeyboardInputDevice());
  mIDevMap.AddDevice(InputDevice::Mouse, new CMouseInputDevice(mConfig.Screen.GetSize()));

  return true;
}

const bool CEngine::InitGame() {
  if(!mTimer.Init()) {
    return false;
  }
  std::srand((Uint32)mTimer.GetLastTick());

  {
    CMenuProcess* pProcess = new CMenuProcess(mIDevMap);
    pProcess->Init();

    mLogicProcessMap[1] = pProcess;

    CMenuView* pView = new CMenuView(*pProcess, mConfig.Screen.GetSize());
    pView->Init();

    mGraphicViewMap[1] = pView;
  }

  mState = 1;

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

void CEngine::FreeInput() {
  SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void CEngine::FreeGame() {
  for(GraphicViewMapT::iterator it = mGraphicViewMap.begin(); it != mGraphicViewMap.end(); it++) {
    delete it->second;
  }
  mGraphicViewMap.clear();

  for(LogicProcessMapT::iterator it = mLogicProcessMap.begin(); it != mLogicProcessMap.end(); it++) {
    delete it->second;
  }
  mLogicProcessMap.clear();

  CModelRepository::Instance.Clear();
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
  LogicProcessMapT::iterator it = mLogicProcessMap.find(mState);
  if(it == mLogicProcessMap.end()) {
    return;
  }

  it->second->Update(timeDelta);
}

void CEngine::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  RenderFrame();

  SDL_GL_SwapWindow(mpWindow);
}

void CEngine::RenderFrame() {
  GraphicViewMapT::iterator it = mGraphicViewMap.find(mState);
  if(it == mGraphicViewMap.end()) {
    return;
  }

  it->second->RenderView();
  it->second->RenderUI();
}
