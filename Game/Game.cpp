#include "stdafx.h"
#include "Game.h"
#include "Log.h"
#include "Input.h"

#include <SDL.h>

CGame::CGame() :
  m_pView(nullptr),
  m_pInput(nullptr),
  m_GUI(),
  m_MenuMng(&m_GUI),
  m_Intro(&m_GUI, glm::vec2(800.0f, 600.0f)),
  m_pHS(nullptr),
  m_Run(true),
  m_bGamePause(false),
  m_bTakeScreen(false),
  m_iLastTick(0),
  m_iFreq(0),
  m_fDT(0.0f),
  m_fBlurTexAlpha(0.3f),
  m_uGameState(GS_INTRO),
  m_uBlurTexture(0),
  m_uBlurTexSize(64)
{
  Log_Init("main.log", "BIT_RACE_LOG");
  SDL_Init(0);
}

CGame::~CGame() {
  Free();
  SDL_Quit();
  Log_Free();
}

int CGame::MainLoop() {
  SDL_Event event;

  
  this->UpdateTimer();
  while (m_Run) {

    if (SDL_PollEvent(&event)) {
      this->ProcessEvent(event);
    }

    this->Update();
    this->Render();
  }

  Free();
  return 0;
}

void CGame::ProcessEvent(const SDL_Event& event) {
  switch(event.type) {
  case SDL_QUIT: m_Run = false; break;
  default:
    m_pInput->ProcessEvent(event);
    break;
  }
}
