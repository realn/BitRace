#include "Intro.h"

#include "../Common/Log.h"
#define GLEW_STATIC
#include <GL/glew.h>

CIntro::CIntro() :
  m_IntroState(IS_STATE1),
  m_uLogosTex(0),
  m_uCharCount(0),
  m_fTime(0.0f),
  m_bIntroEnd(false),
  m_strText1("PRESENTS"),
  m_strText2("A GAME BUILD WITH"),
  m_strText3("TECHNOLOGY") {

}

CIntro::~CIntro() {
  Free();
}

bool CIntro::Init(std::string strLogosFile) {
  if (!LoadTexture(strLogosFile))
    return false;
  return true;
}

void CIntro::Free() {
  if (glIsTexture(m_uLogosTex))
    glDeleteTextures(1, &m_uLogosTex);

  m_IntroState = 0;
  m_uLogosTex = 0;
  m_fTime = 0.0f;
}

void CIntro::Engine(float fDT) {
  switch (m_IntroState) {
  case IS_STATE1:
    m_fTime += 0.5f * fDT;
    if (m_fTime > 1.0f) {
      m_IntroState = IS_STATE2;
      m_fTime = 0.0f;
    }
    break;

  case IS_STATE2:
    m_fTime += 1.0f * fDT;
    if (m_fTime > 0.1f) {
      if (m_uCharCount < unsigned(m_strText1.length()))
        m_uCharCount++;
      else {
        m_IntroState = IS_STATE3;
        m_uCharCount = 0;
      }
      m_fTime = 0.0f;
    }
    break;

  case IS_STATE3:
    m_fTime += 1.0f * fDT;
    if (m_fTime > 3.0f) {
      m_IntroState = IS_STATE4;
      m_fTime = 0.0f;
    }
    break;

  case IS_STATE4:
    m_fTime += 1.0f * fDT;
    if (m_fTime > 0.1f) {
      if (m_uCharCount < unsigned(m_strText2.length()))
        m_uCharCount++;
      else {
        m_IntroState = IS_STATE5;
        m_uCharCount = 0;
      }
      m_fTime = 0.0f;
    }
    break;

  case IS_STATE5:
    m_fTime += 1.0f * fDT;
    if (m_fTime > 2.0f) {
      m_IntroState = IS_STATE6;
      m_fTime = 0.0f;
    }
    break;

  case IS_STATE6:
    m_fTime += 1.0f * fDT;
    if (m_fTime > 0.1f) {
      if (m_uCharCount < unsigned(m_strText3.length()))
        m_uCharCount++;
      else {
        m_IntroState = IS_STATE7;
        m_uCharCount = 0;
      }
      m_fTime = 0.0f;
    }
    break;

  case IS_STATE7:
    m_fTime += 0.8f * fDT;
    if (m_fTime > 1.0f) {
      m_IntroState = IS_STATE8;
      m_fTime = 0.0f;
    }
    break;

  case IS_STATE8:
    m_fTime += 0.8f * fDT;
    if (m_fTime > 1.0f) {
      m_IntroState = IS_STATE9;
      m_fTime = 0.0f;
    }
    break;

  case IS_STATE9:
    m_fTime += 1.0f * fDT;
    if (m_fTime > 1.0f) {
      m_IntroState = IS_STATE10;
      m_fTime = 0.0f;
    }
    break;

  case IS_STATE10:
    m_fTime += 1.0f * fDT;
    if (m_fTime > 1.0f) {
      m_IntroState = IS_STATE11;
      m_fTime = 0.0f;
    }
    break;

  case IS_STATE11:
    m_fTime += 0.25f * fDT;
    if (m_fTime > 1.0f) {
      m_IntroState = IS_STATE12;
      m_fTime = 0.0f;
    }
    break;

  case IS_STATE12:
    m_fTime += 1.0f * fDT;
    if (m_fTime > 2.0f) {
      m_IntroState = IS_STATE13;
      m_fTime = 0.0f;
    }
    break;

  case IS_STATE13:
    m_fTime += 1.2f * fDT;
    if (m_fTime > 1.0f) {
      m_IntroState = IS_STATE1;
      m_fTime = 0.0f;
      m_bIntroEnd = true;
    }
    break;
  };
}

void CIntro::Render() {

}

void CIntro::RenderGUI(CGUI *GUI) {
  switch (m_IntroState) {
  case IS_STATE1:
    glColor4f(1.0f, 1.0f, 1.0f, m_fTime);
    RenderLogo(0);
    break;

  case IS_STATE2:
    glColor3f(1.0f, 1.0f, 1.0f);
    RenderLogo(0);
    glColor3f(0.0f, 1.0f, 0.0f);
    GUI->Print(275.0f, 340.0f, m_strText1.substr(0, m_uCharCount) + "_");
    break;

  case IS_STATE3:
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - (m_fTime - 2.0f));
    RenderLogo(0);
    glColor3f(0.0f, 1.0f, 0.0f);
    GUI->Print(275.0f, 340.0f, m_strText1);
    break;

  case IS_STATE4:
    glColor3f(0.0f, 1.0f, 0.0f);
    GUI->Print(230.0f, 120.0f, m_strText2.substr(0, m_uCharCount) + "_");
    break;

  case IS_STATE5:
    glColor4f(1.0f, 1.0f, 1.0f, m_fTime);
    RenderLogo(3);
    glColor3f(0.0f, 1.0f, 0.0f);
    GUI->Print(230.0f, 120.0f, m_strText2);
    break;

  case IS_STATE6:
    glColor3f(1.0f, 1.0f, 1.0f);
    RenderLogo(3);
    glColor3f(0.0f, 1.0f, 0.0f);
    GUI->Print(230.0f, 120.0f, m_strText2);
    GUI->Print(270.0f, 340.0f, m_strText3.substr(0, m_uCharCount) + "_");
    break;

  case IS_STATE7:
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - m_fTime);
    RenderLogo(3);
    glColor3f(0.0f, 1.0f, 0.0f);
    GUI->Print(230.0f, 120.0f, m_strText2);
    GUI->Print(270.0f, 340.0f, m_strText3);
    break;

  case IS_STATE8:
    glColor4f(1.0f, 1.0f, 1.0f, m_fTime);
    RenderLogo(2);
    glColor3f(0.0f, 1.0f, 0.0f);
    GUI->Print(230.0f, 120.0f, m_strText2);
    GUI->Print(270.0f, 340.0f, m_strText3);
    break;

  case IS_STATE9:
    glColor3f(1.0f, 1.0f, 1.0f);
    RenderLogo(2);
    glColor3f(0.0f, 1.0f, 0.0f);
    GUI->Print(230.0f, 120.0f, m_strText2);
    GUI->Print(270.0f, 340.0f, m_strText3);
    break;

  case IS_STATE10:
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - m_fTime);
    RenderLogo(2);
    glColor3f(0.0f, 1.0f, 0.0f);
    GUI->Print(230.0f, 120.0f, m_strText2);
    GUI->Print(270.0f, 340.0f, m_strText3);
    break;

  case IS_STATE11:
    glColor4f(1.0f, 1.0f, 1.0f, m_fTime);
    RenderLogo(1);
    break;

  case IS_STATE12:
    glColor3f(1.0f, 1.0f, 1.0f);
    RenderLogo(1);
    break;

  case IS_STATE13:
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - m_fTime);
    RenderLogo(1);
    break;
  };
}

bool CIntro::IsIntroEnded() {
  return m_bIntroEnd;
}

bool CIntro::LoadTexture(std::string filename) {
  if (filename.empty())
    return false;

  CFGXFile imgFile;
  if (!imgFile.Load(filename)) {
    return false;
  }

  if (!imgFile.IsValid()) {
    return false;
  }

  glm::ivec2 size = imgFile.GetSize();
  const CFGXFile::CData& Data = imgFile.GetData();

  glGenTextures(1, &m_uLogosTex);
  glBindTexture(GL_TEXTURE_2D, m_uLogosTex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  unsigned format = 0;
  switch (imgFile.GetImgDepth()) {
  case 1: format = GL_LUMINANCE8;	break;
  case 2: format = GL_LUMINANCE8_ALPHA8;	break;
  case 3: format = GL_RGB;	break;
  case 4: format = GL_RGBA;	break;
  };

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, &Data[0]);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
  return true;
}

void CIntro::RenderLogo(unsigned int index) {
  glPushAttrib(GL_TEXTURE_BIT);
  glBindTexture(GL_TEXTURE_2D, m_uLogosTex);
  glBegin(GL_QUADS);
  switch (index) {
  case 0:
    glTexCoord2f(0.0f, 0.5f);
    glVertex2i(200, 120);
    glTexCoord2f(0.5f, 0.5f);
    glVertex2i(440, 120);
    glTexCoord2f(0.5f, 0.0f);
    glVertex2i(440, 360);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(200, 360);
    break;

  case 1:
    glTexCoord2f(0.5f, 0.5f);
    glVertex2i(120, 40);
    glTexCoord2f(1.0f, 0.5f);
    glVertex2i(520, 40);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(520, 440);
    glTexCoord2f(0.5f, 0.0f);
    glVertex2i(120, 440);
    break;

  case 2:
    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(200, 120);
    glTexCoord2f(0.5f, 1.0f);
    glVertex2i(440, 120);
    glTexCoord2f(0.5f, 0.5f);
    glVertex2i(440, 360);
    glTexCoord2f(0.0f, 0.5f);
    glVertex2i(200, 360);
    break;

  case 3:
    glTexCoord2f(0.5f, 1.0f);
    glVertex2i(200, 120);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(440, 120);
    glTexCoord2f(1.0f, 0.5f);
    glVertex2i(440, 360);
    glTexCoord2f(0.5f, 0.5f);
    glVertex2i(200, 360);
    break;
  };
  glEnd();
  glPopAttrib();
}