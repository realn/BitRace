#include "stdafx.h"
#include "Intro.h"
#include "FGXFile.h"
#include "GLDefines.h"
#include "GUI.h"

static const std::string INTRO_TEXT1 = "PRESENTS";
static const std::string INTRO_TEXT2 = "A GAME BUILD WITH";
static const std::string INTRO_TEXT3 = "TECHNOLOGY";

CIntroProcess::CIntroProcess() :
  mIntroState(IntroState::IS_STATE1),
  mCharCount(0),
  mTime(0.0f),
  mIntroEnd(false){
}

CIntroProcess::~CIntroProcess() {
  mIntroState = IntroState::IS_STATE1;
  mTime = 0.0f;
}

void CIntroProcess::Update(const float fDT) {
  switch (mIntroState) {
  case IntroState::IS_STATE1:
    mTime += 0.5f * fDT;
    if (mTime > 1.0f) {
      mIntroState = IntroState::IS_STATE2;
      mTime = 0.0f;
    }
    break;

  case IntroState::IS_STATE2:
    mTime += 1.0f * fDT;
    if (mTime > 0.1f) {
      if (mCharCount < unsigned(INTRO_TEXT1.length()))
        mCharCount++;
      else {
        mIntroState = IntroState::IS_STATE3;
        mCharCount = 0;
      }
      mTime = 0.0f;
    }
    break;

  case IntroState::IS_STATE3:
    mTime += 1.0f * fDT;
    if (mTime > 3.0f) {
      mIntroState = IntroState::IS_STATE4;
      mTime = 0.0f;
    }
    break;

  case IntroState::IS_STATE4:
    mTime += 1.0f * fDT;
    if (mTime > 0.1f) {
      if (mCharCount < unsigned(INTRO_TEXT2.length()))
        mCharCount++;
      else {
        mIntroState = IntroState::IS_STATE5;
        mCharCount = 0;
      }
      mTime = 0.0f;
    }
    break;

  case IntroState::IS_STATE5:
    mTime += 1.0f * fDT;
    if (mTime > 2.0f) {
      mIntroState = IntroState::IS_STATE6;
      mTime = 0.0f;
    }
    break;

  case IntroState::IS_STATE6:
    mTime += 1.0f * fDT;
    if (mTime > 0.1f) {
      if (mCharCount < unsigned(INTRO_TEXT3.length()))
        mCharCount++;
      else {
        mIntroState = IntroState::IS_STATE7;
        mCharCount = 0;
      }
      mTime = 0.0f;
    }
    break;

  case IntroState::IS_STATE7:
    mTime += 0.8f * fDT;
    if (mTime > 1.0f) {
      mIntroState = IntroState::IS_STATE8;
      mTime = 0.0f;
    }
    break;

  case IntroState::IS_STATE8:
    mTime += 0.8f * fDT;
    if (mTime > 1.0f) {
      mIntroState = IntroState::IS_STATE9;
      mTime = 0.0f;
    }
    break;

  case IntroState::IS_STATE9:
    mTime += 1.0f * fDT;
    if (mTime > 1.0f) {
      mIntroState = IntroState::IS_STATE10;
      mTime = 0.0f;
    }
    break;

  case IntroState::IS_STATE10:
    mTime += 1.0f * fDT;
    if (mTime > 1.0f) {
      mIntroState = IntroState::IS_STATE11;
      mTime = 0.0f;
    }
    break;

  case IntroState::IS_STATE11:
    mTime += 0.25f * fDT;
    if (mTime > 1.0f) {
      mIntroState = IntroState::IS_STATE12;
      mTime = 0.0f;
    }
    break;

  case IntroState::IS_STATE12:
    mTime += 1.0f * fDT;
    if (mTime > 2.0f) {
      mIntroState = IntroState::IS_STATE13;
      mTime = 0.0f;
    }
    break;

  case IntroState::IS_STATE13:
    mTime += 1.2f * fDT;
    if (mTime > 1.0f) {
      mIntroState = IntroState::IS_STATE1;
      mTime = 0.0f;
      mIntroEnd = true;
    }
    break;
  };
}

const IntroState CIntroProcess::GetState() const {
  return mIntroState;
}

const float CIntroProcess::GetTime() const {
  return mTime;
}

const Uint32 CIntroProcess::GetCharNumber() const {
  return mCharCount;
}

bool CIntroProcess::IsIntroEnded() {
  return mIntroEnd;
}

CIntroView::CIntroView(CIntroProcess& intro) 
  : mIntro(intro)
  , mLogosTex(0)
{}

CIntroView::~CIntroView() {}

const bool CIntroView::Init(const cb::string & logosFilePath) {
  if(!LoadTexture(logosFilePath))
    return false;
  return true;
}

void CIntroView::Free() {
  if(glIsTexture(mLogosTex))
    glDeleteTextures(1, &mLogosTex);

  mLogosTex = 0;
}

void CIntroView::Render() const {}

void CIntroView::RenderUI(CGUI& gui) const {
  switch(mIntro.GetState()) {
  case IntroState::IS_STATE1:
    glColor4f(1.0f, 1.0f, 1.0f, mIntro.GetTime());
    RenderLogo(0);
    break;

  case IntroState::IS_STATE2:
    glColor3f(1.0f, 1.0f, 1.0f);
    RenderLogo(0);
    glColor3f(0.0f, 1.0f, 0.0f);
    gui.Print(glm::vec2(275.0f, 340.0f), INTRO_TEXT1.substr(0, mIntro.GetCharNumber()) + "_");
    break;

  case IntroState::IS_STATE3:
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - (mIntro.GetTime() - 2.0f));
    RenderLogo(0);
    glColor3f(0.0f, 1.0f, 0.0f);
    gui.Print(glm::vec2(275.0f, 340.0f), INTRO_TEXT1);
    break;

  case IntroState::IS_STATE4:
    glColor3f(0.0f, 1.0f, 0.0f);
    gui.Print(glm::vec2(230.0f, 120.0f), INTRO_TEXT2.substr(0, mIntro.GetCharNumber()) + "_");
    break;

  case IntroState::IS_STATE5:
    glColor4f(1.0f, 1.0f, 1.0f, mIntro.GetTime());
    RenderLogo(3);
    glColor3f(0.0f, 1.0f, 0.0f);
    gui.Print(glm::vec2(230.0f, 120.0f), INTRO_TEXT2);
    break;

  case IntroState::IS_STATE6:
    glColor3f(1.0f, 1.0f, 1.0f);
    RenderLogo(3);
    glColor3f(0.0f, 1.0f, 0.0f);
    gui.Print(glm::vec2(230.0f, 120.0f), INTRO_TEXT2);
    gui.Print(glm::vec2(270.0f, 340.0f), INTRO_TEXT3.substr(0, mIntro.GetCharNumber()) + "_");
    break;

  case IntroState::IS_STATE7:
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - mIntro.GetTime());
    RenderLogo(3);
    glColor3f(0.0f, 1.0f, 0.0f);
    gui.Print(glm::vec2(230.0f, 120.0f), INTRO_TEXT2);
    gui.Print(glm::vec2(270.0f, 340.0f), INTRO_TEXT3);
    break;

  case IntroState::IS_STATE8:
    glColor4f(1.0f, 1.0f, 1.0f, mIntro.GetTime());
    RenderLogo(2);
    glColor3f(0.0f, 1.0f, 0.0f);
    gui.Print(glm::vec2(230.0f, 120.0f), INTRO_TEXT2);
    gui.Print(glm::vec2(270.0f, 340.0f), INTRO_TEXT3);
    break;

  case IntroState::IS_STATE9:
    glColor3f(1.0f, 1.0f, 1.0f);
    RenderLogo(2);
    glColor3f(0.0f, 1.0f, 0.0f);
    gui.Print(glm::vec2(230.0f, 120.0f), INTRO_TEXT2);
    gui.Print(glm::vec2(270.0f, 340.0f), INTRO_TEXT3);
    break;

  case IntroState::IS_STATE10:
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - mIntro.GetTime());
    RenderLogo(2);
    glColor3f(0.0f, 1.0f, 0.0f);
    gui.Print(glm::vec2(230.0f, 120.0f), INTRO_TEXT2);
    gui.Print(glm::vec2(270.0f, 340.0f), INTRO_TEXT3);
    break;

  case IntroState::IS_STATE11:
    glColor4f(1.0f, 1.0f, 1.0f, mIntro.GetTime());
    RenderLogo(1);
    break;

  case IntroState::IS_STATE12:
    glColor3f(1.0f, 1.0f, 1.0f);
    RenderLogo(1);
    break;

  case IntroState::IS_STATE13:
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - mIntro.GetTime());
    RenderLogo(1);
    break;
  };
}

const bool CIntroView::LoadTexture(const cb::string & filepath) {
  if(filepath.empty())
    return false;

  CFGXFile imgFile;
  if(!imgFile.Load(filepath)) {
    return false;
  }

  if(!imgFile.IsValid()) {
    return false;
  }

  glm::ivec2 size = imgFile.GetSize();
  const cb::bytevector& Data = imgFile.GetData();

  glGenTextures(1, &mLogosTex);
  glBindTexture(GL_TEXTURE_2D, mLogosTex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  unsigned format = 0;
  switch(imgFile.GetImgDepth()) {
  case 1: format = GL_LUMINANCE8;	break;
  case 2: format = GL_LUMINANCE8_ALPHA8;	break;
  case 3: format = GL_RGB;	break;
  case 4: format = GL_RGBA;	break;
  };

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, cb::vectorptr(Data));
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
  return true;
}

void CIntroView::RenderLogo(Uint32 index) const {
  glPushAttrib(GL_TEXTURE_BIT);
  glBindTexture(GL_TEXTURE_2D, mLogosTex);
  glBegin(GL_QUADS);
  switch(index) {
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
