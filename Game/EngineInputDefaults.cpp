#include "stdafx.h"
#include "InputBindings.h"
#include "InputSystem.h"
#include "GameInputDefines.h"

const bool CreateDefaultInputBindings(CInputSystem& input) {
  input.SetContextEnabled(INPUT_CTX_GAME, true);
  input.SetContextEnabled(INPUT_CTX_PLAYER, true);
  input.SetContextEnabled(INPUT_CTX_MENU, true);

  {
    CInputBindings ctx;

    ctx.AddTarget(INPUT_TGT_PLAYER_ROTATEX);
    ctx.AddTarget(INPUT_TGT_PLAYER_ATTACK);

    ctx.Bind(INPUT_TGT_PLAYER_ROTATEX, InputDeviceType::Mouse, 
             INPUT_ALL_DEVICES, (Uint32)InputMouseEventId::AxisX);
    ctx.Bind(INPUT_TGT_PLAYER_ATTACK, InputDeviceType::Mouse, 
             INPUT_ALL_DEVICES, (Uint32)InputMouseEventId::ButtonL);
    ctx.Bind(INPUT_TGT_PLAYER_ATTACK, InputDeviceType::Keyboard,
             INPUT_ALL_DEVICES, SDL_SCANCODE_SPACE);

    input.SetContextBindings(INPUT_CTX_PLAYER, ctx);
  }

  return true;
}