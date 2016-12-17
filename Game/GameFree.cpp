#include "Game.h"
#include "GameView.h"
#include "Input.h"
#include "Log.h"
#include "helper.h"

#include <SDL.h>

void CGame::Free() {
  this->FreeGame();
  this->FreeOpenGL();

  helper::deleteobj(m_pInput);

  if(m_pView) {
    m_pView->Destroy();
  }

  helper::deleteobj(m_pView);
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