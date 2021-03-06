#pragma once

#define GAME_NAME "BitRace"
#define GAME_FULLNAME L"CodeRulers BitRace v1.0a"

#include <SDL.h>

#include <glm/glm.hpp>

#include <CBLog/Logger.h>
#include <CBIO/File.h>

#include "Config.h"
#include "PerfTimer.h"
#include "InputSystem.h"

#include "UIFont.h"

class IFileSystem;
class IEngineState;
class CModelRepository;

class CEngine {
private:
  std::wofstream mLogFile;
  cb::CLogger mLogger;

  IFileSystem* mpFileSystem;
  CConfig mConfig;
  CPerfTimer mTimer;
  CInputSystem mInput;

  cb::string mConfigFilePath;
  cb::string mInputConfigFilePath;

  SDL_Window*   mpWindow;
  SDL_GLContext mpGLContext;

  float mFrameTime;
  float mFrameStepTime;

  bool mInited;
  bool mRun;

  CModelRepository* mModelRepo;
  IEngineState* mState;

public:
  CEngine();
  ~CEngine();

  int MainLoop(const cb::string& cmdLine);

  void SaveConfig();
  void LoadConfig();

  void SaveInputConfig();
  void LoadInputConfig();

private:
  const bool Init(const cb::string& cmdLine);
  const bool InitDisplay(const cb::string& title);
  const bool InitInput();
  const bool InitGame();

  void Free();
  void FreeDisplay();
  void FreeInput();
  void FreeGame();

  void Update();
  void UpdateFrame(const float timeDelta);

  void Render();
  void RenderFrame();

  void ProcessEvents();
  void ProcessEvent(const SDL_Event& event);
};