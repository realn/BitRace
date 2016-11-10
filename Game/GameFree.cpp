#include "Game.h"

void CGame::Free() {
  this->FreeGame();
  this->FreeOpenGL();
  this->FreeInput();
  this->FreeRender();
  this->FreeWindow();
  Log_Free();
}

void CGame::FreeWindow() {
  if(this->m_pWindow) {
    SDL_DestroyWindow(this->m_pWindow);
    this->m_pWindow = nullptr;
  }
}

void CGame::FreeRender() {
  if(this->m_pGLContext) {
    SDL_GL_DeleteContext(this->m_pGLContext);
    this->m_pGLContext = nullptr;
  }

  if (ScrParam.bFullscreen)
    ChangeDisplaySettings(NULL, 0);

  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void CGame::FreeInput() {
  DXRELEASE(this->m_cDIMouse);
  DXRELEASE(this->m_cDInput);

  SDL_QuitSubSystem(SDL_INIT_EVENTS);
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