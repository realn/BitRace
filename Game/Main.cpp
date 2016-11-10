#include "Game.h"

int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd) {
  CGame Game;

  if (!Game.Init(lpCmdLine))
    return -1;

  return Game.MainLoop();
}