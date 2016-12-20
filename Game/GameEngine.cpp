#include "stdafx.h"
#include "Game.h"
#include "GameView.h"
#include "IniFiles.h"
#include "helper.h"
#include "Input.h"

#include <SDL_timer.h>
#include <GL/wglew.h>

float CGame::s_fMaxDT = 0.02f;

void CGame::Update() {
  UpdateTimer();

  float timeDelta = this->m_fDT;
  while(timeDelta > this->s_fMaxDT) {
    this->m_pInput->Update(this->s_fMaxDT);
    this->UpdateLogic(this->s_fMaxDT);
    timeDelta -= this->s_fMaxDT;
  }
  this->m_pInput->Update(timeDelta);
  this->UpdateLogic(timeDelta);
}

void CGame::UpdateLogic(const float timeDelta) {
  switch(m_uGameState) {
  case GS_INTRO:
    m_Intro.Update(timeDelta);
    if(m_Intro.IsIntroEnded() || 
       this->m_pInput->IsKeyboardKeyDown(SDL_SCANCODE_ESCAPE)) {
      m_Intro.Free();
      m_uGameState = GS_MENU;
    }
    break;

  case GS_MENU:
    this->UpdateMenu(timeDelta);
    break;

  case GS_GAME:
    this->UpdateGame(timeDelta);
    break;

  case GS_HIGH:
    m_pHS->Update(this->m_pInput, timeDelta);
    if(m_pHS->IsEnded()) {
      m_pHS->Save("score.hsf");
      UpdateHS();
      m_MenuMng.ForceSwitchToMenu(MENU_HIGH);
      m_uGameState = GS_MENU;
    }
    break;
  };
  if(this->m_pInput->IsKeyboardKeyPressed(SDL_SCANCODE_F11)) {
    m_pView->Minimize();
    SDL_WaitEvent(NULL);
    m_pView->Restore();
  }
  if(this->m_pInput->IsKeyboardKeyPressed(SDL_SCANCODE_F12)) {
    this->m_bTakeScreen = true;
  }
}


void CGame::UpdateTimer() {
  if(m_iFreq != 0) {
    Uint64 thisTick = SDL_GetPerformanceCounter();
    this->m_fDT = float(double(thisTick - this->m_iLastTick) / double(m_iFreq));
    this->m_iLastTick = thisTick;
  }
  if(m_fDT == 0.0f)
    m_fDT = 0.030f;
}

void CGame::UpdateGame(const float timeDelta) {
  if(m_RaceTrack.IsGameRuning()) {
    m_RaceTrack.PlayerModRotation(-float(this->m_pInput->GetMousePosDelta().x));
    if(this->m_pInput->IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
      this->m_RaceTrack.PlayerFireWeapon();
    }
  }
  if(m_RaceTrack.IsGameOver()) {
    m_MenuMng.ForceSwitchToMenu(MENU_MAIN);
    m_MenuMng.GetMenu(MENU_MAIN)->GetMenuItem(MI_RETURN)->SetEnable(false);
    m_pHS->StartScoreAnim(m_RaceTrack.GetPoints());
    m_uGameState = GS_HIGH;
    return;
  }

  if(this->m_pInput->IsKeyboardKeyPressed(SDL_SCANCODE_ESCAPE)) {
    m_MenuMng.ForceSwitchToMenu(MENU_MAIN);
    m_MenuMng.GetMenu(MENU_MAIN)->GetMenuItem(MI_RETURN)->SetEnable(true);
    m_uGameState = GS_MENU;
    return;
  }

  m_RaceTrack.Update(timeDelta);
}

void CGame::UpdateMenu(const float timeDelta) {
  Uint32 id = 0;
  CIniFile ini;

  if(!m_MenuMng.Update(this->m_pInput, timeDelta)) {
    return;
  }
  CGUIMenu* Menu = m_MenuMng.GetCurrentMenu();
  CGUIMenuItem* Item = Menu->GetMenuItem(Menu->GetClickedID());

  switch(Menu->GetClickedID()) {
  case MI_RETURN:
    this->m_uGameState = GS_GAME;
    break;

  case MI_NEWGAME:
    this->m_RaceTrack.ResetGame();
    this->m_bGamePause = true;
    this->m_uGameState = GS_GAME;
    break;

  case MI_EXIT:
    this->m_Run = false;
    break;

  case MI_HIGH:
  case MI_OPTIONS:
  case MI_GOBACK:
    m_MenuMng.SwitchToMenu(Item->GetUserDefID());
    break;

  case MI_RESOLUTION:
    id = Item->GetUserDefID() + 1;
    SDL_DisplayMode dispMode;
    if(!m_pView->GetDispMode(id, dispMode)) {
      id = 0;
      m_pView->GetDispMode(id, dispMode);
    }
    m_pView->GetParams().uDevID = id;
    if(glm::vec2(dispMode.w, dispMode.h) == m_pView->GetSize())
      Menu->GetMenuItem(MI_OPWARNING)->SetEnable(false);
    else
      Menu->GetMenuItem(MI_OPWARNING)->SetEnable(true);

    ini.Load(m_strConfigFile);
    ini.Write("GRAPHIC", "uWidth", dispMode.w);
    ini.Write("GRAPHIC", "uHeight", dispMode.h);
    ini.Write("GRAPHIC", "uRefreshRate", dispMode.refresh_rate);
    ini.Save(m_strConfigFile);

    Item->SetName(helper::format("Resolution: %u X %u", dispMode.w, dispMode.h));
    Item->SetUserDefID(id);
    break;

  case MI_SMOOTHSHADE:
    m_pView->GetParams().bSmoothShade = !m_pView->GetParams().bSmoothShade;
    if(m_pView->GetParams().bSmoothShade) {
      glShadeModel(GL_SMOOTH);
      Item->SetName("Smooth Shading: Enabled");
    }
    else {
      glShadeModel(GL_FLAT);
      Item->SetName("Smooth Shading: Disabled");
    }
    ini.Load(m_strConfigFile);
    ini.Write("GRAPHIC", "bSmoothShade", m_pView->GetParams().bSmoothShade);
    ini.Save(m_strConfigFile);
    break;

  case MI_SMOOTHLINE:
    m_pView->GetParams().bSmoothLines = !m_pView->GetParams().bSmoothLines;
    if(m_pView->GetParams().bSmoothLines) {
      glEnable(GL_LINE_SMOOTH);
      Item->SetName("Smooth Lines: Enabled");
    }
    else {
      glDisable(GL_LINE_SMOOTH);
      Item->SetName("Smooth Lines: Disabled");
    }
    ini.Load(m_strConfigFile);
    ini.Write("GRAPHIC", "bSmoothLines", m_pView->GetParams().bSmoothLines);
    ini.Save(m_strConfigFile);
    break;

  case MI_FULLSCREEN:
    id = Uint32((Item->GetUserDefID()) ? false : true);
    if(id)
      Item->SetName("Fullscreen: Enabled");
    else Item->SetName("Fullscreen: Disabled");

    Item->SetUserDefID(id);
    if((id ? true : false) == m_pView->GetParams().bFullscreen)
      Menu->GetMenuItem(MI_OPWARNING)->SetEnable(false);
    else Menu->GetMenuItem(MI_OPWARNING)->SetEnable(true);
    ini.Load(m_strConfigFile);
    ini.Write("GRAPHIC", "bFullscreen", bool((id) ? true : false));
    ini.Save(m_strConfigFile);
    break;

  case MI_FPSCOUNTER:
    m_pView->GetParams().bFPSCount = !m_pView->GetParams().bFPSCount;
    Item->SetName((m_pView->GetParams().bFPSCount) ? "FPS Counter: Enabled" : "FPS Counter: Disabled");
    ini.Load(m_strConfigFile);
    ini.Write("GRAPHIC", "bFPSCount", m_pView->GetParams().bFPSCount);
    ini.Save(m_strConfigFile);
    break;

  case MI_VSYNC:
    m_pView->GetParams().bVSync = !m_pView->GetParams().bVSync;
    if(m_pView->GetParams().bVSync) {
      wglSwapIntervalEXT(1);
      Item->SetName("VSync: Enabled");
    }
    else {
      wglSwapIntervalEXT(0);
      Item->SetName("VSync: Disabled");
    }
    ini.Load(m_strConfigFile);
    ini.Write("GRAPHIC", "bVSync", m_pView->GetParams().bVSync);
    ini.Save(m_strConfigFile);
    break;

  case MI_HSRESET:
    m_pHS->ResetAllScores();
    UpdateHS();
    m_pHS->Save("score.hsf");
    break;
  };
}

void CGame::UpdateHS() {
  CGUIMenu* Menu = m_MenuMng.GetMenu(MENU_HIGH);
  std::string strName;
  Uint32 uScore, i;
  std::string buffer;
  for(i = 0; i < 10; i++) {
    CGUIMenuItem* HSMI = Menu->GetMenuItem(MI_HS1 + i);
    if(HSMI == NULL)
      continue;
    strName = m_pHS->GetName(i);
    uScore = m_pHS->GetScore(i);
    if(strName.empty())
      buffer = helper::format("%u. --EMPTY SCORE--", i + 1);
    else
      buffer = helper::format("%u. %s - %u", i + 1, strName.c_str(), uScore);

    HSMI->SetName(buffer);
  }
}
