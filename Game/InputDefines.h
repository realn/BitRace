#ifndef __BITRACE_INPUTDEFINES_H__
#define __BITRACE_INPUTDEFINES_H__

#include <SDL_types.h>

enum class InputDevice {
  Keyboard = 0,
  Mouse = 1,
};

// SDL_ScanCode keys
enum class MouseEventId {
  AxisX = 0,
  AxisY,
  AxisZ,
  ButtonL,
  ButtonR,
  ButtonM,
  ButtonX1,
  ButtonX2,
  ButtonX3,
  ButtonX4,
};

enum class InputEventType {
  Action = 0,
  State = 1,
  Range = 2,
};

#endif // !__BITRACE_INPUTDEFINES_H__

