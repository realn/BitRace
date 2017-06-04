#ifndef __BITRACE_INPUTDEVICEEVENTS_H__
#define __BITRACE_INPUTDEVICEEVENTS_H__

#include <vector>

#include <SDL_types.h>
#include "InputDefines.h"

class CInputDeviceEvent {
public:
  template<typename _Type>
  class CData {
  public:
    _Type Value;
    _Type ValuePrev;
  };

  InputEventType Type;
  union {
    CData<bool> State;
    CData<float> Range;
  } Data;

  CInputDeviceEvent();
  CInputDeviceEvent(const bool state, const bool statePrev);
  CInputDeviceEvent(const float range, const float rangePrev);
};
typedef std::vector<CInputDeviceEvent> InputDeviceEventVecT;

class IInputDeviceObserver {
public:
  virtual void SendEvents(const InputDevice device,
                          const Uint32 id,
                          const InputDeviceEventVecT& events) = 0;
  virtual void SendEvent(const InputDevice device,
                         const Uint32 id,
                         const CInputDeviceEvent& event) = 0;
};

#endif // !__BITRACE_INPUTDEVICEEVENTS_H__

