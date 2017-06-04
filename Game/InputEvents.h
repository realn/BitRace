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

  Uint32 Id;
  cb::string ContextName;
  InputEventType Type;
  union {
    CData<bool> State;
    CData<float> Range;
  } Data;

  CInputEvent(const Uint32 id, const cb::string& ctxName);
  CInputEvent(const Uint32 id,
              const cb::string& ctxName,
              const bool state,
              const bool statePrev);
  CInputEvent(const Uint32 id,
              const cb::string& ctxName,
              const float range,
              const float rangePrev);
};

class IInputEventObserver {
protected:
  IInputEventObserver();
public:
  virtual ~IInputEventObserver();

  virtual void OnInputEvent(const CInputEvent& event) = 0;
};

#endif // !__BITRACE_INPUTEVENTS_H__

