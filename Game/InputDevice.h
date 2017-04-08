#ifndef __BITRACE_INPUTDEVICE_H__
#define __BITRACE_INPUTDEVICE_H__

#include <SDL_types.h>
#include <SDL_events.h>
#include <glm/glm.hpp>
#include <map>

class CInputDevice {
public:
  CInputDevice();
  virtual ~CInputDevice();

  virtual const bool GetState(const Uint32 type, const Uint32 id) const = 0;
  virtual const float GetRange(const Uint32 type, const Uint32 id) const = 0;

  virtual const bool ProcessEvent(const SDL_Event& event) = 0;

  virtual void Update(const float timeDelta) = 0;
};

enum class InputDevice {
  Keyboard = 0,
  Mouse = 1,
};

class CInputDeviceMap {
private:
  typedef std::map<InputDevice, CInputDevice*> DeviceMapT;

  DeviceMapT mDeviceMap;

public:
  CInputDeviceMap();
  ~CInputDeviceMap();

  void AddDevice(const InputDevice id, CInputDevice* pDevice);

  const bool GetState(const InputDevice devId, const Uint32 type, const Uint32 id) const;
  const float GetRange(const InputDevice devId, const Uint32 type, const Uint32 id) const;

  const bool ProcessEvent(const SDL_Event& event);

  void Update(const float timeDelta);
};

enum class KeyboardType : Uint32 {
  KeyDown = 0,
  KeyUp = 1,
  KeyPress = 2,
};

class CKeyboardInputDevice
  : public CInputDevice {
private:
  Uint8 m_KeyState[SDL_NUM_SCANCODES];
  Uint8 m_KeyStatePrev[SDL_NUM_SCANCODES];

public:
  CKeyboardInputDevice();
  virtual ~CKeyboardInputDevice();

  // Inherited via CInputDevice
  virtual const bool GetState(const Uint32 type, const Uint32 id) const override;
  virtual const float GetRange(const Uint32 type, const Uint32 id) const override;
  virtual const bool ProcessEvent(const SDL_Event & event) override;
  virtual void Update(const float timeDelta) override;

private:
  const bool IsKeyDown(const Uint32 id) const;
  const bool IsKeyUp(const Uint32 id) const;
  const bool IsKeyPress(const Uint32 id) const;
};

enum class MouseType : Uint32 {
  ButtonDown = 0,
  ButtonUp = 1,
  ButtonPress = 2,
  AxisPos = 3,
  AxisDelta = 4
};

enum class MouseAxisId : Uint32 {
  AxisX,
  AxisY,
  AxisZ
};

class CMouseInputDevice
  : public CInputDevice {
private:
  glm::uvec2 mScreenSize;
  Uint32  mMouseButtonState;
  Uint32  mMouseButtonStatePrev;
  glm::ivec2  mMousePos;
  glm::ivec2  mMousePosPrev;

public:
  CMouseInputDevice(const glm::uvec2& screenSize);
  virtual ~CMouseInputDevice();

  // Inherited via CInputDevice
  virtual const bool GetState(const Uint32 type, const Uint32 id) const override;
  virtual const float GetRange(const Uint32 type, const Uint32 id) const override;
  virtual const bool ProcessEvent(const SDL_Event & event) override;
  virtual void Update(const float timeDelta) override;

private:
  const bool IsButtonDown(const Uint32 id) const;
  const bool IsButtonUp(const Uint32 id) const;
  const bool IsButtonPress(const Uint32 id) const;
  const float GetAxisPos(const Uint32 id) const;
  const float GetAxisDelta(const Uint32 id) const;
  const glm::ivec2 GetPos() const;
  const glm::ivec2 GetPosDelta() const;
};

#endif // !__BITRACE_INPUTDEVICE_H__

