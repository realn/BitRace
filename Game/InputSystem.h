#ifndef __BITRACE_INPUTSYSTEM_H__
#define __BITRACE_INPUTSYSTEM_H__

#include <SDL_types.h>
#include <SDL_events.h>
#include <glm/fwd.hpp>
#include <CBStr/Defines.h>
#include <CBIO/Ptr.h>

#include "InputDefines.h"
#include "InputDeviceEvents.h"
#include "InputDevice.h"

class IFileSystem;
class CInputEvent;
class CInputBindings;
class IInputEventObserver;
typedef cb::strvector InputTargetVecT;

class CInputSystem 
  : public IInputDeviceHandler {
private:
  class CContext;
  typedef std::map<cb::string, CContext> ContextMapT;
  typedef std::vector<IInputEventObserver*> ObserverVecT;
  typedef std::vector<CInputEvent> EventVecT;

  ContextMapT mContexts;
  ObserverVecT mObservers;
  CInputDeviceRegistry mDevReg;
  EventVecT mEventQueue;

public:
  CInputSystem();
  virtual ~CInputSystem();

  void AddContext(const cb::string& id);
  void RemoveContext(const cb::string& id);
  void ClearContext(const cb::string& id);
  void ClearContexts();

  void SetContextBindings(const cb::string& id, const CInputBindings& bindings);
  void SetContextEnabled(const cb::string& id, const bool enable = true);

  const bool IsContextEnabled(const cb::string& id) const;

  const bool LoadConfig(IFileSystem& fs, const cb::string& filepath);
  const bool SaveConfig(IFileSystem& fs, const cb::string& filepath) const;

  CInputDeviceRegistry& GetDeviceRegistry();

  void RegisterObserver(IInputEventObserver* pObserver);
  void UnregisterObserver(IInputEventObserver* pObserver);

  const bool ProcessEvent(const SDL_Event& event);

  void Update(const float timeDelta);

private:
  CContext& GetContext(const cb::string& id);

  // Inherited via IInputDeviceHandler
  virtual void SendEvents(const InputDeviceEventVecT & events) override;
  virtual void SendEvent(const CInputDeviceEvent & event) override;
  
  void AddEventToQueue(const InputTargetVecT& targets, const cb::string& context, const CInputDeviceEvent& event);

  const CInputEvent TranslateEvent(const cb::string& target, const cb::string& context, const CInputDeviceEvent& event) const;
  void SendEventsToObserver(const EventVecT& events, IInputEventObserver* pObserver);
};

#endif // !__BITRACE_INPUTSYSTEM_H__

