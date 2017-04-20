#include "stdafx.h"
#include "Engine.h"

int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd) {
  CEngine Game;

  return Game.MainLoop(L"");
}