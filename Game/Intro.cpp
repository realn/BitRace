#include "Intro.h"
#include "GUI.h"
#include "GUIScreen.h"
#include "../Common/FGXFile.h"

#include "../Common/Log.h"
#define GLEW_STATIC
#include <GL/glew.h>

enum CIntro::INTRO_STATE {
  IS_STATE1 = 0,
  IS_STATE2,
  IS_STATE3,
  IS_STATE4,
  IS_STATE5,
  IS_STATE6,
  IS_STATE7,
  IS_STATE8,
  IS_STATE9,
  IS_STATE10,
  IS_STATE11,
  IS_STATE12,
  IS_STATE13
};

CIntro::CIntro(CGUI* pGUI, const glm::vec2& size) :
  m_pGUI(pGUI),
  m_pScreen(nullptr),
  m_pPresentTextItem(nullptr),
  m_pTechText1Item(nullptr),
  m_pTechText2Item(nullptr),
  m_pLogoItem(nullptr),
  m_pPresentAnim(nullptr),
  m_pTechText1Anim(nullptr),
  m_pTechText2Anim(nullptr),
  m_pLogoAnim(nullptr),
  m_pWaitTimer(nullptr),
  m_Size(size),
  m_IntroState(IS_STATE1),
  m_LogoTexId(0),
  m_uCharCount(0),
  m_IntroEnd(false),
  m_TextPresent("PRESENTS"),
  m_TextTech1("A GAME BUILD WITH"),
  m_TextTech2("TECHNOLOGY") {

}

CIntro::~CIntro() {
  Free();
}

bool CIntro::Init(const std::string& logoFilename, const glm::vec2& size) {
  if (!LoadTexture(logoFilename))
    return false;

  this->m_Size = size;
  this->m_pScreen = new CGUIScreen(m_pGUI, m_Size);

  this->m_pLogoItem = new CGUIScreenRectItem(m_pScreen, glm::vec2(0.0f), glm::vec2(240.0f));
  this->m_pLogoItem->SetAlign(CGUIScreenItem::IA_CENTER | CGUIScreenItem::IA_MIDDLE);
  this->m_pLogoItem->SetVisible(false);
  this->m_pLogoItem->SetTexture(this->m_LogoTexId);
  this->m_pLogoItem->SetTextureCoords(glm::vec2(0.0f), glm::vec2(0.5f));
  this->m_pLogoAnim = new CGUIFadeAnimation(m_pLogoItem, 1.0f);
  
  this->m_pPresentTextItem = new CGUIScreenTextItem(m_pScreen, glm::vec2(0.0f, 100.0f), m_TextPresent);
  this->m_pPresentTextItem->SetAlign(CGUIScreenItem::IA_CENTER | CGUIScreenItem::IA_MIDDLE);
  this->m_pPresentTextItem->SetVisible(false);
  this->m_pPresentTextItem->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  this->m_pPresentAnim = new CGUITextAnimation(m_pPresentTextItem, m_TextPresent, 0.6f);

  this->m_pTechText1Item = new CGUIScreenTextItem(m_pScreen, glm::vec2(0.0f, -100.0f), this->m_TextTech1);
  this->m_pTechText1Item->SetAlign(CGUIScreenItem::IA_CENTER | CGUIScreenItem::IA_MIDDLE);
  this->m_pTechText1Item->SetVisible(false);
  this->m_pTechText1Item->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  this->m_pTechText1Anim = new CGUITextAnimation(m_pTechText1Item, m_TextTech1, 0.6f);

  this->m_pTechText2Item = new CGUIScreenTextItem(m_pScreen, glm::vec2(0.0f, 100.0f), this->m_TextTech2);
  this->m_pTechText2Item->SetAlign(CGUIScreenItem::IA_CENTER | CGUIScreenItem::IA_MIDDLE);
  this->m_pTechText2Item->SetVisible(false);
  this->m_pTechText2Item->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  this->m_pTechText2Anim = new CGUITextAnimation(m_pTechText2Item, m_TextTech2, 0.6f);

  this->m_pScreen->AddItem(this->m_pLogoItem);
  this->m_pScreen->AddItem(this->m_pPresentTextItem);
  this->m_pScreen->AddItem(this->m_pTechText1Item);
  this->m_pScreen->AddItem(this->m_pTechText2Item);

  this->m_pLogoAnim->Show();

  this->m_pWaitTimer = new CGUITimer(1.0f);
  this->m_pWaitTimer->Stop();

  return true;
}

void CIntro::Free() {
  if (glIsTexture(m_LogoTexId))
    glDeleteTextures(1, &m_LogoTexId);

  delete m_pScreen;
  delete m_pPresentAnim;
  delete m_pLogoAnim;
  delete m_pTechText1Anim;
  delete m_pTechText2Anim;

  m_pScreen = nullptr;
  m_pPresentAnim = nullptr;
  m_pLogoAnim = nullptr;
  m_pTechText1Anim = nullptr;
  m_pTechText2Anim = nullptr;

  m_IntroState = 0;
  m_LogoTexId = 0;
}

void CIntro::Update(float timeDelta) {
  this->m_pPresentAnim->Update(timeDelta);
  this->m_pTechText1Anim->Update(timeDelta);
  this->m_pTechText2Anim->Update(timeDelta);
  this->m_pLogoAnim->Update(timeDelta);
  this->m_pWaitTimer->Update(timeDelta);

  switch (m_IntroState) {
  case IS_STATE1:
    if(!m_pLogoAnim->IsAnimating()) {
      m_IntroState++;
      m_pPresentAnim->Show();
    }
    break;

  case IS_STATE2:
    if(!m_pPresentAnim->IsAnimating()) {
      m_IntroState++;
      m_pWaitTimer->Start();
    }
    break;

  case IS_STATE3:
    if(m_pWaitTimer->IsDone()) {
      m_IntroState++;
      m_pPresentAnim->Hide();
      m_pLogoAnim->Hide();
    }
    break;

  case IS_STATE4:
    if(!m_pPresentAnim->IsAnimating() && !m_pLogoAnim->IsAnimating()) {
      m_IntroState++;
      m_pTechText1Anim->Show();
      m_pTechText2Anim->Show();
    }
    break;

  case IS_STATE5:
    if(!m_pTechText1Anim->IsAnimating() && !m_pTechText2Anim->IsAnimating()) {
      m_IntroState++;
      m_pLogoItem->SetTextureCoords(glm::vec2(0.0f, 0.5f), glm::vec2(0.5f));
      m_pLogoAnim->Show();
    }
    break;

  case IS_STATE6:
    if(!m_pLogoAnim->IsAnimating()) {
      m_IntroState++;
      m_pLogoAnim->Hide();
    }
    break;

  case IS_STATE7:
    if(!m_pLogoAnim->IsAnimating()) {
      m_IntroState++;
      m_pLogoItem->SetTextureCoords(glm::vec2(0.5f, 0.5f), glm::vec2(0.5f));
      m_pLogoAnim->Show();
    }
    break;

  case IS_STATE8:
    if(!m_pLogoAnim->IsAnimating()) {
      m_IntroState++;
      m_pLogoAnim->Hide();
      m_pTechText1Anim->Hide();
      m_pTechText2Anim->Hide();
    }
    break;

  case IS_STATE9:
    if(!m_pLogoAnim->IsAnimating() && !m_pTechText1Anim->IsAnimating() && !m_pTechText2Anim->IsAnimating()) {
      m_IntroState++;
      m_pLogoItem->SetTextureCoords(glm::vec2(0.5f, 0.0f), glm::vec2(0.5f));
      m_pLogoAnim->Show();
    }
    break;

  case IS_STATE10:
    if(!m_pLogoAnim->IsAnimating()) {
      m_IntroState++;
      m_pWaitTimer->Start();
    }
    break;

  case IS_STATE11:
    if(m_pWaitTimer->IsDone()) {
      m_IntroState++;
      m_pLogoAnim->Hide();
    }
    break;

  case IS_STATE12:
    if(!m_pLogoAnim->IsAnimating()) {
      m_IntroState = 0;
      m_IntroEnd = true;
    }
    break;
  };
}

void CIntro::Render() {

}

void CIntro::RenderGUI() {
  this->m_pScreen->Render();
}

bool CIntro::IsIntroEnded() {
  return m_IntroEnd;
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

  glGenTextures(1, &m_LogoTexId);
  glBindTexture(GL_TEXTURE_2D, m_LogoTexId);
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