#include "Game.h"

CGame::CGame() :
  m_pWindow(NULL),
  m_pGLContext(NULL),
  m_cDInput(NULL),
  m_cDIMouse(NULL),
  m_bShutdown(false),
  m_bGamePause(false),
  m_bTakeScreen(false),
  m_iLastTick(0),
  m_iFreq(0),
  m_fDT(0.0f),
  m_fBlurTexAlpha(0.3f),
  m_uGameState(GS_INTRO),
  m_uBlurTexture(0),
  m_uBlurTexSize(64) {
  SDL_Init(0);
}

CGame::~CGame() {
  Free();
  SDL_Quit();
}

int CGame::MainLoop() {
  SDL_Event event;

  while (!m_bShutdown) {

    if(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT)
        this->m_bShutdown = true;
    }
    
    this->Update();
    this->Render();
  }

  Free();
  return 0;
}