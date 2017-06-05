#ifndef __BITRACE_INPUTCONTEXT_H__
#define __BITRACE_INPUTCONTEXT_H__

#include <SDL_types.h>
#include <CBStr/Defines.h>

#include "InputDefines.h"

typedef cb::strvector InputTargetVecT;

class CInputBindings {
private:
  class CBinding;
  typedef std::vector<CBinding> BindVecT;

private:
  InputTargetVecT mTargets;
  BindVecT mBindings;

public:
  CInputBindings();

  void AddTarget(const cb::string& target);
  void RemoveTarget(const cb::string& target);

  void Bind(const InputDeviceType deviceType,
            const Sint32 deviceId, // -1 for all
            const Uint32 eventId,
            const cb::string& target);
  void Unbind(const InputDeviceType deviceType,
              const Sint32 deviceId, // -1 for all
              const Uint32 eventId,
              const cb::string& target);

  const InputTargetVecT Map(const InputDeviceType device, 
                            const Uint32 deviceId, 
                            const Uint32 eventId) const;
};

#endif // !__BITRACE_INPUTCONTEXT_H__

