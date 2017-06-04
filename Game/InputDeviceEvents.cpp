#include "stdafx.h"
#include "InputDeviceEvents.h"

CInputDeviceEvent::CInputDeviceEvent()
  : Type(InputEventType::Action) {}

CInputDeviceEvent::CInputDeviceEvent(const bool state, 
                                     const bool statePrev)
  : Type(InputEventType::State) {
  Data.State.Value = state;
  Data.State.ValuePrev = statePrev;
}

CInputDeviceEvent::CInputDeviceEvent(const float range, 
                                     const float rangePrev)
  : Type(InputEventType::Range) {
  Data.Range.Value = range;
  Data.Range.ValuePrev = rangePrev;
}
