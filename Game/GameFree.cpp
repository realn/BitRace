#include "Game.h"
#include "GameView.h"
#include "Log.h"

#include <SDL.h>

void CGame::Free() {
  this->FreeGame();
  this->FreeOpenGL();
  this->FreeInput();

  if(m_pView) {
    m_pView->Destroy();
    delete m_pView;
    m_pView = nullptr;
  }

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