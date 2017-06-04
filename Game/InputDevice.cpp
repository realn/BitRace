#include "stdafx.h"
#include "InputDevice.h"
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




CInputDevice::CInputDevice() {}

CInputDevice::~CInputDevice() {}


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

const bool CKeyboardInputDevice::GetState(const Uint32 id) const {
  return mKeys[id].IsDown();
}

const float CKeyboardInputDevice::GetRange(const Uint32 id) const {
  return mKeys[id].IsDown() ? 1.0f : 0.0f;
}

const bool CKeyboardInputDevice::ProcessEvent(const SDL_Event & event,
                                              IInputDeviceObserver& observer) {
  if(event.type != SDL_KEYDOWN && event.type != SDL_KEYUP) {
    return false;
  }

  SDL_Scancode code = event.key.keysym.scancode;
  CKeyState& state = mKeys[code];
  state.Set(event.key.state == SDL_PRESSED);

  InputDeviceEventVecT events;
  if(state.Pressed()) {
    events.push_back(CInputDeviceEvent());
  }

  if(state.Changed()) {
    events.push_back(CInputDeviceEvent(state.IsDown(), state.IsUp()));
    events.push_back(CInputDeviceEvent(state.IsDown() ? 1.0f : 0.0f,
                                 state.IsUp() ? 1.0f : 0.0f));
  }

  observer.SendEvents(InputDevice::Keyboard, code, events);
  return true;
}

void CKeyboardInputDevice::Update(const float timeDelta) {}



CMouseInputDevice::CButtonState::CButtonState()
  : State(false), StatePrev(false) {}

void CMouseInputDevice::CButtonState::Set(const bool value) {}

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

CMouseInputDevice::CMouseInputDevice(const glm::uvec2& screenSize)
  : mScreenSize(screenSize)
  , mMousePos(0)
  , mMousePosPrev(0) {
  mButtons = {
    {MouseEventId::ButtonL, CButtonState()},
    {MouseEventId::ButtonM, CButtonState()},
    {MouseEventId::ButtonR, CButtonState()},
    {MouseEventId::ButtonX1, CButtonState()},
    {MouseEventId::ButtonX2, CButtonState()},
  };
}

CMouseInputDevice::~CMouseInputDevice() {}

const bool CMouseInputDevice::GetState(const Uint32 id) const {
  if(cb::any(id, {MouseEventId::AxisX, MouseEventId::AxisY, MouseEventId::AxisZ})) {
    return false;
  }
  return mButtons.at((MouseEventId)id).IsDown();
}

const float CMouseInputDevice::GetRange(const Uint32 id) const {
  glm::vec2 pos = glm::vec2(mMousePos) / glm::vec2(mScreenSize);
  switch((MouseEventId)id) {
  case MouseEventId::AxisX: return pos.x;
  case MouseEventId::AxisY: return pos.y;
  case MouseEventId::AxisZ: return 0.0f;
  default:
    return;
  }
}

const bool CMouseInputDevice::ProcessEvent(const SDL_Event & event,
                                           IInputDeviceObserver& observer) {
  if(!cb::any(event.type, {SDL_MOUSEMOTION, SDL_MOUSEBUTTONUP, SDL_MOUSEBUTTONDOWN, SDL_MOUSEWHEEL})) {
    return false;
  }

  if(event.type == SDL_MOUSEMOTION) {
    mMousePosPrev = mMousePos;
    mMousePos = glm::vec2(event.motion.x, event.motion.y);

    glm::vec2 pos = glm::vec2(mMousePos) / glm::vec2(mScreenSize);
    glm::vec2 posPrev = glm::vec2(mMousePosPrev) / glm::vec2(mScreenSize);

    if(mMousePos.x != mMousePosPrev.x) {
      observer.SendEvent(InputDevice::Mouse, (Uint32)MouseEventId::AxisX,
                         CInputDeviceEvent(pos.x, posPrev.x));
    }
    if(mMousePos.y != mMousePosPrev.y) {
      observer.SendEvent(InputDevice::Mouse, (Uint32)MouseEventId::AxisY,
                         CInputDeviceEvent(pos.y, posPrev.y));
    }
    return true;
  }
  if(event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN) {
    MouseEventId eventId = ButtonToEventId(event.button.button);
    CButtonState& state = mButtons[eventId];

    state.Set(event.button.state == SDL_PRESSED);
    InputDeviceEventVecT events;
    if(state.Pressed()) {
      events.push_back(CInputDeviceEvent());
    }
    if(state.Changed()) {
      events.push_back(CInputDeviceEvent(state.IsDown(), state.IsUp()));
      events.push_back(CInputDeviceEvent(state.IsDown() ? 1.0f : 0.0f,
                                   state.IsUp() ? 1.0f : 0.0f));
    }

    observer.SendEvents(InputDevice::Mouse, (Uint32)eventId, events);
    return true;
  }

  return false;
}

void CMouseInputDevice::Update(const float timeDelta) {}

const MouseEventId CMouseInputDevice::ButtonToEventId(const Uint32 button) const {
  switch(button) {
  case SDL_BUTTON_LEFT: return MouseEventId::ButtonL;
  case SDL_BUTTON_MIDDLE: return MouseEventId::ButtonM;
  case SDL_BUTTON_RIGHT:  return MouseEventId::ButtonR;
  case SDL_BUTTON_X1:     return MouseEventId::ButtonX1;
  case SDL_BUTTON_X2:     return MouseEventId::ButtonX2;
  default:
    return MouseEventId::ButtonL;
  }
}

CInputDeviceMap::CInputDeviceMap() {}

CInputDeviceMap::~CInputDeviceMap() {
  Clear();
}

void CInputDeviceMap::AddDevice(const InputDevice id, CInputDevice * pDevice) {
  DeviceMapT::iterator it = mDeviceMap.find(id);
  if(it != mDeviceMap.end()) {
    mDeviceMap.erase(it);
  }
  mDeviceMap[id] = pDevice;
}

void CInputDeviceMap::Clear() {
  for(DeviceMapT::iterator it = mDeviceMap.begin(); it != mDeviceMap.end(); it++) {
    delete it->second;
  }
  mDeviceMap.clear();
}

const bool CInputDeviceMap::GetState(const InputDevice devId, const Uint32 id) const {
  DeviceMapT::const_iterator it = mDeviceMap.find(devId);
  if(it != mDeviceMap.end())
    return it->second->GetState(id);
  return false;
}

const float CInputDeviceMap::GetRange(const InputDevice devId, const Uint32 id) const {
  DeviceMapT::const_iterator it = mDeviceMap.find(devId);
  if(it != mDeviceMap.end())
    return it->second->GetRange(id);
  return 0.0f;
}

const bool CInputDeviceMap::ProcessEvent(const SDL_Event & event, 
                                         IInputDeviceObserver& observer) {
  for(DeviceMapT::iterator it = mDeviceMap.begin(); it != mDeviceMap.end(); it++) {
    if(it->second->ProcessEvent(event, observer)) {
      return true;
    }
  }
  return false;
}

void CInputDeviceMap::Update(const float timeDelta) {
  for(DeviceMapT::iterator it = mDeviceMap.begin(); it != mDeviceMap.end(); it++) {
    it->second->Update(timeDelta);
  }
}

