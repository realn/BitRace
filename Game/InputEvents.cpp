#include "stdafx.h"
#include "InputEvents.h"

CInputEvent::CInputEvent(const cb::string& id, const cb::string & ctxName) 
  : Id(id), Context(ctxName), Type(InputEventType::Action)
{}

CInputEvent::CInputEvent(const cb::string& id, 
                         const cb::string& ctxName, 
                         const bool state, 
                         const bool statePrev) 
  : Id(id), Context(ctxName), Type(InputEventType::State) 
{
  Data.State.Value = state;
  Data.State.ValuePrev = statePrev;
}

CInputEvent::CInputEvent(const cb::string& id, 
                         const cb::string& ctxName, 
                         const float range, 
                         const float rangePrev) 
  : Id(id), Context(ctxName), Type(InputEventType::Range) 
{
  Data.Range.Value = range;
  Data.Range.ValuePrev = rangePrev;
}



IInputEventObserver::IInputEventObserver() {}

IInputEventObserver::~IInputEventObserver() {}

