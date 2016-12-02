#include "Game.h"
#include "Log.h"

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

  if (ScrParam.bFullscreen) {
    SDL_SetWindowDisplayMode(this->m_pWindow, &this->m_ModeOryginal);
  }

  SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void CGame::FreeInput() {
  SDL_SetRelativeMouseMode(SDL_FALSE);
  SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void CGame::FreeOpenGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  if (glIsTexture(m_uBlurTexture))
    glDeleteTextures(1, &m_uBlurTexture);
}

void CGame::FreeGame() {
  m_RaceTrack.Free();
  m_MenuMng.Clear();
  m_GUI.Free();
  m_Intro.Free();
}