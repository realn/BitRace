#include "Input.h"

#include <SDL.h>

CInput::CInput() {
  SDL_InitSubSystem(SDL_INIT_EVENTS);

  memset(this->m_KeyState, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
  memset(this->m_KeyStatePrev, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);

  this->m_MouseButtonState = 0;
  this->m_MouseButtonStatePrev = 0;

  this->m_MousePos = glm::ivec2(0);
  this->m_MousePosDelta = glm::ivec2(0);

  SDL_SetRelativeMouseMode(SDL_TRUE);
}

CInput::~CInput() {
  SDL_SetRelativeMouseMode(SDL_FALSE);
  SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void CInput::Update(const float timeDelta) {
  this->UpdateKeyboard();
  this->UpdateMouse();
}

bool CInput::IsKeyboardKeyDown(const SDL_Scancode code) const {
  return this->m_KeyState[code] > 0;
}

bool CInput::IsKeyboardKeyPressed(const SDL_Scancode code) const {
  return this->m_KeyState[code] && this->m_KeyStatePrev[code] == 0;
}

bool CInput::IsMouseButtonDown(const Uint32 button) const {
  return (this->m_MouseButtonState & SDL_BUTTON(button)) > 0;
}

bool CInput::IsMouseButtonPressed(const Uint32 button) const {
  return
    (this->m_MouseButtonState & SDL_BUTTON(button)) &&
    !(this->m_MouseButtonStatePrev & SDL_BUTTON(button));
}

const glm::ivec2 & CInput::GetMousePos() const {
  return this->m_MousePos;
}

const glm::ivec2 CInput::GetMousePosDelta() const {
  return this->m_MousePosDelta;
}

void CInput::UpdateKeyboard() {

  const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

  memcpy(this->m_KeyStatePrev, this->m_KeyState, sizeof(Uint8) * SDL_NUM_SCANCODES);
  memcpy(this->m_KeyState, keyboardState, sizeof(Uint8) * SDL_NUM_SCANCODES);

  //this->m_cDIKey->Acquire();
  //this->m_cDIKey->GetDeviceState(256, (LPVOID)m_cKeyState);
}

void CInput::UpdateMouse() {
  Uint32 mouseState = SDL_GetMouseState(&m_MousePos.x, &m_MousePos.y);
  SDL_GetRelativeMouseState(&m_MousePosDelta.x, &m_MousePosDelta.y);

  this->m_MouseButtonStatePrev = this->m_MouseButtonState;
  this->m_MouseButtonState = mouseState;
}
