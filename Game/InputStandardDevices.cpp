#include "stdafx.h"
#include "InputStandardDevices.h"
#include "InputDeviceEvents.h"

class CKeyboardInputDevice::CKeyState {
public:
  bool State;
  bool StatePrev;

  CKeyState();
  void Set(const bool value);
  const bool Pressed() const;
  const bool Changed() const;
  const bool IsDown() const;
  const bool IsUp() const;
};

class CMouseInputDevice::CButtonState {
public:
  bool State;
  bool StatePrev;
  CButtonState();
  void Set(const bool value);
  const bool Pressed() const;
  const bool Changed() const;
  const bool IsDown() const;
  const bool IsUp() const;
};

class CMouseInputDevice::CMouse {
public:
  ButtonMapT Buttons;
  glm::ivec2 Pos;
  glm::ivec2 PosPrev;

  CMouse();

  void Move(const glm::ivec2& pos);
  const glm::vec2 GetPos(const glm::uvec2& screenSize) const;
  const glm::vec2 GetPosPrev(const glm::uvec2& screenSize) const;
};


CKeyboardInputDevice::CKeyState::CKeyState()
  : State(false), StatePrev(false) {}

void CKeyboardInputDevice::CKeyState::Set(const bool value) {
  StatePrev = State;
  State = value;
}

const bool CKeyboardInputDevice::CKeyState::Pressed() const {
  return State && !StatePrev;
}

const bool CKeyboardInputDevice::CKeyState::Changed() const {
  return State != StatePrev;
}

const bool CKeyboardInputDevice::CKeyState::IsDown() const {
  return State;
}

const bool CKeyboardInputDevice::CKeyState::IsUp() const {
  return !State;
}

CKeyboardInputDevice::CKeyboardInputDevice()
  : mKeys(SDL_NUM_SCANCODES) {}

CKeyboardInputDevice::~CKeyboardInputDevice() {}

const bool CKeyboardInputDevice::ProcessEvent(const SDL_Event & event,
                                              IInputDeviceHandler& handler) {
  if(event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) {
    return false;
  }

  SDL_Scancode code = event.key.keysym.scancode;
  CKeyState& state = mKeys[code];
  state.Set(event.key.state == SDL_PRESSED);

  InputDeviceEventVecT events;
  if(state.Pressed()) {
    events.push_back(CInputDeviceEvent(GetType(), 0, code));
  }

  if(state.Changed()) {
    events.push_back(CInputDeviceEvent(GetType(), 0, code,
                                       state.IsDown(), state.IsUp()));
    events.push_back(CInputDeviceEvent(GetType(), 0, code,
                                       state.IsDown() ? 1.0f : 0.0f,
                                       state.IsUp() ? 1.0f : 0.0f));
  }

  handler.SendEvents(events);
  return true;
}

const InputDeviceType CKeyboardInputDevice::GetType() const {
  return InputDeviceType::Keyboard;
}

void CKeyboardInputDevice::Update(const float timeDelta) {}



CMouseInputDevice::CButtonState::CButtonState()
  : State(false), StatePrev(false) {}

void CMouseInputDevice::CButtonState::Set(const bool value) {
  StatePrev = State;
  State = value;
}

const bool CMouseInputDevice::CButtonState::Pressed() const {
  return State && !StatePrev;
}

const bool CMouseInputDevice::CButtonState::Changed() const {
  return State != StatePrev;
}

const bool CMouseInputDevice::CButtonState::IsDown() const {
  return State;
}

const bool CMouseInputDevice::CButtonState::IsUp() const {
  return !State;
}

CMouseInputDevice::CMouse::CMouse()
  : Pos(0)
  , PosPrev(0) {
  Buttons = {
    {InputMouseEventId::ButtonL, CButtonState()},
    {InputMouseEventId::ButtonM, CButtonState()},
    {InputMouseEventId::ButtonR, CButtonState()},
    {InputMouseEventId::ButtonX1, CButtonState()},
    {InputMouseEventId::ButtonX2, CButtonState()},
  };
}

void CMouseInputDevice::CMouse::Move(const glm::ivec2 & pos) {
  PosPrev = Pos;
  Pos = pos;
}

const glm::vec2 CMouseInputDevice::CMouse::GetPos(const glm::uvec2 & screenSize) const {
  return glm::vec2(Pos) / glm::vec2(screenSize);
}

const glm::vec2 CMouseInputDevice::CMouse::GetPosPrev(const glm::uvec2 & screenSize) const {
  return glm::vec2(PosPrev) / glm::vec2(screenSize);
}

CMouseInputDevice::CMouseInputDevice(const glm::uvec2& screenSize)
  : mScreenSize(screenSize) {}

CMouseInputDevice::~CMouseInputDevice() {}

const bool CMouseInputDevice::ProcessEvent(const SDL_Event & event,
                                           IInputDeviceHandler& handler) {
  if(!cb::any(event.type, {SDL_MOUSEMOTION, SDL_MOUSEBUTTONUP, SDL_MOUSEBUTTONDOWN, SDL_MOUSEWHEEL})) {
    return false;
  }

  if(event.type == SDL_MOUSEMOTION) {
    Uint32 devId = event.motion.which;

    CMouse& mouse = GetMouse(devId);
    mouse.Move(glm::ivec2(event.motion.x, event.motion.y));


    glm::vec2 pos = mouse.GetPos(mScreenSize);
    glm::vec2 posPrev = mouse.GetPosPrev(mScreenSize);

    if(pos.x != posPrev.x) {
      handler.SendEvent(CInputDeviceEvent(GetType(), devId, (Uint32)InputMouseEventId::AxisX,
                                          pos.x, posPrev.x));
    }
    if(pos.y != posPrev.y) {
      handler.SendEvent(CInputDeviceEvent(GetType(), devId, (Uint32)InputMouseEventId::AxisY,
                                          pos.x, posPrev.x));
    }
    return true;
  }
  if(event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN) {
    Uint32 devId = event.button.which;

    CMouse& mouse = GetMouse(devId);

    InputMouseEventId eventId = ButtonToEventId(event.button.button);
    CButtonState& state = mouse.Buttons[eventId];

    state.Set(event.button.state == SDL_PRESSED);
    InputDeviceEventVecT events;
    if(state.Pressed()) {
      events.push_back(CInputDeviceEvent(GetType(), devId, (Uint32)eventId));
    }
    if(state.Changed()) {
      events.push_back(CInputDeviceEvent(GetType(), devId, (Uint32)eventId,
                                         state.IsDown(), state.IsUp()));
      events.push_back(CInputDeviceEvent(GetType(), devId, (Uint32)eventId, 
                                         state.IsDown() ? 1.0f : 0.0f,
                                         state.IsUp() ? 1.0f : 0.0f));
    }

    handler.SendEvents(events);
    return true;
  }

  return false;
}

const InputDeviceType CMouseInputDevice::GetType() const {
  return InputDeviceType::Mouse;
}

void CMouseInputDevice::Update(const float timeDelta) {}

CMouseInputDevice::CMouse & CMouseInputDevice::GetMouse(const Uint32 devId) {
  if(mMouses.find(devId) == mMouses.end()) {
    mMouses[devId] = CMouse();
  }
  return mMouses[devId];
}

const InputMouseEventId CMouseInputDevice::ButtonToEventId(const Uint32 button) const {
  switch(button) {
  case SDL_BUTTON_LEFT: return InputMouseEventId::ButtonL;
  case SDL_BUTTON_MIDDLE: return InputMouseEventId::ButtonM;
  case SDL_BUTTON_RIGHT:  return InputMouseEventId::ButtonR;
  case SDL_BUTTON_X1:     return InputMouseEventId::ButtonX1;
  case SDL_BUTTON_X2:     return InputMouseEventId::ButtonX2;
  default:
    return InputMouseEventId::ButtonL;
  }
}

