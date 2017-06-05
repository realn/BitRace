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

  InputDeviceType DeviceType;
  Uint32 DeviceId;
  Uint32 Id;
  InputEventType Type;
  union {
    CData<bool> State;
    CData<float> Range;
  } Data;

  CInputDeviceEvent(const InputDeviceType deviceType, 
                    const Uint32 deviceId, 
                    const Uint32 id);
  CInputDeviceEvent(const InputDeviceType deviceType, 
                    const Uint32 deviceId, 
                    const Uint32 id, 
                    const bool state, 
                    const bool statePrev);
  CInputDeviceEvent(const InputDeviceType deviceType, 
                    const Uint32 deviceId, 
                    const Uint32 id, 
                    const float range, 
                    const float rangePrev);
};
typedef std::vector<CInputDeviceEvent> InputDeviceEventVecT;

class IInputDeviceHandler {
public:
  IInputDeviceHandler();
  virtual ~IInputDeviceHandler();

  virtual void SendEvents(const InputDeviceEventVecT& events) = 0;
  virtual void SendEvent(const CInputDeviceEvent& event) = 0;

protected:
  IInputDeviceHandler(const IInputDeviceHandler&) = delete;
  IInputDeviceHandler& operator=(IInputDeviceHandler&) = delete;
};

#endif // !__BITRACE_INPUTDEVICEEVENTS_H__

