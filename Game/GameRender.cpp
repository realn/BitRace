#include "Game.h"

void CGame::Render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  if (m_bGamePause) {
    //if (ScrParam.bBlur) {
    //  glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
    //  glViewport(0, 0, m_uBlurTexSize, m_uBlurTexSize);
    //  glPushMatrix();
    //  glMatrixMode(GL_PROJECTION);
    //  glPushMatrix();
    //  glLoadIdentity();
    //  gluPerspective(50.0, ScrParam.GetAspectRatio(), 1.0, 50000.0);
    //  glMatrixMode(GL_MODELVIEW);
    //  glLoadIdentity();

    //  RenderGame();

    //  glBindTexture(GL_TEXTURE_2D, m_uBlurTexture);
    //  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0,
    //                   m_uBlurTexSize, m_uBlurTexSize, 0);

    //  glMatrixMode(GL_PROJECTION);
    //  glPopMatrix();
    //  glMatrixMode(GL_MODELVIEW);
    //  glPopMatrix();
    //  glPopAttrib();

    //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //}


    //if (ScrParam.bBlur) {
    //  m_cGUI.Begin(glm::vec2(1.0f, 1.0f));
    //  glBindTexture(GL_TEXTURE_2D, m_uBlurTexture);
    //  glColor4f(1.0f, 1.0f, 1.0f, m_fBlurTexAlpha);
    //  m_cGUI.RenderFSQuadTex(glm::vec2(1.0f, 1.0f));
    //  m_cGUI.End();
    //}
  }

  RenderGame();
	RenderGUI();

  if (m_bTakeScreen) {
    this->TakeScreenshot();
    m_bTakeScreen = false;
  }

  SDL_GL_SwapWindow(this->m_pWindow);
}

void CGame::RenderGame() {
  switch(m_uGameState) {
  case GS_INTRO:
    m_Intro.Render();
    break;

  case GS_MENU:
    if(m_bGamePause)
      m_RaceTrack.Render();
    break;

  case GS_GAME:
    m_RaceTrack.Render();
    break;

  default:
    break;
  }
}

void CGame::RenderGUI() {
  m_GUI.Begin(ScrParam.GetSize());
  switch (m_uGameState) {
  case GS_INTRO:
    m_Intro.RenderGUI();
    break;

  case GS_MENU:
    RenderMenu();
    break;

  case GS_GAME:
    m_RaceTrack.RenderGUI(&m_GUI);
    break;

  case GS_HIGH:
    m_HS.RenderGUI(&m_GUI);
    break;
  };
  if (ScrParam.bFPSCount && m_uGameState != GS_INTRO) {
    m_GUI.Print(glm::vec2(530.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), "FPS: %d", int(1.0f / (this->m_fDT != 0.0f ? this->m_fDT : 1.0f)));
  }
  m_GUI.End();
}

void CGame::RenderMenu() {
  if (m_bGamePause) {
    m_GUI.RenderQuadFullScreen(ScrParam.GetSize(), glm::vec4(0.0f, 0.0f, 0.0f, 0.4f));
  }
  m_MenuMng.Render();
}

void CGame::TakeScreenshot() {
  //std::vector<BYTE> Buffer;
  //Buffer.resize(ScrParam.uWidth * ScrParam.uHeight * 4);
  //glReadPixels(0, 0, ScrParam.uWidth, ScrParam.uHeight, GL_BGRA, GL_UNSIGNED_BYTE, &Buffer[0]);
  //int i = 0;

  //BITMAPFILEHEADER bfh;
  //BITMAPINFOHEADER bih;

  //bfh.bfType = MAKEWORD('B', 'M');
  //bfh.bfSize = DWORD(sizeof(bfh) + sizeof(bih) + Buffer.size());
  //bfh.bfReserved1 = 0;
  //bfh.bfReserved2 = 0;
  //bfh.bfOffBits = sizeof(bfh) + sizeof(bih);

  //bih.biSize = sizeof(BITMAPINFOHEADER);
  //bih.biWidth = ScrParam.uWidth;
  //bih.biHeight = ScrParam.uHeight;
  //bih.biBitCount = ScrParam.uColorBits;
  //bih.biPlanes = 1;
  //bih.biCompression = BI_RGB;
  //bih.biSizeImage = DWORD(Buffer.size());
  //bih.biXPelsPerMeter = 0;
  //bih.biYPelsPerMeter = 0;
  //bih.biClrUsed = 0;
  //bih.biClrImportant = 0;

  //CFile fp;
  //std::string filename = "";

  //CIniFile ini;
  //ini.Open(m_strConfigFile);
  //i = ini.ReadInt("GENERAL", "uScreenShotNum", 1);
  //std::string scrname = ini.ReadString("GENERAL", "strScreenShotName", "ScreenShot");
  //int len = _scprintf("%s%02d.bmp", scrname.c_str(), i);
  //filename.resize(len + 1);
  //sprintf(&filename[0], "%s%02d.bmp", scrname.c_str(), i);
  //ini.WriteInt("GENERAL", "uScreenShotNum", i + 1);
  //ini.Close();

  //fp.Open(filename, "wb");

  //fp.Write(&bfh, sizeof(bfh));
  //fp.Write(&bih, sizeof(bih));
  //fp.Write(&Buffer[0], sizeof(BYTE), unsigned(Buffer.size()));

  //fp.Close();
  //Buffer.clear();
}