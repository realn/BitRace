#include "stdafx.h"
#include "InputEvents.h"

CInputEvent::CInputEvent(const Uint32 id, const cb::string & ctxName) 
  : Id(id), ContextName(ctxName), Type(InputEventType::Action)
{}

CInputEvent::CInputEvent(const Uint32 id, 
                         const cb::string & ctxName, 
                         const bool state, 
                         const bool statePrev) 
  : Id(id), ContextName(ctxName), Type(InputEventType::State) 
{
  Data.State.Value = state;
  Data.State.ValuePrev = statePrev;
}

CInputEvent::CInputEvent(const Uint32 id, 
                         const cb::string & ctxName, 
                         const float range, 
                         const float rangePrev) 
  : Id(id), ContextName(ctxName), Type(InputEventType::Range) 
{
  Data.Range.Value = range;
  Data.Range.ValuePrev = rangePrev;
}



IInputEventObserver::IInputEventObserver() {}

IInputEventObserver::~IInputEventObserver() {}

