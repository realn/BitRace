#include "stdafx.h"
#include "InputBindings.h"

CInputBindings::CBinding::CBinding(const InputDeviceType deviceType,
                                   const Sint32 deviceId,
                                   const Uint32 eventId,
                                   const cb::string& target)
  : DeviceType(deviceType)
  , DeviceId(deviceId)
  , EventId(eventId)
  , Target(target) {}

CInputBindings::CBinding::CBinding() 
  : CBinding(InputDeviceType::Keyboard, INPUT_ALL_DEVICES, 0, cb::string())
{}

const bool CInputBindings::CBinding::Match(const InputDeviceType deviceType, const Uint32 deviceId, const Uint32 eventId) const {
  if(DeviceType != deviceType || EventId != eventId) {
    return false;
  }
  if(DeviceId >= 0 && (Uint32)DeviceId == deviceId) {
    return false;
  }
  return true;
}

const bool CInputBindings::CBinding::operator==(const CBinding & other) const {
  return
    DeviceType == other.DeviceType &&
    DeviceId == other.DeviceId &&
    EventId == other.EventId &&
    Target == other.Target;
}

CInputBindings::CInputBindings() {}

void CInputBindings::AddTarget(const cb::string& target) {
  if(std::find(mTargets.begin(), mTargets.end(), target) != mTargets.end()) {
    return;
  }
  mTargets.push_back(target);
}

void CInputBindings::RemoveTarget(const cb::string& target) {
  InputTargetVecT::iterator it = std::find(mTargets.begin(), mTargets.end(), target);
  if(it == mTargets.end()) {
    return;
  }
  mTargets.erase(it);
}

void CInputBindings::Bind(const cb::string& target,
                          const InputDeviceType deviceType,
                          const Sint32 deviceId,
                          const Uint32 eventId) {
  CBinding binding(deviceType, deviceId, eventId, target);
  if(std::find(mBindings.begin(), mBindings.end(), binding) != mBindings.end()) {
    return;
  }
  mBindings.push_back(binding);
}

void CInputBindings::Unbind(const cb::string& target,
                            const InputDeviceType deviceType,
                            const Sint32 deviceId,
                            const Uint32 eventId) {
  CBinding binding(deviceType, deviceId, eventId, target);
  BindVecT::iterator it = std::find(mBindings.begin(), mBindings.end(), binding);
  if(it != mBindings.end()) {
    mBindings.erase(it);
  }
}

const CInputBindings::BindVecT & CInputBindings::GetAllBindings() const {
  return mBindings;
}

const InputTargetVecT CInputBindings::Map(const InputDeviceType deviceType, 
                                          const Uint32 deviceId,
                                          const Uint32 eventId) const {
  InputTargetVecT targets;
  for(BindVecT::const_iterator it = mBindings.begin(); it != mBindings.end(); it++) {
    if(it->Match(deviceType, deviceId, eventId)) {
      targets.push_back(it->Target);
    }
  }
  return targets;
}




