#include "stdafx.h"
#include "Engine.h"

int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd) {
  CEngine Game;

  if (!Game.Init(L""))
    return -1;

  return Game.MainLoop();
}