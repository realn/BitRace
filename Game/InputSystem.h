#ifndef __BITRACE_INPUTSYSTEM_H__
#define __BITRACE_INPUTSYSTEM_H__

#include <SDL_types.h>
#include <SDL_events.h>
#include <glm/fwd.hpp>
#include <CBStr/Defines.h>

#include "InputDefines.h"
#include "InputDeviceEvents.h"

class CInputBindings;
class IInputEventObserver;

class CInputSystem 
  : public IInputDeviceObserver {
private:
  class CContext;
  typedef std::map<cb::string, CContext> ContextMapT;
  typedef std::vector<IInputEventObserver*> ObserverVecT;
  typedef std::vector<Uint32> IdVecT;
  typedef std::vector<CInputEvent> EventVecT;

  ContextMapT mContexts;

public:
  CInputSystem();
  virtual ~CInputSystem();

  void AddContext(const cb::string& id);
  void RemoveContext(const cb::string& id);

  void SetContextBindings(const cb::string& id, const CInputBindings& bindings);
  void SetContextEnabled(const cb::string& id, const bool enable = true);

  const bool IsContextEnabled(const cb::string& id) const;

  void RegisterObserver(const cb::string& ctxId, IInputEventObserver* pObserver);
  void UnregisterObserver(const cb::string& ctxId, IInputEventObserver* pObserver);

private:
  CContext& GetContext(const cb::string& id);

  // Inherited via IInputDeviceObserver
  virtual void SendEvents(const InputDevice device, const Uint32 id, const InputDeviceEventVecT & events) override;
  virtual void SendEvent(const InputDevice device, const Uint32 id, const CInputDeviceEvent & event) override;
  
  const EventVecT TranslateEvents(const cb::string& ctxId,
                                  const IdVecT& targets, 
                                  const InputDeviceEventVecT& events);
  const CInputEvent TranslateEvent(const cb::string& ctxId,
                                   const Uint32 target,
                                   const CInputDeviceEvent& event);

  void SendEventsToObservers(const cb::string& ctxId,
                            const IdVecT& targetIds,
                            const InputDeviceEventVecT& events,
                            ObserverVecT& observers);
  void SendEventToObserver(const cb::string& ctxId,
                           const Uint32 targetId,
                           const CInputDeviceEvent& event,
                           IInputEventObserver& observer)
};

#endif // !__BITRACE_INPUTSYSTEM_H__

