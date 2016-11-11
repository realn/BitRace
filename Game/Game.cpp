#include "Game.h"

#pragma comment(lib, "Common.lib")
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32s.lib")

const glm::vec2 CGame::SCREENPARAMS::GetSize() const {
  return glm::vec2((float)uWidth, (float)uHeight);
}

const float CGame::SCREENPARAMS::GetAspectRatio() const {
  return (float)uWidth / (float)uHeight;
}

CGame::CGame() :
  m_pWindow(NULL),
  m_pGLContext(NULL),
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
  SDL_Init(0);
}

CGame::~CGame() {
  Free();
  SDL_Quit();
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

bool CGame::IsKeyboardKeyDown(const SDL_Scancode code) const {
  return this->m_KeyState[code] > 0;
}

bool CGame::IsKeyboardKeyPressed(const SDL_Scancode code) const {
  return this->m_KeyState[code]  && this->m_KeyStatePrev[code] == 0;
}

bool CGame::IsMouseButtonDown(const Uint32 button) const {
  return (this->m_MouseButtonState & SDL_BUTTON(button)) > 0;
}

bool CGame::IsMouseButtonPressed(const Uint32 button) const {
  return 
    (this->m_MouseButtonState & SDL_BUTTON(button)) &&
    !(this->m_MouseButtonStatePrev & SDL_BUTTON(button));
}

const glm::ivec2 & CGame::GetMousePos() const {
  return this->m_MousePos;
}

const glm::ivec2 CGame::GetMousePosDelta() const {
  return this->m_MousePos - this->m_MousePosPrev;
}
