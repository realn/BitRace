#pragma once

#define DIRECTINPUT_VERSION 0x0800
#define GAME_NAME "BitRace"
#define GAME_FULLNAME "CodeRulers BitRace v1.0a"
#define GAME_DI_BUFSIZE 32
#define	GAME_BLEND_TEX_SIZE	64

#define GLEW_STATIC
#include <GL/glew.h>
//#include <GL/wglew.h>

#include "../Common/Vector.h"
#include "../Common/Log.h"

#include <SDL.h>
#include <glm/glm.hpp>

#include "../Common/Files.h"
#include "../Common/IniFiles.h"
#include "RaceTrack.h"
#include "GUI.h"
#include "Intro.h"
#include "HighScore.h"

class CGame {
private:
  SDL_Window*   m_pWindow;
  SDL_GLContext m_pGLContext;
  CGUI		m_cGUI;

  Uint8 m_KeyState[SDL_NUM_SCANCODES];
  Uint8 m_KeyStatePrev[SDL_NUM_SCANCODES];

  Uint32  m_MouseButtonState;
  Uint32  m_MouseButtonStatePrev;
  glm::ivec2  m_MousePos;
  glm::ivec2  m_MousePosPrev;

  bool			m_bShutdown;
  bool			m_bGamePause;
  bool			m_bTakeScreen;
  __int64			m_iLastTick;
  __int64			m_iFreq;
  float			m_fDT;
  float			m_fBlurTexAlpha;
  unsigned int	m_uBlurTexture;
  unsigned int	m_uBlurTexSize;
  std::string		m_strConfigFile;

  CRaceTrack		m_RaceTrack;
  CSpace			m_Space;
  CRacer			m_Racer;
  CGUIMenuManager	m_MenuMng;
  CIntro			m_Intro;
  CHighScore		m_HS;

  enum MENU_ID {
    MENU_MAIN = 0,
    MENU_HIGH,
    MENU_OPTIONS
  };
  enum MENUITEM_ID {
    MI_NEWGAME = 0,
    MI_HIGH,
    MI_OPTIONS,
    MI_EXIT,
    MI_GOBACK,
    MI_RETURN,

    MI_RESOLUTION,
    MI_FULLSCREEN,
    MI_SMOOTHSHADE,
    MI_SMOOTHLINE,
    MI_FPSCOUNTER,
    MI_VSYNC,
    MI_OPWARNING,

    MI_HS1,
    MI_HS2,
    MI_HS3,
    MI_HS4,
    MI_HS5,
    MI_HS6,
    MI_HS7,
    MI_HS8,
    MI_HS9,
    MI_HS10,
    MI_HSRESET
  };
  enum GAME_STATE {
    GS_INTRO = 0,
    GS_MENU,
    GS_GAME,
    GS_HIGH
  };
  unsigned m_uGameState;

  struct	SCREENPARAMS {
    unsigned	uWidth;
    unsigned	uHeight;
    unsigned	uColorBits;
    unsigned	uRefreshRate;

    unsigned	uDevID;

    bool	bSmoothLines;
    bool	bSmoothShade;
    bool	bFullscreen;
    bool	bFPSCount;
    bool	bVSync;
    bool	bBlur;

    const glm::vec2 GetSize() const;
    const float GetAspectRatio() const;
  }	ScrParam;

  SDL_DisplayMode m_ModeOryginal;
  std::vector<SDL_DisplayMode> m_ModeList;

public:
  CGame();
  ~CGame();

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
  void UpdateMenu(const float timeDelta);
  void UpdateGame(const float timeDelta);
  void Render();
  void RenderMenu();
  void RenderGame();
  void TakeScreenshot();
  void ScanDispModes();
  void ChangeDispMode();

  bool  IsKeyboardKeyDown(const SDL_Scancode code) const;
  bool  IsKeyboardKeyPressed(const SDL_Scancode code) const;
  bool  IsMouseButtonDown(const Uint32 button) const;
  bool  IsMouseButtonPressed(const Uint32 button) const;
  const glm::ivec2& GetMousePos() const;
  const glm::ivec2  GetMousePosDelta() const;

  int MainLoop();

  static float	s_fMaxDT;

  void UpdateKeyboard();
  void UpdateMouse();
  void UpdateTimer();
  void UpdateHS();
};