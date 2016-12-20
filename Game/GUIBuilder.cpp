#include "stdafx.h"
#include "GUIBuilder.h"
#include "GUIScreen.h"
#include "GUIController.h"
#include "helper.h"

CGUIBuilder::CContext::CContext(CGUIControllerList* pList) : 
  m_pList(pList), m_pBegin(nullptr), m_pEnd(nullptr) {}

CGUIBuilder::CContext::~CContext() {}

CGUIController * CGUIBuilder::CContext::GetBegin() const {
  return m_pBegin;
}

CGUIController * CGUIBuilder::CContext::GetEnd() const {
  return m_pEnd;
}




CGUIBuilder::CSequenceContext::CSequenceContext(CGUIControllerList* pList) :
  CContext(pList) {}

CGUIBuilder::CSequenceContext::~CSequenceContext() {}

void CGUIBuilder::CSequenceContext::AddController(CGUIController * pBegin, CGUIController * pEnd) {
  if(m_pBegin == nullptr) {
    m_pBegin = pBegin;
  }
  else {
    CGUIDoneTrigger* pDone = new CGUIDoneTrigger(m_pList, m_pEnd, pBegin);
  }
  m_pEnd = pEnd;
}

CGUIBuilder::CParallelContext::CParallelContext(CGUIControllerList * pList) :
  CContext(pList), m_pTriggerBegin(nullptr), m_pTriggerEnd(nullptr) {}

CGUIBuilder::CParallelContext::~CParallelContext() {}

void CGUIBuilder::CParallelContext::AddController(CGUIController * pBegin, CGUIController * pEnd) {
  if(m_pBegin == nullptr) {
    m_pBegin = pBegin;
  }
  else if(m_pTriggerBegin == nullptr) {
    m_pTriggerBegin = new CGUIDoneTrigger(m_pList);
    m_pTriggerBegin->AddTarget(m_pBegin);
    m_pTriggerBegin->AddTarget(pBegin);
    m_pBegin = m_pTriggerBegin;
  }
  else {
    m_pTriggerBegin->AddTarget(pBegin);
  }

  if(m_pEnd == nullptr) {
    m_pEnd = pEnd;
  }
  else if(m_pTriggerEnd == nullptr) {
    m_pTriggerEnd = new CGUIDoneTrigger(m_pList);
    m_pTriggerEnd->AddCondition(m_pEnd);
    m_pTriggerEnd->AddCondition(pEnd);
    m_pEnd = m_pTriggerEnd;
  }
  else {
    m_pTriggerEnd->AddCondition(pEnd);
  }
}



CGUIBuilder::CGUIBuilder(CGUIScreen * pScreen, CGUIControllerList * pControllerList) :
  m_pScreen(pScreen), m_pList(pControllerList), m_pCurrentContext(nullptr)
{
  m_pCurrentContext = new CSequenceContext(m_pList);
}

CGUIBuilder::~CGUIBuilder() {
  helper::deletevector(m_ContextStack);
  delete m_pCurrentContext;
}

CGUIController * CGUIBuilder::GetFirst() const {
  if(m_pCurrentContext)
    return m_pCurrentContext->GetBegin();
  return nullptr;
}

CGUIController * CGUIBuilder::GetLast() const {
  if(m_pCurrentContext)
    return m_pCurrentContext->GetEnd();
  return nullptr;
}

void CGUIBuilder::BeginSequence() {
  this->BeginBlock(new CSequenceContext(m_pList));
}

void CGUIBuilder::EndSequence() {
  this->EndBlock();
}

void CGUIBuilder::BeginParallel() {
  this->BeginBlock(new CParallelContext(m_pList));
}

void CGUIBuilder::EndParallel() {
  this->EndBlock();
}

void CGUIBuilder::Color(const std::string & name, const glm::vec4 & color) {
  this->m_Colors[name] = color;
}

void CGUIBuilder::Color(const std::string & name, const float r, const float g, const float b, const float a) {
  this->Color(name, glm::vec4(r, g, b, a));
}

void CGUIBuilder::Texture(const std::string & name, const Uint32 texId) {
  m_Textures[name] = texId;
}

void CGUIBuilder::Label(const std::string & name, const std::string & text) {
  Label(name, text, glm::vec4(1.0f));
}

void CGUIBuilder::Label(const std::string & name, const std::string & text, const glm::vec4 & color) {
  CGUITextControl* pControl = new CGUITextControl(text, color);

  this->m_Controls[name] = pControl;
}

void CGUIBuilder::Label(const std::string & name, const std::string & text, const std::string & color) {
  this->Label(name, text, m_Colors[color]);
}

void CGUIBuilder::Rect(const std::string & name, const glm::vec2 & size) {
  this->Rect(name, size, glm::vec4(1.0f));
}

void CGUIBuilder::Rect(const std::string & name, const glm::vec2 & size, const glm::vec4 & color) {
  CGUIRectControl* pControl = new CGUIRectControl(size, color);

  this->m_Controls[name] = pControl;
}

void CGUIBuilder::Rect(const std::string & name, const glm::vec2 & size, const std::string & color) {
  this->Rect(name, size, m_Colors[color]);
}

void CGUIBuilder::Put(const std::string & name, const glm::vec2 & pos, const Uint32 align) {
  CGUIControl* pControl = m_Controls[name];
  m_pScreen->AddControl(pControl, pos, align);
}

void CGUIBuilder::SetVisible(const std::string & name, const bool value) {
  CGUIControl* pControl = m_Controls[name];
  pControl->SetVisible(value);
}

void CGUIBuilder::SetRectTexture(const std::string & name, const std::string & texName, const glm::vec2 & pos, const glm::vec2 & size) {
  CGUIRectControl* pControl = GetControl<CGUIRectControl>(name);
  pControl->SetTexture(m_Textures[texName]);
  pControl->SetTextureCoords(pos, size);
}

void CGUIBuilder::TextShow(const std::string & name, const float time, const std::string & showText) {
  CGUITextControl* pControl = GetControl<CGUITextControl>(name);
  CGUITextAnimation* pAnim = new CGUITextAnimation(m_pList, pControl, showText, time, true);
  this->AddController(pAnim);
}

void CGUIBuilder::TextShow(const std::string & name, const float time) {
  CGUITextControl* pControl = GetControl<CGUITextControl>(name);
  CGUITextAnimation* pAnim = new CGUITextAnimation(m_pList, pControl, time, true);
  this->AddController(pAnim);
}

void CGUIBuilder::TextHide(const std::string & name, const float time, const std::string & text) {
  CGUITextControl* pControl = GetControl<CGUITextControl>(name);
  CGUITextAnimation* pAnim = new CGUITextAnimation(m_pList, pControl, text, time, false);
  this->AddController(pAnim);
}

void CGUIBuilder::TextHide(const std::string & name, const float time) {
  CGUITextControl* pControl = GetControl<CGUITextControl>(name);
  CGUITextAnimation* pAnim = new CGUITextAnimation(m_pList, pControl, time, false);
  this->AddController(pAnim);
}

void CGUIBuilder::FadeIn(const std::string & name, const float time, const float min, const float max) {
  CGUIControl* pControl = m_Controls[name];
  CGUIAlphaFadeAnimation* pAnim = new CGUIAlphaFadeAnimation(m_pList, pControl, time, min, max, true);
  this->AddController(pAnim);
}

void CGUIBuilder::FadeOut(const std::string & name, const float time, const float min, const float max) {
  CGUIControl* pControl = m_Controls[name];
  CGUIAlphaFadeAnimation* pAnim = new CGUIAlphaFadeAnimation(m_pList, pControl, time, min, max, false);
  this->AddController(pAnim);
}

void CGUIBuilder::BlendIn(const std::string & name, const float time, const glm::vec4 & min, const glm::vec4 & max) {
  CGUIControl* pControl = m_Controls[name];
  CGUIColorFadeAnimation* pAnim = new CGUIColorFadeAnimation(m_pList, pControl, time, min, max, true);
  this->AddController(pAnim);
}

void CGUIBuilder::BlendOut(const std::string & name, const float time, const glm::vec4 & min, const glm::vec4 & max) {
  CGUIControl* pControl = m_Controls[name];
  CGUIColorFadeAnimation* pAnim = new CGUIColorFadeAnimation(m_pList, pControl, time, min, max, false);
  this->AddController(pAnim);
}

void CGUIBuilder::Wait(const float time) {
  CGUIController* pTimer = new CGUITimer(m_pList, time);
  this->AddController(pTimer);
}

void CGUIBuilder::BeginBlock(CContext * pNewContext) {
  m_ContextStack.push_back(m_pCurrentContext);
  m_pCurrentContext = pNewContext;
}

void CGUIBuilder::EndBlock() {
  if(m_ContextStack.empty())
    return;

  CContext* pContext = *m_ContextStack.rbegin();
  pContext->AddController(m_pCurrentContext->GetBegin(),
                          m_pCurrentContext->GetEnd());

  m_ContextStack.pop_back();
  delete m_pCurrentContext;
  m_pCurrentContext = pContext;
}

void CGUIBuilder::AddController(CGUIController * pController) {
  m_pCurrentContext->AddController(pController, pController);
}

