#ifndef __BITRACE_INPUTDEVICE_H__
#define __BITRACE_INPUTDEVICE_H__

#include <SDL_types.h>
#include <SDL_events.h>
#include <glm/glm.hpp>
#include <map>

#include "InputDefines.h"

class IInputDeviceHandler;

class IInputDevice {
public:
  IInputDevice();
  virtual ~IInputDevice();

  virtual const bool ProcessEvent(const SDL_Event& event,
                                  IInputDeviceHandler& handler) = 0;

  virtual const InputDeviceType GetType() const = 0;

  virtual void Update(const float timeDelta) = 0;

protected:
  IInputDevice(const IInputDevice&) = delete;
  IInputDevice& operator=(IInputDevice&) = delete;
};

class CInputDeviceRegistry {
private:
  typedef std::map<cb::string, IInputDevice*> DeviceMapT;

  DeviceMapT mDeviceMap;

public:
  CInputDeviceRegistry();
  ~CInputDeviceRegistry();

  void AddDevice(const cb::string& name, IInputDevice* pDevice);
  void RemoveDevice(const cb::string& name);
  void Clear();

  const bool ProcessEvent(const SDL_Event& event, IInputDeviceHandler& handler);

  void Update(const float timeDelta);
};


#endif // !__BITRACE_INPUTDEVICE_H__

