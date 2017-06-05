#include "stdafx.h"
#include "InputSystem.h"
#include "InputBindings.h"
#include "InputEvents.h"
#include "InputStandardDevices.h"

class CInputSystem::CContext {
public:
  bool Enabled;
  CInputBindings Bindings;
  ObserverVecT Observers;

  CContext();

  void SendEvent(const CInputEvent& event);
};

CInputSystem::CContext::CContext()
  : Enabled(false) {}

void CInputSystem::CContext::SendEvent(const CInputEvent& event) {
  for(ObserverVecT::iterator it = Observers.begin();
      it != Observers.end(); it++) {
    (*it)->OnInputEvent(event);
  }
}

CInputSystem::CInputSystem() {
}

CInputSystem::~CInputSystem() {}

void CInputSystem::SetContextBindings(const cb::string & id, const CInputBindings & bindings) {
  CContext& ctx = GetContext(id);
  ctx.Bindings = bindings;
}

void CInputSystem::SetContextEnabled(const cb::string & id, const bool enable) {
  CContext& ctx = GetContext(id);
  ctx.Enabled = enable;
}

const bool CInputSystem::IsContextEnabled(const cb::string & id) const {
  ContextMapT::const_iterator it = mContexts.find(id);
  if(it != mContexts.end()) {
    return it->second.Enabled;
  }
  return false;
}

void CInputSystem::RegisterObserver(IInputEventObserver * pObserver) {
  if(std::find(mObservers.begin(), mObservers.end(), pObserver) != mObservers.end()) {
    return;
  }
  mObservers.push_back(pObserver);
}

void CInputSystem::UnregisterObserver(IInputEventObserver * pObserver) {
  ObserverVecT::iterator it = std::find(mObservers.begin(), mObservers.end(), pObserver);
  if(it != mObservers.end()) {
    mObservers.erase(it);
  }
}

const bool CInputSystem::ProcessEvent(const SDL_Event & event) {
  return mDevReg.ProcessEvent(event, *this);
}

void CInputSystem::Update(const float timeDelta) {
  for(ObserverVecT::iterator it = mObservers.begin(); it != mObservers.end(); it++) {
    SendEventsToObserver(mEventQueue, *it);
  }
}

CInputSystem::CContext & CInputSystem::GetContext(const cb::string & id) {
  if(mContexts.find(id) == mContexts.end()) {
    mContexts[id] = CContext();
  }
  return mContexts[id];
}

void CInputSystem::SendEvents(const InputDeviceEventVecT & events) {
  if(mContexts.empty())
    return;
  for(InputDeviceEventVecT::const_iterator it = events.begin(); it != events.end(); it++) {
    SendEvent(*it);
  }
}

void CInputSystem::SendEvent(const CInputDeviceEvent & event) {
  if(mContexts.empty()) {
    return;
  }

  for(ContextMapT::iterator it = mContexts.begin(); it != mContexts.end(); it++) {
    if(!it->second.Enabled) {
      continue;
    }
    InputTargetVecT targets = it->second.Bindings.Map(event.DeviceType, event.DeviceId, event.Id);
    if(targets.empty()) {
      continue;
    }
    AddEventToQueue(targets, it->first, event);
  }
}

void CInputSystem::AddEventToQueue(const InputTargetVecT & targets, const cb::string& context, const CInputDeviceEvent & event) {
  for(InputTargetVecT::const_iterator it = targets.begin(); it != targets.end(); it++) {
    mEventQueue.push_back(TranslateEvent(*it, context, event));
  }
}

const CInputEvent CInputSystem::TranslateEvent(const cb::string& target, const cb::string& context, const CInputDeviceEvent& event) const {
  switch(event.Type) {
  default:
  case InputEventType::Action: return CInputEvent(target, context);
  case InputEventType::State: return CInputEvent(target,
                                                 context,
                                                 event.Data.State.Value,
                                                 event.Data.State.ValuePrev);
  case InputEventType::Range: return CInputEvent(target,
                                                 context,
                                                 event.Data.Range.Value,
                                                 event.Data.Range.ValuePrev);
  }
}

void CInputSystem::SendEventsToObserver(const EventVecT & events, IInputEventObserver * pObserver) {
  for(EventVecT::const_iterator it = events.begin(); it != events.end(); it++) {
    pObserver->OnInputEvent(*it);
  }
}
