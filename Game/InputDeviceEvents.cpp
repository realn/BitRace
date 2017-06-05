#include "stdafx.h"
#include "InputDeviceEvents.h"

CInputDeviceEvent::CInputDeviceEvent(const InputDeviceType deviceType,
                                     const Uint32 deviceId,
                                     const Uint32 id)
  : DeviceType(deviceType)
  , DeviceId(deviceId)
  , Id(id)
  , Type(InputEventType::Action) {}

CInputDeviceEvent::CInputDeviceEvent(const InputDeviceType deviceType,
                                     const Uint32 deviceId,
                                     const Uint32 id,
                                     const bool state,
                                     const bool statePrev)
  : DeviceType(deviceType)
  , DeviceId(deviceId)
  , Id(id)
  , Type(InputEventType::State) {
  Data.State.Value = state;
  Data.State.ValuePrev = statePrev;
}

CInputDeviceEvent::CInputDeviceEvent(const InputDeviceType deviceType,
                                     const Uint32 deviceId,
                                     const Uint32 id,
                                     const float range,
                                     const float rangePrev)
  : DeviceType(deviceType)
  , DeviceId(deviceId)
  , Id(id)
  , Type(InputEventType::Range) {
  Data.Range.Value = range;
  Data.Range.ValuePrev = rangePrev;
}

IInputDeviceHandler::IInputDeviceHandler() {}

IInputDeviceHandler::~IInputDeviceHandler() {}
