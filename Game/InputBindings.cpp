#include "stdafx.h"
#include "InputBindings.h"

class CInputBindings::CBinding {
public:
  InputDevice Device;
  Uint32 EventId;
  Uint32 TargetId;

  CBinding(const InputDevice device,
           const Uint32 eventId,
           const Uint32 targetId);

  const bool operator==(const CBinding& other) const;
  const bool operator!=(const CBinding& other) const;
};

CInputBindings::CBinding::CBinding(const InputDevice device,
                                  const Uint32 eventId,
                                  const Uint32 targetId)
  : Device(device)
  , EventId(eventId)
  , TargetId(targetId) {}

const bool CInputBindings::CBinding::operator==(const CBinding & other) const {
  return
    Device == other.Device &&
    EventId == other.EventId &&
    TargetId == other.TargetId;
}

const bool CInputBindings::CBinding::operator!=(const CBinding & other) const {
  return !(*this == other);
}


CInputBindings::CInputBindings() {}

void CInputBindings::RegisterTarget(const Uint32 id) {
  if(std::find(mTargets.begin(), mTargets.end(), id) != mTargets.end()) {
    return;
  }
  mTargets.push_back(id);
}

void CInputBindings::UnregisterTarget(const Uint32 id) {
  TargetVecT::iterator it = std::find(mTargets.begin(), mTargets.end(), id);
  if(it == mTargets.end()) {
    return;
  }
  mTargets.erase(it);
}

void CInputBindings::Bind(const InputDevice device,
                         const Uint32 eventId,
                         const Uint32 targetId) {
  CBinding binding(device, eventId, targetId);
  if(std::find(mBindings.begin(), mBindings.end(), binding) != mBindings.end()) {
    return;
  }
  mBindings.push_back(binding);
}

void CInputBindings::Unbind(const InputDevice device,
                           const Uint32 eventId,
                           const Uint32 targetId) {
  CBinding binding(device, eventId, targetId);
  BindVecT::iterator it = std::find(mBindings.begin(), mBindings.end(), binding);
  if(it != mBindings.end()) {
    mBindings.erase(it);
  }
}

const CInputBindings::TargetVecT CInputBindings::Map(const InputDevice device,
                                                   const Uint32 eventId) const {
  TargetVecT ids;
  for(BindVecT::const_iterator it = mBindings.begin(); it != mBindings.end(); it++) {
    if(it->Device == device && it->EventId == eventId) {
      ids.push_back(it->TargetId);
    }
  }
  return ids;
}

