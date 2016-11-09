#include "Game.h"

CGame::~CGame() {
  Free();
  GInst = NULL;

  SDL_Quit();
}

void CGame::Free() {
  this->FreeGame();
  this->FreeOpenGL();
  this->FreeDevice();
  this->FreeWindow();
  Log_Free();
}

void CGame::FreeWindow() {
  if(this->m_pWindow) {
    SDL_DestroyWindow(this->m_pWindow);
    this->m_pWindow = nullptr;
  }
}

void CGame::FreeDevice() {
  if(this->m_pGLContext) {
    SDL_GL_DeleteContext(this->m_pGLContext);
    this->m_pGLContext = nullptr;
  }

  DXRELEASE(this->m_cDIMouse);
  DXRELEASE(this->m_cDIKey);
  DXRELEASE(this->m_cDInput);
  if (ScrParam.bFullscreen)
    ChangeDisplaySettings(NULL, 0);

  SDL_QuitSubSystem(SDL_INIT_VIDEO);
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