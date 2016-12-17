#include "Game.h"
#include "Log.h"

#include <SDL.h>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32s.lib")

CGame::CGame() :
  m_pView(nullptr),
  m_pInput(nullptr),
  m_GUI(),
  m_MenuMng(&m_GUI),
  m_Intro(&m_GUI, glm::vec2(800.0f, 600.0f)),
  m_bShutdown(false),
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

  while (!m_bShutdown) {

    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        this->m_bShutdown = true;
    }

    this->Update();
    this->Render();
  }

  Free();
  return 0;
}

