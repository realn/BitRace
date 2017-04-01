#include "stdafx.h"
#include "GUIBuilder.h"
#include "GUIScreen.h"
#include "GUIInstructions.h"
#include "helper.h"


CGUIBuilder::CGUIBuilder(CGUIScreen * pScreen) :
  m_pScreen(pScreen)
{
}

CGUIBuilder::~CGUIBuilder() {
}

const glm::vec4 CGUIBuilder::GetColor(const std::string & name) const {
  colormap::const_iterator it = m_Colors.find(name);
  if(it != m_Colors.end())
    return it->second;
  return glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // pink
}

CGUIControl * CGUIBuilder::GetControl(const std::string & name) const {
  controlmap::const_iterator it = m_Controls.find(name);
  if(it != m_Controls.end())
    return it->second;
  return nullptr;
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
  CGUIRectControl* pControl = nullptr;
  if(!GetControl(name, pControl))
    return;

  pControl->SetTexture(m_Textures[texName]);
  pControl->SetTextureCoords(pos, size);
}



CGUIInstructionBuilder::CGUIInstructionBuilder(CGUIBuilder * pGUIBuilder) :
  m_pGUIBuilder(pGUIBuilder) {
}

CGUIInstructionBuilder::~CGUIInstructionBuilder() {
}

CInstruction* CGUIInstructionBuilder::TextShow(const std::string & name, const float time, const std::string & showText) {
  CGUITextControl* pControl = nullptr;
  if(!m_pGUIBuilder->GetControl(name, pControl))
    return nullptr;
  return new CGUITextAnimation(pControl, showText, 0, showText.length(), time);
}

CInstruction* CGUIInstructionBuilder::TextShow(const std::string & name, const float time) {
  CGUITextControl* pControl = nullptr;
  if(!m_pGUIBuilder->GetControl(name, pControl))
    return nullptr;
  std::string text = pControl->GetText();
  return new CGUITextAnimation(m_pList, pControl, time, true);
}

CInstruction* CGUIInstructionBuilder::TextHide(const std::string & name, const float time, const std::string & text) {
  CGUITextControl* pControl = nullptr;
  if(!m_pGUIBuilder->GetControl(name, pControl))
    return nullptr;
  return new CGUITextAnimation(m_pList, pControl, text, time, false);
}

CInstruction* CGUIInstructionBuilder::TextHide(const std::string & name, const float time) {
  CGUITextControl* pControl = nullptr;
  if(!m_pGUIBuilder->GetControl(name, pControl))
    return nullptr;
  return new CGUITextAnimation(m_pList, pControl, time, false);
}

CInstruction* CGUIInstructionBuilder::TextCountAsc(const std::string& name, const Sint32 value, const std::string & formatText, const float time) {
  CGUITextControl* pControl = nullptr;
  if(!m_pGUIBuilder->GetControl(name, pControl))
    return nullptr;
  return new CGUITextCountAnimation<Sint32>(m_pList, pControl, value, formatText, time, true);
}

CInstruction* CGUIInstructionBuilder::TextCountDsc(const std::string& name, const Sint32 value, const std::string & formatText, const float time) {
  CGUITextControl* pControl = nullptr;
  if(!m_pGUIBuilder->GetControl(name, pControl))
    return nullptr;
  return new CGUITextCountAnimation<Sint32>(m_pList, pControl, value, formatText, time, false);
}

CInstruction* CGUIInstructionBuilder::FadeIn(const std::string & name, const float time, const float min, const float max) {
  CGUIControl* pControl = m_pGUIBuilder->GetControl(name);
  return new CGUIAlphaFadeAnimation(m_pList, pControl, time, min, max, true);
}

CInstruction* CGUIInstructionBuilder::FadeOut(const std::string & name, const float time, const float min, const float max) {
  CGUIControl* pControl = m_pGUIBuilder->GetControl(name);
  return new CGUIAlphaFadeAnimation(m_pList, pControl, time, min, max, false);
}

CInstruction* CGUIInstructionBuilder::BlendIn(const std::string & name, const float time, const glm::vec4 & min, const glm::vec4 & max) {
  CGUIControl* pControl = m_pGUIBuilder->GetControl(name);
  return new CGUIColorFadeAnimation(m_pList, pControl, time, min, max, true);
}

CInstruction* CGUIInstructionBuilder::BlendOut(const std::string & name, const float time, const glm::vec4 & min, const glm::vec4 & max) {
  CGUIControl* pControl = m_pGUIBuilder->GetControl(name);
  return new CGUIColorFadeAnimation(m_pList, pControl, time, min, max, false);
}

CInstruction* CGUIInstructionBuilder::Wait(const float time) {
  return new CGUITimer(m_pList, time);
}

void CGUIInstructionBuilder::BeginBlock(CGUIInstructionList* pNewContext) {
  if(m_pList)
    m_ContextStack.push_back(m_pList);
  m_pList = pNewContext;
}

void CGUIInstructionBuilder::EndBlock() {
  if(m_ContextStack.empty()) {
    if(m_pResult)
      helper::deleteobj(m_pResult);
    m_pResult = m_pList;
    m_pList = nullptr;
    return;
  }

  CGUIInstructionList* pContext = *m_ContextStack.rbegin();
  if(m_pList->IsEmpty()) {
    delete m_pList;
  }
  else {
    pContext->Add(m_pList);
  }
  m_ContextStack.pop_back();
  m_pList = pContext;
}

