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
  m_pPresentTextControl(nullptr),
  m_pTechText1Control(nullptr),
  m_pTechText2Control(nullptr),
  m_pLogoControl(nullptr),
  m_pControllerList(nullptr),
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
  glm::vec4 textColor(0.0f, 1.0f, 0.0f, 1.0f);

  this->m_pLogoControl = new CGUIRectControl(m_pScreen, glm::vec2(240.0f));
  this->m_pLogoControl->SetVisible(false);
  this->m_pLogoControl->SetTexture(this->m_LogoTexId);
  this->m_pLogoControl->SetTextureCoords(glm::vec2(0.0f), glm::vec2(0.5f));

  this->m_pPresentTextControl = new CGUITextControl(m_pScreen, m_TextPresent, textColor);
  this->m_pPresentTextControl->SetVisible(false);

  this->m_pTechText1Control = new CGUITextControl(m_pScreen, m_TextTech1, textColor);
  this->m_pTechText1Control->SetVisible(false);

  this->m_pTechText2Control = new CGUITextControl(m_pScreen, this->m_TextTech2, textColor);
  this->m_pTechText2Control->SetVisible(false);

  CGUIRectControl* pTechLogo1Control = new CGUIRectControl(m_pScreen, glm::vec2(240.0f));
  pTechLogo1Control->SetVisible(false);
  pTechLogo1Control->SetTexture(this->m_LogoTexId);
  pTechLogo1Control->SetTextureCoords(glm::vec2(0.0f, 0.5f), glm::vec2(0.5f));

  CGUIRectControl* pTechLogo2Control = new CGUIRectControl(m_pScreen, glm::vec2(240.0f));
  pTechLogo2Control->SetVisible(false);
  pTechLogo2Control->SetTexture(this->m_LogoTexId);
  pTechLogo2Control->SetTextureCoords(glm::vec2(0.5f, 0.5f), glm::vec2(0.5f));

  this->m_pScreen->AddControl(m_pLogoControl, glm::vec2(0.0f), CGUIScreen::IA_CENTER | CGUIScreen::IA_MIDDLE);
  this->m_pScreen->AddControl(pTechLogo1Control, glm::vec2(0.0f), CGUIScreen::IA_CENTER | CGUIScreen::IA_MIDDLE);
  this->m_pScreen->AddControl(pTechLogo2Control, glm::vec2(0.0f), CGUIScreen::IA_CENTER | CGUIScreen::IA_MIDDLE);
  this->m_pScreen->AddControl(m_pPresentTextControl, glm::vec2(0.0f, 100.0f), CGUIScreen::IA_CENTER | CGUIScreen::IA_MIDDLE);
  this->m_pScreen->AddControl(m_pTechText1Control, glm::vec2(0.0f, -100.0f), CGUIScreen::IA_CENTER | CGUIScreen::IA_MIDDLE);
  this->m_pScreen->AddControl(m_pTechText2Control, glm::vec2(0.0f, 100.0f), CGUIScreen::IA_CENTER | CGUIScreen::IA_MIDDLE);

  this->m_pControllerList = new CGUIControllerList();
  CGUIControllerList* pList = m_pControllerList;

  {
    m_pIntroStart = new CGUIFadeAnimation(pList, m_pLogoControl, 1.0f);
    m_pIntroStart->Start();

    IGUIController* pTextShowAnim = new CGUITextAnimation(pList, m_pPresentTextControl, 0.6f);
    CGUIDoneTrigger* pTrigger = new CGUIDoneTrigger(pList, m_pIntroStart, pTextShowAnim);

    IGUIController* pWaitTimer = new CGUITimer(pList, 1.0f);
    pTrigger = new CGUIDoneTrigger(pList, pTextShowAnim, pWaitTimer);

    IGUIController* pTextHideAnim = new CGUITextAnimation(pList, m_pPresentTextControl, 0.6f, false);
    IGUIController* pFadeHideAnim = new CGUIFadeAnimation(pList, m_pLogoControl, 1.0f, false);

    pTrigger = new CGUIDoneTrigger(pList, pWaitTimer);
    pTrigger->AddTarget(pTextHideAnim);
    pTrigger->AddTarget(pFadeHideAnim);

    IGUIController* pTextShowAnim1 = new CGUITextAnimation(pList, m_pTechText1Control, 0.6f);
    IGUIController* pTextShowAnim2 = new CGUITextAnimation(pList, m_pTechText2Control, 0.6f);

    pTrigger = new CGUIDoneTrigger(pList);
    pTrigger->AddCondition(pTextHideAnim);
    pTrigger->AddCondition(pFadeHideAnim);
    pTrigger->AddTarget(pTextShowAnim1);
    pTrigger->AddTarget(pTextShowAnim2);

    IGUIController* pFadeShowAnim = new CGUIFadeAnimation(pList, pTechLogo1Control, 1.0f);

    pTrigger = new CGUIDoneTrigger(pList);
    pTrigger->AddCondition(pTextShowAnim1);
    pTrigger->AddCondition(pTextShowAnim2);
    pTrigger->AddTarget(pFadeShowAnim);

    pFadeHideAnim = new CGUIFadeAnimation(pList, pTechLogo1Control, 1.0f, false);
    pTrigger = new CGUIDoneTrigger(pList, pFadeShowAnim, pFadeHideAnim);

    pFadeShowAnim = new CGUIFadeAnimation(pList, pTechLogo2Control, 1.0f);
    pTrigger = new CGUIDoneTrigger(pList, pFadeHideAnim, pFadeShowAnim);


    pFadeHideAnim = new CGUIFadeAnimation(pList, pTechLogo2Control, 1.0f, false);
    IGUIController* pTextHideAnim1 = new CGUITextAnimation(pList, m_pTechText1Control, 0.6f, false);
    IGUIController* pTextHideAnim2 = new CGUITextAnimation(pList, m_pTechText2Control, 0.6f, false);
    pTrigger = new CGUIDoneTrigger(pList, pFadeShowAnim);
    pTrigger->AddTarget(pFadeHideAnim);
    pTrigger->AddTarget(pTextHideAnim1);
    pTrigger->AddTarget(pTextHideAnim2);

    pTrigger = new CGUIDoneTrigger(pList);
    pTrigger->AddCondition(pFadeHideAnim);
    pTrigger->AddCondition(pTextHideAnim1);
    pTrigger->AddCondition(pTextHideAnim2);

    m_pIntroEnd = pTrigger;
  }

  return true;
}

void CIntro::Free() {
  if (glIsTexture(m_LogoTexId))
    glDeleteTextures(1, &m_LogoTexId);

  delete m_pControllerList;
  delete m_pScreen;

  m_pControllerList = nullptr;
  m_pScreen = nullptr;
 
  m_IntroState = 0;
  m_LogoTexId = 0;
}

void CIntro::Update(float timeDelta) {
  this->m_pControllerList->Update(timeDelta);

  if(m_pIntroEnd->IsDone()) {
    m_IntroEnd = true;
  }
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