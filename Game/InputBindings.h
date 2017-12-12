#ifndef __BITRACE_INPUTCONTEXT_H__
#define __BITRACE_INPUTCONTEXT_H__

#include <SDL_types.h>
#include <CBStr/Defines.h>

#include "InputDefines.h"

typedef cb::strvector InputTargetVecT;

class CInputBindings {
public:
  class CBinding {
  public:
    InputDeviceType DeviceType;
    Sint32 DeviceId;
    Uint32 EventId;
    cb::string Target;

    CBinding(const InputDeviceType device,
             const Sint32 deviceId,
             const Uint32 eventId,
             const cb::string& target);
    CBinding();

    const bool Match(const InputDeviceType deviceType,
                     const Uint32 deviceId,
                     const Uint32 eventId) const;

    const bool operator==(const CBinding& other) const;
  };
  typedef std::vector<CBinding> BindVecT;

private:
  InputTargetVecT mTargets;
  BindVecT mBindings;

public:
  CInputBindings();

  void AddTarget(const cb::string& target);
  void RemoveTarget(const cb::string& target);

  void Bind(const cb::string& target,
            const InputDeviceType deviceType,
            const Sint32 deviceId, // -1 for all
            const Uint32 eventId);
  void Unbind(const cb::string& target,
              const InputDeviceType deviceType,
              const Sint32 deviceId, // -1 for all
              const Uint32 eventId);

  const BindVecT& GetAllBindings() const;

  const InputTargetVecT Map(const InputDeviceType device, 
                            const Uint32 deviceId, 
                            const Uint32 eventId) const;
};

#define INPUT_ALL_DEVICES -1

#endif // !__BITRACE_INPUTCONTEXT_H__

