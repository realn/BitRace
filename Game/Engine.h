#pragma once

#define GAME_NAME "BitRace"
#define GAME_FULLNAME L"CodeRulers BitRace v1.0a"
#define GAME_DI_BUFSIZE 32
#define	GAME_BLEND_TEX_SIZE	64

#include <SDL.h>

#include <glm/glm.hpp>

#include <CBLog/Logger.h>
#include <CBIO/File.h>

#include "Config.h"
#include "PerfTimer.h"
#include "InputDevice.h"

#include "UIFont.h"

class IFileSystem;
class ILogicProcess;
class IGraphicView;

class CEngine {
private:
  typedef std::map<Uint32, ILogicProcess*> LogicProcessMapT;
  typedef std::map<Uint32, IGraphicView*> GraphicViewMapT;

  std::wofstream mLogFile;
  cb::CLogger mLogger;

  IFileSystem* mpFileSystem;
  CConfig mConfig;
  CPerfTimer mTimer;
  CInputDeviceMap mIDevMap;

  cb::string mConfigFilePath;

  SDL_Window*   mpWindow;
  SDL_GLContext mpGLContext;

  float mFrameTime;
  float mFrameStepTime;
  bool	mRun;

  LogicProcessMapT mLogicProcessMap;
  GraphicViewMapT mGraphicViewMap;

  Uint32 mState;

public:
  CEngine();
  ~CEngine();

  const bool Init(const cb::string& cmdLine);

  void Free();

  int MainLoop();

private:
  void SaveConfig();
  void LoadConfig();

  const bool InitDisplay(const cb::string& title);
  const bool InitInput();
  const bool InitGame();

  void FreeDisplay();
  void FreeInput();
  void FreeGame();

  void Update();
  void UpdateLogic(const float timeDelta);

  void Render();
  void RenderFrame();
};