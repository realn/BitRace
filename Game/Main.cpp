#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
  CGame Game;

  if (!Game.Init(lpCmdLine))
    return -1;

  return Game.MainLoop();
}