#ifndef __BITRACE_INPUTEVENTS_H__
#define __BITRACE_INPUTEVENTS_H__

#include <SDL_types.h>
#include "InputDefines.h"

class CInputEvent {
public:
  template<typename _Type>
  class CData {
  public:
    _Type Value;
    _Type ValuePrev;
  };

  cb::string Id;
  cb::string Context;
  InputEventType Type;
  union {
    CData<bool> State;
    CData<float> Range;
  } Data;

  CInputEvent(const cb::string& id, const cb::string& ctxName);
  CInputEvent(const cb::string& id,
              const cb::string& ctxName,
              const bool state,
              const bool statePrev);
  CInputEvent(const cb::string& id,
              const cb::string& ctxName,
              const float range,
              const float rangePrev);
};

class IInputEventObserver {
public:
  IInputEventObserver();
  virtual ~IInputEventObserver();

  virtual void OnInputEvent(const CInputEvent& event) = 0;

protected:
  IInputEventObserver(const IInputEventObserver&) = delete;
  IInputEventObserver& operator=(IInputEventObserver&) = delete;
};

#endif // !__BITRACE_INPUTEVENTS_H__

