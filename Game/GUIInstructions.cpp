#include "stdafx.h"
#include "GUIInstructions.h"
#include "GUIScreen.h"
#include "helper.h"


//================================================================================


CGUITextAnimation::CGUITextAnimation(CGUITextControl* pControl,
                                     const std::string text,
                                     const Uint32 lenStart,
                                     const Uint32 lenTarget,
                                     const float animTime) 
  : m_pControl(pControl)
  , m_Text(text)
  , m_TextTargetPos(lenTarget)
  , m_TextStartPos(lenStart)
  , m_TextPos(lenStart)
  , m_AnimTime(animTime)
  , m_CharTime(0.0f)
  , m_Time(0.0f)
  , m_TextChanged(false)
{
  m_CharTime = m_AnimTime / m_Text.length();
}

CGUITextAnimation::~CGUITextAnimation() {}

void CGUITextAnimation::Update(float timeDelta) {
  if(m_State != InstState::RUNNING)
    return;

  m_Time += timeDelta;
  if(m_Time >= m_CharTime) {
    m_Time -= m_CharTime;
    m_TextPos += (m_TextTargetPos > m_TextPos) ? 1 : -1;
    m_TextChanged = false;
  }

  if(!m_TextChanged) {
    std::string text = m_Text.substr(0, m_TextPos);
    if(m_TextPos == m_TextTargetPos) {
      text += "_";
    }
    m_pControl->SetText(text, false);
    m_TextChanged = true;
  }

  if(m_TextPos == m_TextTargetPos)
    End();
}

void CGUITextAnimation::OnReset() {
  m_TextChanged = false;
  m_TextPos = m_TextStartPos;
}

void CGUITextAnimation::OnEnd() {
  m_TextChanged = true;
  m_TextPos = m_TextTargetPos;
}


//================================================================================


CGUITextCountAnimation::CGUITextCountAnimation(CGUITextControl * pControl, 
                                               const Uint32 valStart, 
                                               const Uint32 valTarget, 
                                               const float waitTime, 
                                               const std::string & prefix,
                                               const std::string & sufix) 
  : CWaitInstruction(waitTime)
  , m_pControl(pControl)
  , m_TextPrefix(prefix)
  , m_TextSufix(sufix)
  , m_ValueStart(valStart)
  , m_ValueTarget(valTarget)
  , m_LastValue(valTarget)
{}

CGUITextCountAnimation::~CGUITextCountAnimation() {}

void CGUITextCountAnimation::Update(const float timeDelta) {
  CWaitInstruction::Update(timeDelta);
  float val = GetValue();
  if(val != m_LastValue) {
    m_LastValue = val;
    m_pControl->SetText(GetText(), false);
  }
}

void CGUITextCountAnimation::SetValueRange(const Uint32 start, const Uint32 target) {
  m_ValueStart = start;
  m_ValueTarget = target;
}

void CGUITextCountAnimation::SetText(const std::string & prefix, const std::string & sufix) {
  m_TextPrefix = prefix;
  m_TextSufix = sufix;
}

const Uint32 CGUITextCountAnimation::GetValue() const {
  float v = m_Time / m_TimeTarget;
  float val = (float)(m_ValueTarget - m_ValueStart) * v;
  return m_ValueStart + (Uint32)(val);
}

const std::string CGUITextCountAnimation::GetText() const {
  return m_TextPrefix + helper::tostr(GetValue()) + m_TextSufix;
}

void CGUITextCountAnimation::OnReset() {
  CWaitInstruction::OnReset();
  m_LastValue = m_ValueTarget;
}

void CGUITextCountAnimation::OnEnd() {
  CWaitInstruction::OnEnd();
  m_LastValue = m_ValueStart;
}


//================================================================================

const glm::vec4 CGUIFadeAnimation::maskAll = glm::vec4(1.0f);
const glm::vec4 CGUIFadeAnimation::maskColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
const glm::vec4 CGUIFadeAnimation::maskAlpha = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);


CGUIFadeAnimation::CGUIFadeAnimation(CGUIControl * pControl,
                                     const glm::vec4 colorStart,
                                     const glm::vec4 colorTarget,
                                     const float waitTime,
                                     const glm::vec4 colorMask)
  : CWaitInstruction(waitTime)
  , m_pControl(pControl)
  , m_ColorStart(colorStart)
  , m_ColorTarget(colorTarget)
  , m_ColorMask(colorMask)
{}

CGUIFadeAnimation::~CGUIFadeAnimation() {}

void CGUIFadeAnimation::Update(float timeDelta) {
  CWaitInstruction::Update(timeDelta);

  glm::vec4 oldColor = m_pControl->GetColor();
  glm::vec4 newColor = GetColor();

  m_pControl->SetColor(glm::mix(oldColor, newColor, m_ColorMask));
}

void CGUIFadeAnimation::SetColor(const glm::vec4 & start, const glm::vec4 & target) {
  m_ColorStart = start;
  m_ColorTarget = target;
}

void CGUIFadeAnimation::SetColorMask(const glm::vec4 & mask) {
  m_ColorMask = mask;
}

const glm::vec4 CGUIFadeAnimation::GetColor() const {
  float v = m_Time / m_TimeTarget;
  return glm::mix(m_ColorStart, m_ColorTarget, v);
}
