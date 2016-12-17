#pragma once

#include <SDL_types.h>
#include <SDL_events.h>
#include <glm/glm.hpp>
#include <string>

class CInput {
private:
  Uint8 m_KeyState[SDL_NUM_SCANCODES];
  Uint8 m_KeyStatePrev[SDL_NUM_SCANCODES];

  Uint32  m_MouseButtonState;
  Uint32  m_MouseButtonStatePrev;
  glm::ivec2  m_MousePos;
  glm::ivec2  m_MousePosDelta;

  std::string m_Text;
  std::string m_Composition;
  Uint32  m_Cursor;
  Uint32  m_SelectionLength;

public:
  CInput();
  ~CInput();

  void Update(const float timeDelta);
  void ProcessEvent(const SDL_Event& event);

  void StartTextInput();
  void StartTextInput(const glm::vec2& pos, const glm::vec2& size);
  void StopTextInput();

  const std::string& GetText() const;
  const std::string& GetTextComposition() const;
  const Uint32  GetTextCursor() const;
  const Uint32  GetTextSelectionLength() const;

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