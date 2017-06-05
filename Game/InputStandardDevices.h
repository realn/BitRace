#ifndef __BITRACE_INPUTSTANDARDDEVICES_H__
#define __BITRACE_INPUTSTANDARDDEVICES_H__

#include <SDL_types.h>
#include <vector>

#include "InputDevice.h"

class CKeyboardInputDevice
  : public IInputDevice {
private:
  class CKeyState;
  typedef std::vector<CKeyState> KeyStateVecT;

  KeyStateVecT mKeys;

public:
  CKeyboardInputDevice();
  virtual ~CKeyboardInputDevice();

  // Inherited via CInputDevice
  virtual const bool ProcessEvent(const SDL_Event & event,
                                  IInputDeviceHandler& handler) override;
  virtual const InputDeviceType GetType() const override;

  virtual void Update(const float timeDelta) override;
};


class CMouseInputDevice
  : public IInputDevice {
private:
  class CButtonState;
  class CMouse;
  typedef std::map<InputMouseEventId, CButtonState> ButtonMapT;
  typedef std::map<Uint32, CMouse> MouseMapT;

  MouseMapT mMouses;
  glm::uvec2 mScreenSize;

public:
  CMouseInputDevice(const glm::uvec2& screenSize);
  virtual ~CMouseInputDevice();

  // Inherited via CInputDevice
  virtual const bool ProcessEvent(const SDL_Event & event,
                                  IInputDeviceHandler& handler) override;
  virtual const InputDeviceType GetType() const;
  virtual void Update(const float timeDelta) override;

private:
  CMouse& GetMouse(const Uint32 devId);
  const InputMouseEventId ButtonToEventId(const Uint32 button) const;
};


#endif // !__BITRACE_INPUTSTANDARDDEVICES_H__

