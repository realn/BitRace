#include "stdafx.h"
#include "Input.h"

#include <SDL.h>

CInput::CInput() :
  m_MouseButtonState(0),
  m_MouseButtonStatePrev(0),
  m_MousePos(0),
  m_MousePosDelta(0),
  m_Cursor(0),
  m_SelectionLength(0)
{
  SDL_InitSubSystem(SDL_INIT_EVENTS);

  memset(this->m_KeyState, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
  memset(this->m_KeyStatePrev, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);

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

void CInput::ProcessEvent(const SDL_Event & event) {
  switch(event.type) {
  case SDL_TEXTEDITING:
    m_Composition = std::string(event.edit.text);
    m_Cursor = (Uint32)event.edit.start;
    m_SelectionLength = (Uint32)event.edit.length;
    break;

  case SDL_TEXTINPUT:
    m_Text += std::string(event.text.text);
    break;

  default:
    break;
  }
}

void CInput::StartTextInput() {
  SDL_StartTextInput();
}

void CInput::StartTextInput(const glm::vec2 & pos, const glm::vec2 & size) {
  SDL_Rect rect;
  rect.x = (int)pos.x;
  rect.y = (int)pos.y;
  rect.w = (int)size.x;
  rect.h = (int)size.y;
  SDL_SetTextInputRect(&rect);
  SDL_StartTextInput();
}

void CInput::StopTextInput() {
  SDL_StopTextInput();
}

const std::string & CInput::GetText() const {
  return m_Text;
}

const std::string & CInput::GetTextComposition() const {
  return m_Composition;
}

const Uint32 CInput::GetTextCursor() const {
  return m_Cursor;
}

const Uint32 CInput::GetTextSelectionLength() const {
  return m_SelectionLength;
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
