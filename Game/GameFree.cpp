#include "Game.h"

CGame::~CGame() {
  Free();
  GInst = NULL;
}

void CGame::Free() {
  this->FreeGame();
  this->FreeOpenGL();
  this->FreeDevice();
  this->FreeWindow();
  Log_Free();
}

void CGame::FreeWindow() {
  if (this->m_hWnd != NULL) {
    if (IsWindow(this->m_hWnd)) {
      DestroyWindow(this->m_hWnd);
    }
    this->m_hWnd = NULL;
  }
  WNDCLASSEX wc = { 0 };
  if (GetClassInfoEx(GetModuleHandle(NULL), GAME_WCLASS, &wc)) {
    UnregisterClass(GAME_WCLASS, GetModuleHandle(NULL));
  }
}

void CGame::FreeDevice() {
  if (this->m_cGLDevice.IsEnabled())
    this->m_cGLDevice.DisableGL();

  DXRELEASE(this->m_cDIMouse);
  DXRELEASE(this->m_cDIKey);
  DXRELEASE(this->m_cDInput);
  if (ScrParam.bFullscreen)
    ChangeDisplaySettings(NULL, 0);
}

void CGame::FreeOpenGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  if (glIsTexture(m_uBlurTexture))
    glDeleteTextures(1, &m_uBlurTexture);
}

void CGame::FreeGame() {
  m_RaceTrack.Free();
  m_Racer.Free();
  m_MMag.Clear();
  m_cGUI.Free();
  m_Intro.Free();
  CModel::FreeModels();
}