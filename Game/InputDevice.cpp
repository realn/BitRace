#include "stdafx.h"
#include "InputDevice.h"

CInputDevice::CInputDevice() {}

CInputDevice::~CInputDevice() {}


CKeyboardInputDevice::CKeyboardInputDevice() {
  memset(this->m_KeyState, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
  memset(this->m_KeyStatePrev, 0, sizeof(Uint8) * SDL_NUM_SCANCODES);
}

CKeyboardInputDevice::~CKeyboardInputDevice() {}

const bool CKeyboardInputDevice::GetState(const Uint32 type, const Uint32 id) const {
  switch((KeyboardType)type) {
  case KeyboardType::KeyDown:  return IsKeyDown(id);
  case KeyboardType::KeyUp:    return IsKeyUp(id);
  case KeyboardType::KeyPress: return IsKeyPress(id);
  default:
    return false;
  }
}

const float CKeyboardInputDevice::GetRange(const Uint32 type, const Uint32 id) const {
  return 0.0f;
}

const bool CKeyboardInputDevice::ProcessEvent(const SDL_Event & event) {
  return false;
}

void CKeyboardInputDevice::Update(const float timeDelta) {
  const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);

  memcpy(m_KeyStatePrev, m_KeyState, sizeof(Uint8) * SDL_NUM_SCANCODES);
  memcpy(m_KeyState, keyboardState, sizeof(Uint8) * SDL_NUM_SCANCODES);
}

const bool CKeyboardInputDevice::IsKeyDown(const Uint32 id) const {
  return m_KeyState[id] > 0;
}

const bool CKeyboardInputDevice::IsKeyUp(const Uint32 id) const {
  return m_KeyState[id] == 0;
}

const bool CKeyboardInputDevice::IsKeyPress(const Uint32 id) const {
  return m_KeyState[id] && m_KeyStatePrev[id] == 0;
}

CMouseInputDevice::CMouseInputDevice(const glm::uvec2& screenSize)
  : mScreenSize(screenSize)
  , mMouseButtonState(0)
  , mMouseButtonStatePrev(0)
  , mMousePos(0)
  , mMousePosPrev(0) {}

CMouseInputDevice::~CMouseInputDevice() {}

const bool CMouseInputDevice::GetState(const Uint32 type, const Uint32 id) const {
  switch((MouseType)type) {
  case MouseType::ButtonDown: return IsButtonDown(id);
  case MouseType::ButtonUp:   return IsButtonUp(id);
  case MouseType::ButtonPress:  return IsButtonPress(id);
  default:
    return false;
  }
}

const float CMouseInputDevice::GetRange(const Uint32 type, const Uint32 id) const {
  switch((MouseType)type) {
  case MouseType::AxisPos: return GetAxisPos(id);
  case MouseType::AxisDelta:  return GetAxisDelta(id);
  default:
    return 0.0f;
  }
}

const bool CMouseInputDevice::ProcessEvent(const SDL_Event & event) {
  return false;
}

void CMouseInputDevice::Update(const float timeDelta) {
  glm::ivec2 mousePos;
  Uint32 mouseState = SDL_GetMouseState(&mousePos.x, &mousePos.y);

  mMouseButtonStatePrev = mMouseButtonState;
  mMouseButtonState = mouseState;

  mMousePosPrev = mMousePos;
  mMousePos = mousePos;
}

const bool CMouseInputDevice::IsButtonDown(const Uint32 id) const {
  return (mMouseButtonState & SDL_BUTTON(id)) > 0;
}

const bool CMouseInputDevice::IsButtonUp(const Uint32 id) const {
  return (mMouseButtonState & SDL_BUTTON(id)) == 0;
}

const bool CMouseInputDevice::IsButtonPress(const Uint32 id) const {
  return
    (mMouseButtonState & SDL_BUTTON(id)) &&
    !(mMouseButtonStatePrev & SDL_BUTTON(id));
}

const float CMouseInputDevice::GetAxisPos(const Uint32 id) const {
  glm::vec2 pos = glm::vec2(mMousePos) / glm::vec2(mScreenSize);
  switch((MouseAxisId)id) {
  case MouseAxisId::AxisX:  return pos.x;
  case MouseAxisId::AxisY:  return pos.y;
  default:
    return 0.0f;
  }
}

const float CMouseInputDevice::GetAxisDelta(const Uint32 id) const {
  glm::vec2 delta = glm::vec2(mMousePos - mMousePosPrev) / glm::vec2(mScreenSize);
  switch((MouseAxisId)id) {
  case MouseAxisId::AxisX:  return delta.x;
  case MouseAxisId::AxisY:  return delta.y;
  default:
    return 0.0f;
  }
}

const glm::ivec2 CMouseInputDevice::GetPos() const {
  return mMousePos;
}

const glm::ivec2 CMouseInputDevice::GetPosDelta() const {
  return mMousePos - mMousePosPrev;
}

CInputDeviceMap::CInputDeviceMap() {}

CInputDeviceMap::~CInputDeviceMap() {
  for(DeviceMapT::iterator it = mDeviceMap.begin(); it != mDeviceMap.end(); it++) {
    delete it->second;
  }
  mDeviceMap.clear();
}

void CInputDeviceMap::AddDevice(const InputDevice id, CInputDevice * pDevice) {
  DeviceMapT::iterator it = mDeviceMap.find(id);
  if(it != mDeviceMap.end()) {
    mDeviceMap.erase(it);
  }
  mDeviceMap[id] = pDevice;
}

const bool CInputDeviceMap::GetState(const InputDevice devId, const Uint32 type, const Uint32 id) const {
  DeviceMapT::const_iterator it = mDeviceMap.find(devId);
  if(it != mDeviceMap.end())
    return it->second->GetState(type, id);
  return false;
}

const float CInputDeviceMap::GetRange(const InputDevice devId, const Uint32 type, const Uint32 id) const {
  DeviceMapT::const_iterator it = mDeviceMap.find(devId);
  if(it != mDeviceMap.end())
    return it->second->GetRange(type, id);
  return 0.0f;
}

const bool CInputDeviceMap::ProcessEvent(const SDL_Event & event) {
  for(DeviceMapT::iterator it = mDeviceMap.begin(); it != mDeviceMap.end(); it++) {
    if(it->second->ProcessEvent(event))
      return true;
  }
  return false;
}

void CInputDeviceMap::Update(const float timeDelta) {
  for(DeviceMapT::iterator it = mDeviceMap.begin(); it != mDeviceMap.end(); it++) {
    it->second->Update(timeDelta);
  }
}
