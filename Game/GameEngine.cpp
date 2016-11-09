#include "Game.h"

float CGame::s_fMaxDT = 0.02f;

#define KEYDOWN( A ) ( m_cKeyState[A] & 0x80 )
void CGame::Engine() {
  UpdateTimer();
  UpdateKeyboard();
  UpdateMouse();

  if (m_fDT > s_fMaxDT) {
    UINT i, It = UINT(m_fDT / s_fMaxDT);
    float fTempDT = m_fDT - float(It) * s_fMaxDT;
    m_fDT = s_fMaxDT;
    EngineApp();
    for (i = 1; i < It; ++i) {
      UpdateKeyboard();
      UpdateMouse();
      EngineApp();
    }
    m_fDT = fTempDT;
    UpdateKeyboard();
    UpdateMouse();
  }
  EngineApp();
}

void CGame::EngineApp() {
  static bool down2 = false;
  switch (m_uGameState) {
  case GS_INTRO:
    m_Intro.Engine(m_fDT);
    if (m_Intro.IsIntroEnded() || KEYDOWN(DIK_ESCAPE)) {
      m_Intro.Free();
      m_uGameState = GS_MENU;
    }
    break;

  case GS_MENU:
    EngineMenu();
    break;

  case GS_GAME:
    if (!m_MMag.GetCurrentMenu()->IsHiden()) {
      CGUIMenu* M = m_MMag.GetCurrentMenu();
      if (!M->IsHideing())
        M->Hide();
      m_MMag.Engine(this->m_cKeyState, &this->m_cMouseState, m_fDT);
    }
    else EngineGame();
    break;

  case GS_HIGH:
    m_HS.Engine(this->m_cKeyState, m_fDT);
    if (m_HS.IsEnded()) {
      m_HS.SaveScores("score.hsf");
      UpdateHS();
      m_MMag.ForceSwitchToMenu(MENU_HIGH);
      m_uGameState = GS_MENU;
    }
    break;
  };
  if (KEYDOWN(DIK_F11)) {
    ShowWindow(this->m_hWnd, SW_MINIMIZE);

    if (ScrParam.bFullscreen)
      ChangeDisplaySettings(NULL, 0);

    WaitMessage();

    if (ScrParam.bFullscreen)
      this->ChangeDispMode();
    m_cKeyState[DIK_F11] = 0;
  }
  if (KEYDOWN(DIK_F12)) {
    if (!down2) {
      this->m_bTakeScreen = true;
      down2 = true;
    }
  }
  else down2 = false;
}

void CGame::UpdateKeyboard() {
  this->m_cDIKey->Acquire();
  this->m_cDIKey->GetDeviceState(256, (LPVOID)m_cKeyState);
}

void CGame::UpdateMouse() {
  this->m_cDIMouse->Acquire();
  this->m_cDIMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (LPVOID)&m_cMouseState);
}

void CGame::UpdateTimer() {
  __int64 ThisTick = 0;
  if (m_iFreq != 0 && QueryPerformanceCounter((LARGE_INTEGER*)&ThisTick)) {
    this->m_fDT = float(double(ThisTick - this->m_iLastTick) / double(m_iFreq));
    this->m_iLastTick = ThisTick;
  }
  if (m_fDT == 0.0f) m_fDT = 0.030f;
}

void CGame::EngineGame() {
  static bool down = false;
  if (m_RaceTrack.IsGameRuning()) {
    this->m_Racer.ModRotation(float(m_cMouseState.lX));
    if (this->m_cMouseState.rgbButtons[0] & 0x80) {
      if (!down) {
        this->m_RaceTrack.FireWeapon();
        down = true;
      }
    }
    else down = false;
  }
  if (m_RaceTrack.IsGameOver()) {
    m_MMag.ForceSwitchToMenu(MENU_MAIN);
    m_MMag.GetMenu(MENU_MAIN)->GetMenuItem(MI_RETURN)->SetEnable(false);
    m_HS.SetTempScore(m_RaceTrack.GetPoints());
    m_uGameState = GS_HIGH;
    return;
  }

  if (KEYDOWN(DIK_ESCAPE)) {
    m_MMag.ForceSwitchToMenu(MENU_MAIN);
    m_MMag.GetMenu(MENU_MAIN)->GetMenuItem(MI_RETURN)->SetEnable(true);
    m_uGameState = GS_MENU;
    return;
  }

  m_RaceTrack.Engine(m_fDT);
}

void CGame::EngineMenu() {
  UINT id = 0;
  CIniFile ini;
  char szBuffer[1000];
  static bool down = false;
  if (m_MMag.Engine(this->m_cKeyState, &this->m_cMouseState, m_fDT)) {
    if (down)
      return;
    CGUIMenu* Menu = m_MMag.GetCurrentMenu();
    CGUIMenuItem* Item = Menu->GetMenuItem(Menu->GetClickedID());
    switch (Menu->GetClickedID()) {
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
      m_MMag.SwitchToMenu(Item->GetUserDefID());
      break;

    case MI_RESOLUTION:
      id = Item->GetUserDefID();
      if (++id >= UINT(m_aMode.size()))
        id = 0;
      ScrParam.uDevID = id;
      if (m_aMode[id].uWidth == ScrParam.uWidth && m_aMode[id].uHeight == ScrParam.uHeight)
        Menu->GetMenuItem(MI_OPWARNING)->SetEnable(false);
      else Menu->GetMenuItem(MI_OPWARNING)->SetEnable(true);
      ini.Open(m_strConfigFile);
      ini.WriteInt("GRAPHIC", "uWidth", int(m_aMode[id].uWidth));
      ini.WriteInt("GRAPHIC", "uHeight", int(m_aMode[id].uHeight));
      ini.WriteInt("GRAPHIC", "uRefreshRate", int(m_aMode[id].uRefreshRate));
      ini.Close();

      sprintf_s(szBuffer, 1000, "Resolution: %u X %u", m_aMode[id].uWidth, m_aMode[id].uHeight);

      Item->SetName(szBuffer);
      Item->SetUserDefID(id);
      break;

    case MI_SMOOTHSHADE:
      ScrParam.bSmoothShade = !ScrParam.bSmoothShade;
      if (ScrParam.bSmoothShade) {
        glShadeModel(GL_SMOOTH);
        Item->SetName("Smooth Shading: Enabled");
      }
      else {
        glShadeModel(GL_FLAT);
        Item->SetName("Smooth Shading: Disabled");
      }
      ini.Open(m_strConfigFile);
      ini.WriteBool("GRAPHIC", "bSmoothShade", ScrParam.bSmoothShade);
      ini.Close();
      break;

    case MI_SMOOTHLINE:
      ScrParam.bSmoothLines = !ScrParam.bSmoothLines;
      if (ScrParam.bSmoothLines) {
        glEnable(GL_LINE_SMOOTH);
        Item->SetName("Smooth Lines: Enabled");
      }
      else {
        glDisable(GL_LINE_SMOOTH);
        Item->SetName("Smooth Lines: Disabled");
      }
      ini.Open(m_strConfigFile);
      ini.WriteBool("GRAPHIC", "bSmoothLines", ScrParam.bSmoothLines);
      ini.Close();
      break;

    case MI_FULLSCREEN:
      id = UINT((Item->GetUserDefID()) ? false : true);
      if (id)
        Item->SetName("Fullscreen: Enabled");
      else Item->SetName("Fullscreen: Disabled");

      Item->SetUserDefID(id);
      if ((id ? true : false) == ScrParam.bFullscreen)
        Menu->GetMenuItem(MI_OPWARNING)->SetEnable(false);
      else Menu->GetMenuItem(MI_OPWARNING)->SetEnable(true);
      ini.Open(m_strConfigFile);
      ini.WriteBool("GRAPHIC", "bFullscreen", bool((id) ? true : false));
      ini.Close();
      break;

    case MI_FPSCOUNTER:
      ScrParam.bFPSCount = !ScrParam.bFPSCount;
      Item->SetName((ScrParam.bFPSCount) ? "FPS Counter: Enabled" : "FPS Counter: Disabled");
      ini.Open(m_strConfigFile);
      ini.WriteBool("GRAPHIC", "bFPSCount", ScrParam.bFPSCount);
      ini.Close();
      break;

    case MI_VSYNC:
      ScrParam.bVSync = !ScrParam.bVSync;
      if (ScrParam.bVSync) {
        wglSwapIntervalEXT(1);
        Item->SetName("VSync: Enabled");
      }
      else {
        wglSwapIntervalEXT(0);
        Item->SetName("VSync: Disabled");
      }
      ini.Open(m_strConfigFile);
      ini.WriteBool("GRAPHIC", "bVSync", ScrParam.bVSync);
      ini.Close();
      break;

    case MI_HSRESET:
      m_HS.ResetAllScores();
      UpdateHS();
      m_HS.SaveScores("score.hsf");
      break;
    };
    down = true;
  }
  else down = false;
}

void CGame::UpdateHS() {
  CGUIMenu* Menu = m_MMag.GetMenu(MENU_HIGH);
  std::string strName;
  UINT uScore, i;
  char szBuffer[1000];
  for (i = 0; i < 10; i++) {
    CGUIMenuItem* HSMI = Menu->GetMenuItem(MI_HS1 + i);
    if (HSMI == NULL)
      continue;
    strName = m_HS.GetName(i);
    uScore = m_HS.GetScore(i);
    if (strName.empty())
      sprintf_s(szBuffer, 1000, "%u. --EMPTY SCORE--", i + 1);
    else sprintf_s(szBuffer, 1000, "%u. %s - %u", i + 1, strName.c_str(), uScore);

    HSMI->SetName(szBuffer);
  }
}