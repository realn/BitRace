#ifndef __BITRACE_INPUTCONTEXT_H__
#define __BITRACE_INPUTCONTEXT_H__

#include <SDL_types.h>

#include "InputDefines.h"

class CInputBindings {
public:
  typedef std::vector<Uint32> TargetVecT;

private:
  class CBinding;
  typedef std::vector<CBinding> BindVecT;

private:
  TargetVecT mTargets;
  BindVecT mBindings;

public:
  CInputBindings();

  void RegisterTarget(const Uint32 id);
  void UnregisterTarget(const Uint32 id);

  void Bind(const InputDevice device,
            const Uint32 eventId,
            const Uint32 targetId);
  void Unbind(const InputDevice device,
              const Uint32 eventId,
              const Uint32 targetId);

  const TargetVecT Map(const InputDevice device, const Uint32 eventId) const;
};

#endif // !__BITRACE_INPUTCONTEXT_H__

