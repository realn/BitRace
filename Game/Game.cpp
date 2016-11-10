#include "Game.h"

bool CGame::IsKeyboardKeyDown(const SDL_Scancode code) const {
  return this->m_KeyState[code] > 0;
}

bool CGame::IsKeyboardKeyPressed(const SDL_Scancode code) const {
  return this->m_KeyState[code] > 0 && this->m_KeyStatePrev[code] == 0;
}
