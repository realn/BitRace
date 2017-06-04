#include "stdafx.h"
#include "InputSystem.h"
#include "InputBindings.h"
#include "InputEvents.h"

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

CInputSystem::CInputSystem() {}

CInputSystem::~CInputSystem() {}

void CInputSystem::AddContext(const cb::string & id) {
  GetContext(id);
}

void CInputSystem::RemoveContext(const cb::string & id) {
  ContextMapT::iterator it = mContexts.find(id);
  if(it != mContexts.end()) {
    mContexts.erase(it);
  }
}

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

void CInputSystem::RegisterObserver(const cb::string& ctxId, IInputEventObserver * pObserver) {
  CContext& ctx = GetContext(ctxId);
  ObserverVecT& observers = ctx.Observers;

  if(std::find(observers.begin(), observers.end(), pObserver) != observers.end())
    return;
  observers.push_back(pObserver);
}

void CInputSystem::UnregisterObserver(const cb::string& ctxId, IInputEventObserver * pObserver) {
  CContext& ctx = GetContext(ctxId);
  ObserverVecT& observers = ctx.Observers;

  ObserverVecT::iterator it = std::find(observers.begin(), observers.end(), pObserver);
  if(it != observers.end()) {
    observers.erase(it);
  }
}

CInputSystem::CContext & CInputSystem::GetContext(const cb::string & id) {
  if(mContexts.find(id) == mContexts.end()) {
    mContexts[id] = CContext();
  }
  return mContexts[id];
}

void CInputSystem::SendEvents(const InputDevice device,
                              const Uint32 id,
                              const InputDeviceEventVecT & events) {
  if(mContexts.empty()) {
    return;
  }

  for(ContextMapT::iterator it = mContexts.begin(); it != mContexts.end(); it++) {
    if(!it->second.Enabled) {
      continue;
    }

    IdVecT targets = it->second.Bindings.Map(device, id);
    if(targets.empty()) {
      continue;
    }

    

    SendEventsToObservers(it->first, 
                          targets, 
                          events, 
                          it->second.Observers);
  }
}

void CInputSystem::SendEvent(const InputDevice device,
                             const Uint32 id,
                             const CInputDeviceEvent & event) {
  InputDeviceEventVecT events = {event};
  SendEvents(device, id, events);
}

const CInputSystem::EventVecT CInputSystem::TranslateEvents(const cb::string& ctxId, 
                                                            const IdVecT & targets,
                                                            const InputDeviceEventVecT & events) {
  EventVecT result;
  for(InputDeviceEventVecT::const_iterator eit = events.begin();
      eit != events.end(); eit++) {
    for(IdVecT::const_iterator it = targets.begin(); it != targets.end(); it++) {
      result.push_back(TranslateEvent(ctxId, *it, *eit));
    }
  }
  return result;
}

const CInputEvent CInputSystem::TranslateEvent(const cb::string & ctxId, const Uint32 target, const CInputDeviceEvent & event) {
  switch(event.Type) {
  default:
  case InputEventType::Action: return CInputEvent(target, ctxId);
  case InputEventType::State: return CInputEvent(target,
                                                 ctxId,
                                                 event.Data.State.Value,
                                                 event.Data.State.ValuePrev);
  case InputEventType::Range: return CInputEvent(target,
                                                 ctxId,
                                                 event.Data.Range.Value,
                                                 event.Data.Range.ValuePrev);
  }
}

void CInputSystem::SendEventsToObservers(const cb::string & ctxId, 
                                        const IdVecT & targetIds, 
                                        const InputDeviceEventVecT & events, 
                                        ObserverVecT & observers) {
  for(ObserverVecT::iterator oit = observers.begin(); oit != observers.end(); oit++) {
    for(InputDeviceEventVecT::const_iterator eit = events.begin();
        eit != events.end(); eit++) {
      for(IdVecT::const_iterator tit = targetIds.begin();
          tit != targetIds.end(); tit++) {
        SendEventToObserver(ctxId, *tit, *eit, **oit);
      }
    }
  }
}

