#include "stdafx.h"
#include "Game.h"
#include "IniFiles.h"
#include "GLDefines.h"

float CGame::s_fMaxDT = 0.02f;

#define KEYDOWN( A ) ( m_cKeyState[A] & 0x80 )
void CGame::Update() {
  UpdateTimer();

  float timeDelta = this->m_fDT;
  while(timeDelta > this->s_fMaxDT) {
    this->UpdateKeyboard();
    this->UpdateMouse();

    this->UpdateLogic(this->s_fMaxDT);
    timeDelta -= this->s_fMaxDT;
  }
  this->UpdateKeyboard();
  this->UpdateMouse();
  this->UpdateLogic(timeDelta);
}

void CGame::UpdateLogic(const float timeDelta) {
  static bool down2 = false;
  switch(m_uGameState) {
  case GS_INTRO:
    m_Intro.Engine(timeDelta);
    if(m_Intro.IsIntroEnded() || this->IsKeyboardKeyDown(SDL_SCANCODE_ESCAPE)) {
      m_Intro.Free();
      m_uGameState = GS_MENU;
    }
    break;

  case GS_MENU:
    this->UpdateMenu(timeDelta);
    break;

  case GS_GAME:
    if(!m_MenuMng.GetCurrentMenu()->IsHidden()) {
      CGUIMenu* M = m_MenuMng.GetCurrentMenu();
      if(!M->IsHiding())
        M->Hide();
      m_MenuMng.Update(this, timeDelta);
    }
    else
      this->UpdateGame(timeDelta);
    break;

  case GS_HIGH:
    m_HS.Update(this, timeDelta);
    if(m_HS.IsEnded()) {
      m_HS.SaveScores(L"score.hsf");
      UpdateHS();
      m_MenuMng.ForceSwitchToMenu(MENU_HIGH);
      m_uGameState = GS_MENU;
    }
    break;
  };
  if(this->IsKeyboardKeyPressed(SDL_SCANCODE_F11)) {
    SDL_MinimizeWindow(this->m_pWindow);

    if(mConfig.Screen.Fullscreen)
      SDL_SetWindowDisplayMode(this->m_pWindow, &this->m_ModeOryginal);

    SDL_WaitEvent(NULL);

    if(mConfig.Screen.Fullscreen)
      this->ChangeDispMode();
  }
  if(this->IsKeyboardKeyPressed(SDL_SCANCODE_F12)) {
    this->m_bTakeScreen = true;
  }
}

void CGame::UpdateKeyboard() {

  const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

  memcpy(this->m_KeyStatePrev, this->m_KeyState, sizeof(Uint8) * SDL_NUM_SCANCODES);
  memcpy(this->m_KeyState, keyboardState, sizeof(Uint8) * SDL_NUM_SCANCODES);

  //this->m_cDIKey->Acquire();
  //this->m_cDIKey->GetDeviceState(256, (LPVOID)m_cKeyState);
}

void CGame::UpdateMouse() {

  glm::ivec2 mousePos;
  Uint32 mouseState = SDL_GetMouseState(&mousePos.x, &mousePos.y);

  this->m_MouseButtonStatePrev = this->m_MouseButtonState;
  this->m_MouseButtonState = mouseState;

  this->m_MousePosPrev = this->m_MousePos;
  this->m_MousePos = mousePos;
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
  static bool down = false;
  if(m_RaceTrack.IsGameRuning()) {
    this->m_Racer.ModRotation(float(this->GetMousePosDelta().x));
    if(this->IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
      this->m_RaceTrack.FireWeapon();
    }
  }
  if(m_RaceTrack.IsGameOver()) {
    m_MenuMng.ForceSwitchToMenu(MENU_MAIN);
    m_MenuMng.GetMenu(MENU_MAIN)->GetMenuItem(MI_RETURN)->SetEnable(false);
    m_HS.SetTempScore(m_RaceTrack.GetPoints());
    m_uGameState = GS_HIGH;
    return;
  }

  if(this->IsKeyboardKeyPressed(SDL_SCANCODE_ESCAPE)) {
    m_MenuMng.ForceSwitchToMenu(MENU_MAIN);
    m_MenuMng.GetMenu(MENU_MAIN)->GetMenuItem(MI_RETURN)->SetEnable(true);
    m_uGameState = GS_MENU;
    return;
  }

  m_RaceTrack.Engine(timeDelta);
}

void CGame::UpdateMenu(const float timeDelta) {
  Uint32 id = 0;
  CIniFile ini;
  char szBuffer[1000];
  static bool down = false;
  if(m_MenuMng.Update(this, timeDelta)) {
    if(down)
      return;
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
      this->m_bShutdown = true;
      break;

    case MI_HIGH:
    case MI_OPTIONS:
    case MI_GOBACK:
      m_MenuMng.SwitchToMenu(Item->GetUserDefID());
      break;

    case MI_RESOLUTION:
      {
        id = Item->GetUserDefID();
        if(++id >= Uint32(this->m_ModeList.size()))
          id = 0;

        mConfig.Screen.DevId = id;
        SDL_DisplayMode& disp = m_ModeList[id];

        if(disp.w == mConfig.Screen.Width && disp.h == mConfig.Screen.Height)
          Menu->GetMenuItem(MI_OPWARNING)->SetEnable(false);
        else
          Menu->GetMenuItem(MI_OPWARNING)->SetEnable(true);

        mConfig.Screen.Width = disp.w;
        mConfig.Screen.Height = disp.h;
        mConfig.Screen.RefreshRate = disp.refresh_rate;
        SaveConfig();

        sprintf_s(szBuffer, 1000, "Resolution: %u X %u", m_ModeList[id].w, m_ModeList[id].h);

        Item->SetName(szBuffer);
        Item->SetUserDefID(id);
      }
      break;

    case MI_SMOOTHSHADE:
      mConfig.Render.SmoothShade = !mConfig.Render.SmoothShade;
      if(mConfig.Render.SmoothShade) {
        glShadeModel(GL_SMOOTH);
        Item->SetName("Smooth Shading: Enabled");
      }
      else {
        glShadeModel(GL_FLAT);
        Item->SetName("Smooth Shading: Disabled");
      }
      SaveConfig();
      break;

    case MI_SMOOTHLINE:
      mConfig.Render.SmoothLines = !mConfig.Render.SmoothLines;
      if(mConfig.Render.SmoothLines) {
        glEnable(GL_LINE_SMOOTH);
        Item->SetName("Smooth Lines: Enabled");
      }
      else {
        glDisable(GL_LINE_SMOOTH);
        Item->SetName("Smooth Lines: Disabled");
      }
      SaveConfig();
      break;

    case MI_FULLSCREEN:
      mConfig.Screen.Fullscreen = !mConfig.Screen.Fullscreen;
      if(mConfig.Screen.Fullscreen)
        Item->SetName("Fullscreen: Enabled");
      else 
        Item->SetName("Fullscreen: Disabled");
      Menu->GetMenuItem(MI_OPWARNING)->SetEnable(true);
      SaveConfig();
      break;

    case MI_FPSCOUNTER:
      mConfig.Diag.FPSCounter = !mConfig.Diag.FPSCounter;
      Item->SetName((mConfig.Diag.FPSCounter) ? "FPS Counter: Enabled" : "FPS Counter: Disabled");
      SaveConfig();
      break;

    case MI_VSYNC:
      mConfig.Screen.VSync = !mConfig.Screen.VSync;
      if(mConfig.Screen.VSync) {
        //wglSwapIntervalEXT(1);
        Item->SetName("VSync: Enabled");
      }
      else {
        //wglSwapIntervalEXT(0);
        Item->SetName("VSync: Disabled");
      }
      SaveConfig();
      break;

    case MI_HSRESET:
      m_HS.ResetAllScores();
      UpdateHS();
      m_HS.SaveScores(L"score.hsf");
      break;
    };
    down = true;
  }
  else down = false;
}

void CGame::UpdateHS() {
  CGUIMenu* Menu = m_MenuMng.GetMenu(MENU_HIGH);
  std::string strName;
  Uint32 uScore, i;
  char szBuffer[1000];
  for(i = 0; i < 10; i++) {
    CGUIMenuItem* HSMI = Menu->GetMenuItem(MI_HS1 + i);
    if(HSMI == NULL)
      continue;
    strName = m_HS.GetName(i);
    uScore = m_HS.GetScore(i);
    if(strName.empty())
      sprintf_s(szBuffer, 1000, "%u. --EMPTY SCORE--", i + 1);
    else sprintf_s(szBuffer, 1000, "%u. %s - %u", i + 1, strName.c_str(), uScore);

    HSMI->SetName(szBuffer);
  }
}

