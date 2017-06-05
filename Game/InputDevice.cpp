#include "stdafx.h"
#include "InputDevice.h"
#include "InputDeviceEvents.h"


IInputDevice::IInputDevice() {}

IInputDevice::~IInputDevice() {}


CInputDeviceRegistry::CInputDeviceRegistry() {}

CInputDeviceRegistry::~CInputDeviceRegistry() {
  Clear();
}

void CInputDeviceRegistry::AddDevice(const cb::string& name, IInputDevice * pDevice) {
  mDeviceMap[name] = pDevice;
}

void CInputDeviceRegistry::RemoveDevice(const cb::string & name) {

}

void CInputDeviceRegistry::Clear() {
  for(DeviceMapT::iterator it = mDeviceMap.begin(); it != mDeviceMap.end(); it++) {
    delete it->second;
  }
  mDeviceMap.clear();
}

const bool CInputDeviceRegistry::ProcessEvent(const SDL_Event & event, IInputDeviceHandler& observer) {
  for(DeviceMapT::iterator it = mDeviceMap.begin(); it != mDeviceMap.end(); it++) {
    if(it->second->ProcessEvent(event, observer)) {
      return true;
    }
  }
  return false;
}

void CInputDeviceRegistry::Update(const float timeDelta) {
  for(DeviceMapT::iterator it = mDeviceMap.begin(); it != mDeviceMap.end(); it++) {
    it->second->Update(timeDelta);
  }
}


