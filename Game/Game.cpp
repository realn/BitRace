#include "Game.h"

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
