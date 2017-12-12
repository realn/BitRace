#include "stdafx.h"
#include "InputSystem.h"
#include "InputBindings.h"
#include "InputBindingsXml.h"
#include "InputEvents.h"
#include "FileSystem.h"

class CInputSystem::CContext {
public:
  bool Enabled;
  CInputBindings Bindings;
  ObserverVecT Observers;

  CContext();

  void SendEvent(const CInputEvent& event);
};

class CInputConfig {
public:
  typedef std::map<cb::string, CInputBindings> ContextBindMapT;
  ContextBindMapT mBindMap;
};

static const cb::string XML_INPUTCFG_ELEM = L"Input";
static const cb::string XML_INPUTCFG_CONTEXT_ELEM = L"Context";
static const cb::string XML_INPUTCFG_CONTEXT_ID = L"Id";

CB_DEFINEXMLRW(CInputConfig) {
  return RWNodeMap(mObject.mBindMap,
                   XML_INPUTCFG_CONTEXT_ELEM,
                   XML_INPUTCFG_CONTEXT_ID);
}


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
  if(mContexts.find(id) == mContexts.end()) {
    mContexts[id] = CContext();
  }
}

void CInputSystem::RemoveContext(const cb::string & id) {
  ContextMapT::iterator it = mContexts.find(id);
  if(it != mContexts.end()) {
    mContexts.erase(it);
  }
}

void CInputSystem::ClearContext(const cb::string & id) {
  mContexts[id] = CContext();
}

void CInputSystem::ClearContexts() {
  for(auto item : mContexts) {
    item.second = CContext();
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

const bool CInputSystem::LoadConfig(IFileSystem & fs, const cb::string & filepath) {
  CInputConfig cfg;
  if(!fs.ReadXml(filepath, XML_INPUTCFG_ELEM, cfg)) {
    return false;
  }
  for(auto item : cfg.mBindMap) {
    SetContextBindings(item.first, item.second);
  }
  return true;
}

const bool CInputSystem::SaveConfig(IFileSystem & fs, const cb::string & filepath) const {
  CInputConfig cfg;
  for(auto item : mContexts) {
    cfg.mBindMap[item.first] = item.second.Bindings;
  }
  return fs.WriteXml(filepath, XML_INPUTCFG_ELEM, cfg);
}

CInputDeviceRegistry & CInputSystem::GetDeviceRegistry() {
  return mDevReg;
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
  for(IInputEventObserver* pObserver : mObservers) {
    SendEventsToObserver(mEventQueue, pObserver);
  }
  mEventQueue.clear();
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
  for(auto& event : events) {
    SendEvent(event);
  }
}

void CInputSystem::SendEvent(const CInputDeviceEvent & event) {
  if(mContexts.empty()) {
    return;
  }

  for(auto& item : mContexts) {
    if(!item.second.Enabled) {
      continue;
    }
    InputTargetVecT targets = 
      item.second.Bindings.Map(event.DeviceType, event.DeviceId, event.Id);
    if(targets.empty()) {
      continue;
    }
    AddEventToQueue(targets, item.first, event);
  }
}

void CInputSystem::AddEventToQueue(const InputTargetVecT & targets, const cb::string& context, const CInputDeviceEvent & event) {
  for(auto& target : targets) {
    mEventQueue.push_back(TranslateEvent(target, context, event));
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
  for(auto& event : events) {
    pObserver->OnInputEvent(event);
  }
}

