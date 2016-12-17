#pragma once

#include <SDL_types.h>
#include <SDL_events.h>
#include <glm/glm.hpp>

class CInput {
private:
  Uint8 m_KeyState[SDL_NUM_SCANCODES];
  Uint8 m_KeyStatePrev[SDL_NUM_SCANCODES];

  Uint32  m_MouseButtonState;
  Uint32  m_MouseButtonStatePrev;
  glm::ivec2  m_MousePos;
  glm::ivec2  m_MousePosDelta;

public:
  CInput();
  ~CInput();

  void Update(const float timeDelta);

  bool  IsKeyboardKeyDown(const SDL_Scancode code) const;
  bool  IsKeyboardKeyPressed(const SDL_Scancode code) const;
  bool  IsMouseButtonDown(const Uint32 button) const;
  bool  IsMouseButtonPressed(const Uint32 button) const;
  const glm::ivec2& GetMousePos() const;
  const glm::ivec2  GetMousePosDelta() const;

private:
  void UpdateKeyboard();
  void UpdateMouse();
};