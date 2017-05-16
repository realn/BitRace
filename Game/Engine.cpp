#include "stdafx.h"
#include "Engine.h"
#include "BasicFileSystem.h"
#include "Model.h"
#include "GameState.h"
#include "TestState.h"

#include "GameEntity.h"
#include <CBXml/Serialize.h>

CEngine::CEngine()
  : mpFileSystem(NULL)
  , mpWindow(NULL)
  , mpGLContext(NULL)
  , mFrameTime(0.0f)
  , mFrameStepTime(0.01f)
  , mInited(false)
  , mRun(true)
  , mState(nullptr) {

  mLogFile.open(L"game.log", std::ios::out | std::ios::trunc);
  mLogger.AddStream(&mLogFile);

  cb::CLogger::SetInstance(&mLogger);

  cb::info(L"Initializing SDL.");
  SDL_Init(0);

  cb::info(L"Creating filesystem.");
  mpFileSystem = new CBasicFileSystem();
}

CEngine::~CEngine() {
  if(mInited) {
    Free();
  }

  cb::info(L"Destroing filesystem.");
  delete mpFileSystem;

  cb::info(L"Quiting SDL.");
  SDL_Quit();

  cb::CLogger::SetInstance(nullptr);
}

int CEngine::MainLoop(const cb::string& cmdLine) {
  SDL_Event event;

  if(!Init(cmdLine)) {
    return -1;
  }

  cb::info(L"Entering main loop.");
  while(mRun) {

    if(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        mRun = false;
      }
    }

    Update();
    Render();
  }
  cb::info(L"Leaving main loop.");

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

const bool CEngine::Init(const cb::string& cmdLine) {
  if(mInited) {
    cb::warn(L"Engine already initialized.");
    return true;
  }

  cb::info(L"Initializing engine.");
  mConfigFilePath = L"main.cfg";
  LoadConfig();

  if(!InitDisplay(GAME_FULLNAME)) {
    cb::error(L"Failed to initialize main display.");
    return false;
  }
  if(!InitInput()) {
    cb::error(L"Can't initialize input.");
    return false;
  }
  if(!InitGame()) {
    cb::error(L"Can't Initialize Final Game Settings");
    return false;
  }

  mTimer.Update();
  mInited = true;
  cb::info(L"Engine initialized.");
  return true;
}

const bool CEngine::InitDisplay(const cb::string & title) {
  cb::info(L"Initializing display.");

  cb::debug(L"Initializing SDL VIDEO.");
  if(SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
    cb::error(L"Failed initializing SDL VIDEO.");
    return false;
  }

  int winFlags = SDL_WINDOW_OPENGL;
  if(mConfig.Screen.Fullscreen) {
    winFlags |= SDL_WINDOW_FULLSCREEN;
    winFlags |= SDL_WINDOW_BORDERLESS;
  }

  cb::charvector szTitle = cb::toUtf8(title);
  szTitle.push_back(0);

  cb::debug(L"Creating window titled " + title);
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

  cb::debug(L"Seting up OpenGL Context creation.");
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, int(mConfig.Screen.ColorBits));
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

  cb::debug(L"Creating OpenGL Context.");
  mpGLContext = SDL_GL_CreateContext(mpWindow);
  if(mpGLContext == nullptr) {
    cb::error(L"Failed to create OpenGL Context.");
    return false;
  }

  cb::debug(L"Binding OpenGL Context to window.");
  SDL_GL_MakeCurrent(mpWindow, mpGLContext);

  cb::debug(L"Initialzing GLEW.");
  GLenum glewStatus = glewInit();
  if(glewStatus != GLEW_OK) {
    cb::charvector szErr = 
      cb::utf8vec(reinterpret_cast<const char*>(glewGetErrorString(glewStatus)));
    cb::error(cb::format(L"Failed to initialize glew, error: {0}.",
                         cb::fromUtf8(szErr)));
    return false;
  }

  {
    cb::charvector exts =
      cb::utf8vec(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));
    cb::debug(L"Extensions: " + cb::fromUtf8(exts));
  }

  cb::debug(L"Seting up OpenGL State.");
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

  cb::info(L"Display initialized.");
  return true;
}

const bool  CEngine::InitInput() {
  cb::info(L"Initializing input.");
  cb::debug(L"Initializing SDL EVENTS");
  SDL_InitSubSystem(SDL_INIT_EVENTS);

  cb::debug(L"Adding input devices.");
  mIDevMap.AddDevice(InputDevice::Keyboard, new CKeyboardInputDevice());
  mIDevMap.AddDevice(InputDevice::Mouse, new CMouseInputDevice(mConfig.Screen.GetSize()));

  cb::info(L"Input initialized.");
  return true;
}

const bool CEngine::InitGame() {
  cb::info(L"Initializing game.");
  if(!mTimer.Init()) {
    return false;
  }
  std::srand((Uint32)mTimer.GetLastTick());

  mModelRepo = new CModelRepository(mpFileSystem);

  CGameState* pState = new CGameState(mConfig,
                                      mIDevMap,
                                      mModelRepo);
  if(!pState->LoadResources(*mpFileSystem)) {
    cb::error(L"Failed to initialize game state.");
    delete pState;
    return false;
  }
  //CTestState* pState = new CTestState();
  mState = pState;

  cb::info(L"Game initialized.");
  return true;
}

void CEngine::Free() {
  if(!mInited) {
    cb::warn(L"Engine already freed.");
    return;
  }

  cb::info(L"Freeing engine.");
  FreeGame();
  FreeInput();
  FreeDisplay();
  mInited = false;
  cb::info(L"Engine freed.");
}

void CEngine::FreeDisplay() {
  cb::info(L"Freeing display.");
  if(mpGLContext) {
    cb::debug(L"Deleting OpenGL Context.");
    SDL_GL_DeleteContext(mpGLContext);
    mpGLContext = nullptr;
  }

  if(mpWindow) {
    cb::debug(L"Destroing window.");
    SDL_DestroyWindow(mpWindow);
    mpWindow = nullptr;
  }

  cb::debug(L"Quiting SDL VIDEO.");
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  cb::info(L"Display freed.");
}

void CEngine::FreeInput() {
  cb::info(L"Freeing input.");
  cb::debug(L"Clearing devices.");
  mIDevMap.Clear();

  cb::debug(L"Quiting SDL EVENTS.");
  SDL_QuitSubSystem(SDL_INIT_EVENTS);
  cb::info(L"Input freed.");
}

void CEngine::FreeGame() {
  cb::info(L"Freeing game.");

  if(mState != nullptr) {
    cb::debug(L"Removing state.");
    delete mState;
    mState = nullptr;
  }

  cb::debug(L"Clearing 3d model cache.");
  delete mModelRepo;
  mModelRepo = nullptr;
  cb::info(L"Game freed.");
}

void CEngine::Update() {
  mTimer.Update();

  mFrameTime += mTimer.GetTimeDelta();
  if(mFrameTime > mFrameStepTime) {
    while(mFrameTime > mFrameStepTime) {
      mIDevMap.Update(mFrameStepTime);
      UpdateFrame(mFrameStepTime);

      mFrameTime -= mFrameStepTime;
    }
  }
  else if(mFrameTime > 0.0f) {
    mIDevMap.Update(mFrameTime);
    UpdateFrame(mFrameTime);
    mFrameTime = 0.0f;
  }

  if(mState != nullptr) {
    mState->UpdateRender(mTimer.GetTimeDelta());
  }
}

void CEngine::UpdateFrame(const float timeDelta) {
  if(mState != nullptr) {
    mState->Update(timeDelta);
  }
}

void CEngine::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  RenderFrame();

  SDL_GL_SwapWindow(mpWindow);
}

void CEngine::RenderFrame() {
  if(mState == nullptr)
    return;

  mState->Render();
  mState->RenderUI();
}
