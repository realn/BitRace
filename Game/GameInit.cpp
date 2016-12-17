#include "Game.h"
#include "GameView.h"
#include "Input.h"
#include "Log.h"
#include "IniFiles.h"

#include <SDL.h>
#include <GL/wglew.h>

bool CGame::Init(std::string strCmdLine) {
	m_strConfigFile = "config.ini";

	CIniFile ini(m_strConfigFile);

	m_uBlurTexSize = ini.Read("GRAPHIC", "uBlurTexSize", 64);
	m_fBlurTexAlpha = ini.Read("GRAPHIC", "fBlurTexAlpha", 0.3f);

	if(!m_pView)
		m_pView = new CGameView(ini);

	if (!m_pView->Create(GAME_FULLNAME)) {
		Log_Error("Can't initialize game view.");
		Free();
		return false;
	}

  if(!m_pInput)
    m_pInput = new CInput();

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

bool CGame::InitOpenGL() {
	if (glewInit() != GLEW_OK) {
		return false;
	}

	m_GUI.Init();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClearStencil(0);

	if (m_pView->GetParams().bSmoothShade)
		glShadeModel(GL_SMOOTH);
	else glShadeModel(GL_FLAT);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	if (m_pView->GetParams().bSmoothLines)
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
	//gluPerspective(50.0, double(ScrParam.uWidth) / double(ScrParam.uHeight), 1.0, 50000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_GUI.Begin(glm::vec2(640.0f, 480.0f));
	m_GUI.Print(glm::vec2(100.0f, 200.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "Please wait, loading game...");
	m_GUI.End();

	m_pView->Swap();

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

	m_pView->ScanDispModes();

	this->m_RaceTrack.Init(&m_GUI, glm::vec2(640.0f, 480.0f));
	this->m_HS.LoadScores("score.hsf");

	if (WGLEW_EXT_swap_control) {
		if (m_pView->GetParams().bVSync)
			wglSwapIntervalEXT(1);
		else wglSwapIntervalEXT(0);
	}

	m_MenuMng.SetSize(m_pView->GetSize());

	CGUIMenu* Menu = this->m_MenuMng.AddMenu(MENU_MAIN, "BitRace");
	Menu->AddMenuItem(MI_RETURN, "Return to Game")->SetEnable(false);
	Menu->AddMenuItem(MI_NEWGAME, "New Game");
	Menu->AddMenuItem(MI_HIGH, "High Scores", MENU_HIGH);
	Menu->AddMenuItem(MI_OPTIONS, "Options", MENU_OPTIONS);
	Menu->AddMenuItem(MI_EXIT, "Exit Game");
	Menu = this->m_MenuMng.AddMenu(MENU_OPTIONS, "Options");
	sprintf_s(szBuffer, 1000, "Resolution: %d X %d", m_pView->GetParams().uWidth, m_pView->GetParams().uHeight);
	Menu->AddMenuItem(MI_RESOLUTION, szBuffer, m_pView->GetParams().uDevID);
	Menu->AddMenuItem(MI_FULLSCREEN, (m_pView->GetParams().bFullscreen) ? "Fullscreen: Enabled" : "FullScreen: Disabled", Uint32(m_pView->GetParams().bFullscreen));
	Menu->AddMenuItem(MI_SMOOTHSHADE, (m_pView->GetParams().bSmoothShade) ? "Smooth Shading: Enabled" : "Smooth Shading: Disabled");
	Menu->AddMenuItem(MI_SMOOTHLINE, (m_pView->GetParams().bSmoothLines) ? "Smooth Lines: Enabled" : "Smooth Lines: Disabled");
	Menu->AddMenuItem(MI_FPSCOUNTER, (m_pView->GetParams().bFPSCount) ? "FPS Counter: Enabled" : "FPS Counter: Disabled");
	Menu->AddMenuItem(MI_VSYNC, (m_pView->GetParams().bVSync) ? "VSync: Enabled" : "VSync: Disabled");
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

	m_Intro.Init("logos.fgx", m_pView->GetSize());

	return true;
}
