#ifndef __BITRACE_INPUTDEVICE_H__
#define __BITRACE_INPUTDEVICE_H__

#include <SDL_types.h>
#include <SDL_events.h>
#include <glm/glm.hpp>
#include <map>

#include "InputDefines.h"

class IInputDeviceObserver;

class CInputDevice {
public:
  CInputDevice();
  virtual ~CInputDevice();

  virtual const bool GetState(const Uint32 id) const = 0;
  virtual const float GetRange(const Uint32 id) const = 0;

  virtual const bool ProcessEvent(const SDL_Event& event,
                                  IInputDeviceObserver& observer) = 0;

  virtual void Update(const float timeDelta) = 0;
};

class CInputDeviceMap {
private:
  typedef std::map<InputDevice, CInputDevice*> DeviceMapT;

  DeviceMapT mDeviceMap;

public:
  CInputDeviceMap();
  ~CInputDeviceMap();

  void AddDevice(const InputDevice id, CInputDevice* pDevice);
  void Clear();

  const bool GetState(const InputDevice devId, const Uint32 id) const;
  const float GetRange(const InputDevice devId, const Uint32 id) const;

  const bool ProcessEvent(const SDL_Event& event, 
                          IInputDeviceObserver& observer);

  void Update(const float timeDelta);
};


class CKeyboardInputDevice
  : public CInputDevice {
private:
  class CKeyState;
  typedef std::vector<CKeyState> KeyStateVecT;

  KeyStateVecT mKeys;

public:
  CKeyboardInputDevice();
  virtual ~CKeyboardInputDevice();

  // Inherited via CInputDevice
  virtual const bool GetState(const Uint32 id) const override;
  virtual const float GetRange(const Uint32 id) const override;

  virtual const bool ProcessEvent(const SDL_Event & event,
                                  IInputDeviceObserver& observer) override;
  virtual void Update(const float timeDelta) override;
};


class CMouseInputDevice
  : public CInputDevice {
private:
  class CButtonState;
  typedef std::map<MouseEventId, CButtonState> ButtonMapT;

  glm::uvec2 mScreenSize;
  ButtonMapT mButtons;
  glm::ivec2  mMousePos;
  glm::ivec2  mMousePosPrev;

public:
  CMouseInputDevice(const glm::uvec2& screenSize);
  virtual ~CMouseInputDevice();

  // Inherited via CInputDevice
  virtual const bool GetState(const Uint32 id) const override;
  virtual const float GetRange(const Uint32 id) const override;

  virtual const bool ProcessEvent(const SDL_Event & event, 
                                  IInputDeviceObserver& observer) override;
  virtual void Update(const float timeDelta) override;

private:
  const MouseEventId ButtonToEventId(const Uint32 button) const;
};

#endif // !__BITRACE_INPUTDEVICE_H__

