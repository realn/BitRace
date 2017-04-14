#pragma once

#define GAME_NAME "BitRace"
#define GAME_FULLNAME "CodeRulers BitRace v1.0a"
#define GAME_DI_BUFSIZE 32
#define	GAME_BLEND_TEX_SIZE	64

#include <SDL.h>

#include <glm/glm.hpp>

#include <CBLog/Logger.h>
#include <CBIO/File.h>

#include "Config.h"
#include "PerfTimer.h"
#include "InputDevice.h"

#include "RaceTrack.h"
#include "GUI.h"
#include "Intro.h"
#include "Menu.h"
#include "Game.h"
#include "HighScore.h"
#include "UIFont.h"

class IFileSystem;
class ILogicProcess;
class IGraphicView;

class CEngine 
  : public IMenuEventObserver
{
private:
  std::wofstream mLogFile;
  cb::CLogger mLogger;

  IFileSystem* mpFileSystem;
  CConfig mConfig;
  CPerfTimer mTimer;
  CInputDeviceMap mIDevMap;

  ILogicProcess* mpProcess;
  IGraphicView* mpView;

  cb::string mConfigFilePath;

  SDL_Window*   m_pWindow;
  SDL_GLContext m_pGLContext;

  CUIFont mUIFont;
  CUIText mUIText;
  CGUI		        m_GUI;

  CIntroProcess mIntroProcess;
  CMenuProcess mMenuProcess;
  CGameProcess mGameProcess;
  CSpace			m_Space;
  CHighScore		mHS;

  CIntroView mIntroView;
  CMenuView mMenuView;
  CGameView mGameView;

  float mFrameTime;
  float mFrameStepTime;
  bool			m_bShutdown;
  bool			m_bGamePause;
  bool			m_bTakeScreen;
  unsigned int	m_uBlurTexture;

  glm::mat4 mProjMatrix;

  enum GAME_STATE {
    GS_INTRO = 0,
    GS_MENU,
    GS_GAME,
    GS_HIGH,
    GS_EXIT
  };
  unsigned m_uGameState;

  SDL_DisplayMode m_ModeOryginal;
  std::vector<SDL_DisplayMode> m_ModeList;

public:
  CEngine();
  ~CEngine();

  bool Init(std::string strCmdLine);
  bool InitWindow(std::string strTitle);
  bool InitRender();
  bool InitInput();
  bool InitOpenGL();
  bool InitGame();

  void Free();
  void FreeWindow();
  void FreeRender();
  void FreeInput();
  void FreeOpenGL();
  void FreeGame();

  void Update();
  void UpdateLogic(const float timeDelta);
  void MenuItemAction(CGUIMenuManager& menuMng, CGUIMenu& menu, CGUIMenuItem& item);
  void UpdateGame(const float timeDelta);

  void Render();
  void RenderGame();
  void RenderGUI();
  void RenderMenu();
  void TakeScreenshot();
  void ScanDispModes();
  void ChangeDispMode();

  int MainLoop();

  static float	s_fMaxDT;

  void UpdateHS();

private:
  void SaveConfig();
  void LoadConfig();

  const GAME_STATE GetNextState() const;
};