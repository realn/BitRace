#pragma once

#define GAME_NAME "BitRace"
#define GAME_FULLNAME "CodeRulers BitRace v1.0a"
#define	GAME_BLEND_TEX_SIZE	64

#include <SDL_types.h>
#include <SDL_events.h>
#include <glm/glm.hpp>

#include "Level.h"
#include "GUI.h"
#include "GUIMenu.h"
#include "Intro.h"
#include "HighScore.h"

class CGameView;
class CInput;

class CGame {
private:
	CGameView*  m_pView;
	CInput*     m_pInput;

	CGUI		        m_GUI;
	CGUIMenuManager	m_MenuMng;
	CLevel		    m_RaceTrack;
	CIntro			  m_Intro;
	CHighScore*	m_pHS;

	bool			m_Run;
	bool			m_bGamePause;
	bool			m_bTakeScreen;
	Uint64		m_iLastTick;
	Uint64		m_iFreq;
	float			m_fDT;
	float			m_fBlurTexAlpha;
	Uint32	  m_uBlurTexture;
	Uint32	  m_uBlurTexSize;
	std::string		m_strConfigFile;

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


public:
	CGame();
	~CGame();

	bool Init(std::string strCmdLine);
	bool InitOpenGL();
	bool InitGame();

	void Free();
	void FreeOpenGL();
	void FreeGame();

	void Update();
	void UpdateLogic(const float timeDelta);
	void UpdateMenu(const float timeDelta);
	void UpdateGame(const float timeDelta);

	void Render();
	void RenderGame();
	void RenderGUI();
	void RenderMenu();
	void TakeScreenshot();

	int MainLoop();

	static float	s_fMaxDT;

	void UpdateTimer();
	void UpdateHS();

private:
	void ProcessEvent(const SDL_Event& event);
};