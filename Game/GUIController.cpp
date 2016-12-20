#include "stdafx.h"
#include "GUIController.h"
#include "GUIScreen.h"
#include "helper.h"

CGUIController::CGUIController(CGUIControllerList * pList, const bool enabled) :
  m_Enabled(enabled), m_State(STOPPED) 
{
  pList->Add(this);
}

void CGUIController::Run() {
  this->ChangeState(RUNNING);
}

void CGUIController::Stop() {
  this->ChangeState(STOPPED);
}

void CGUIController::Done() { 
  this->ChangeState(DONE);
}

void CGUIController::Rerun() {
  this->Stop();
  this->Run();
}

void CGUIController::ForceRun() {
  m_State = RUNNING;
}

void CGUIController::SetEnabled(const bool enabled) { 
  m_Enabled = enabled; 
}

const bool CGUIController::IsEnabled() const { 
  return m_Enabled; 
}

const bool CGUIController::IsRunning() const { 
  return m_State == RUNNING; 
}

const bool CGUIController::IsStopped() const {
  return m_State == STOPPED;
}

const bool CGUIController::IsDone() const { 
  return m_State == DONE; 
}

void CGUIController::ChangeState(const State newState) {
  if(m_State == newState)
    return;

  State state = newState;
  this->StateChange(m_State, state);
  m_State = state;
}


//================================================================================


CGUIControllerList::CGUIControllerList() {}

CGUIControllerList::~CGUIControllerList() {
  this->Clear();
}

void CGUIControllerList::Update(const float timeDelta) {
  for(std::set<CGUIController*>::iterator it = m_List.begin(); it != m_List.end(); it++) {
    if((*it)->IsEnabled()) {
      (*it)->Update(timeDelta);
    }
  }
}

CGUIController* CGUIControllerList::Add(CGUIController * pController) {
  m_List.insert(pController);
  return pController;
}

void CGUIControllerList::Clear() {
  helper::deleteset(m_List);
}

void CGUIControllerList::StopAll() {
  for(std::set<CGUIController*>::iterator it = m_List.begin(); it != m_List.end(); it++) {
    (*it)->Stop();
  }
}


//================================================================================


CGUIDoneTrigger::CGUIDoneTrigger(CGUIControllerList* pList, CGUIController * pCondition, CGUIController * pTarget, const bool running) :
  CGUIController(pList), m_CondOp(CO_AND) 
{
  this->AddCondition(pCondition);
  this->AddTarget(pTarget);
  this->Run();
}

CGUIDoneTrigger::~CGUIDoneTrigger() {}

void CGUIDoneTrigger::AddCondition(CGUIController * pCondition) {
  if(pCondition == nullptr)
    return;
  if(std::find(m_Conditions.begin(), m_Conditions.end(), pCondition) != m_Conditions.end())
    return;

  m_Conditions.push_back(pCondition);
}

void CGUIDoneTrigger::AddTarget(CGUIController * pTarget) {
  if(pTarget == nullptr)
    return;
  if(std::find(m_Targets.begin(), m_Targets.end(), pTarget) != m_Targets.end())
    return;

  m_Targets.push_back(pTarget);
}

void CGUIDoneTrigger::SetCondOp(const CondOp condOp) {
  m_CondOp = condOp;
}

void CGUIDoneTrigger::Update(const float timeDelta) {
  if(!IsRunning())
    return;

  bool cond = m_CondOp == CO_AND;

  for(std::vector<CGUIController*>::iterator it = m_Conditions.begin(); it != m_Conditions.end(); it++) {
    if(m_CondOp == CO_AND)
      cond &= (*it)->IsDone();
    else
      cond |= (*it)->IsDone();
  }

  if(cond) {
    Done();

    for(std::vector<CGUIController*>::iterator it = m_Targets.begin(); it != m_Targets.end(); it++) {
      (*it)->Run();
    }
  }
}

void CGUIDoneTrigger::StateChange(const State oldState, State & newState) {
  if(newState == STOPPED)
    newState = RUNNING;
}


//================================================================================


CGUITextAnimation::CGUITextAnimation(CGUIControllerList* pList, CGUITextControl* pControl, const std::string text, const float animTime, const bool show) :
  CGUIController(pList), m_pControl(pControl), m_Text(text), m_AnimTime(animTime), m_Time(0.0f), m_CharTime(0.0f), m_CharLen(0), m_Visible(show) {
  m_CharTime = m_AnimTime / m_Text.length();
}

CGUITextAnimation::CGUITextAnimation(CGUIControllerList* pList, CGUITextControl* pControl, const float animTime, const bool show) :
  CGUIController(pList), m_pControl(pControl), m_Text(pControl->GetText()), m_AnimTime(animTime), m_CharTime(0.0f), m_CharLen(0), m_Visible(show) {
  m_CharTime = m_AnimTime / m_Text.length();
}

CGUITextAnimation::~CGUITextAnimation() {}

void CGUITextAnimation::Update(float timeDelta) {
  if(!IsRunning())
    return;

  m_Time += timeDelta;
  if(m_Time < m_CharTime)
    return;

  m_Time -= m_CharTime;
  m_CharLen += (m_Visible ? 1 : -1);
  if((m_Visible && m_CharLen >= m_Text.length()) || (!m_Visible && m_CharLen == 0)) {
    Done();
    return;
  }

  m_pControl->SetText(m_Text.substr(0, m_CharLen) + "_", false);
}

void CGUITextAnimation::SetVisible(const bool visible) {
  m_Visible = visible;
}

void CGUITextAnimation::Show() {
  m_Visible = true;
  this->Run();
}

void CGUITextAnimation::Hide() {
  m_Visible = false;
  this->Run();
}

void CGUITextAnimation::StateChange(const State oldState, State & newState) {
  if(newState == RUNNING) {
    m_pControl->SetVisible(true);

    if(oldState == STOPPED) {
      m_Time = 0.0f;
      if(m_Visible) {
        m_CharLen = 0;
        m_pControl->SetText("_", false);
      }
      else {
        m_CharLen = m_Text.length();
        m_pControl->SetText(m_Text, false);
      }
    }
  }
  if(newState == DONE) {
    m_pControl->SetVisible(m_Visible);
    m_pControl->SetText(m_Text, false);
  }
  if(newState == STOPPED) {
    m_pControl->SetVisible(!m_Visible);
  }
}



//================================================================================


CGUITextCountAnimationBase::CGUITextCountAnimationBase(CGUIControllerList* pList, CGUITextControl * pControl, const std::string& formatText, const float animTime, const bool show) :
  CGUIController(pList), m_pControl(pControl), m_FormatText(formatText), m_AnimTime(animTime), m_Time(0.0f), m_Visible(show) {}

CGUITextCountAnimationBase::~CGUITextCountAnimationBase() {}

void CGUITextCountAnimationBase::Update(const float timeDelta) {
  if(!IsRunning())
    return;

  m_Time += (m_Visible ? timeDelta : -timeDelta);
  m_Time = glm::clamp(m_Time, 0.0f, m_AnimTime);

  this->UpdateValue();

  std::string text = this->GetValueText();
  this->m_pControl->SetText(text);

  if((m_Visible && m_AnimTime == m_Time) || (!m_Visible && m_Time == 0.0f)) {
    Done();
  }
}

void CGUITextCountAnimationBase::SetVisible(const bool value) {
  m_Visible = value;
}

void CGUITextCountAnimationBase::StateChange(const State oldState, State & newState) {
  if(oldState == STOPPED && newState == RUNNING) {
    m_Time = m_Visible ? 0.0f : m_AnimTime;
  }
}


//================================================================================


CGUIFadeAnimation::CGUIFadeAnimation(CGUIControllerList* pList, 
                                     CGUIControl* pControl, 
                                     const float animTime, 
                                     const bool visible) :
  CGUIController(pList), m_pControl(pControl), m_AnimTime(animTime), m_Time(0.0f), m_Visible(visible) {}

CGUIFadeAnimation::~CGUIFadeAnimation() {}

void CGUIFadeAnimation::Update(float timeDelta) {
  if(!IsRunning())
    return;

  float timeAdd = m_Visible ? timeDelta : -timeDelta;
  m_Time = glm::clamp(m_Time + timeAdd, 0.0f, m_AnimTime);

  glm::vec4 color = CalcColor();
  m_pControl->SetColor(color);

  if((m_Visible && m_Time >= m_AnimTime) || (!m_Visible && m_Time <= 0.0f)) {
    m_pControl->SetVisible(m_Visible);
    Done();
  }
  else {
    m_pControl->SetVisible(true);
  }
}

void CGUIFadeAnimation::SetVisible(const bool visible) {
  m_Visible = visible;
}

void CGUIFadeAnimation::Show() {
  m_Visible = true;
  Run();
}

void CGUIFadeAnimation::Hide() {
  m_Visible = false;
  Run();
}

const bool CGUIFadeAnimation::IsVisible() const {
  return m_Visible;
}

void CGUIFadeAnimation::StateChange(const State oldState, State & newState) {
  if(newState == RUNNING && oldState == STOPPED) {
    m_Time = (m_Visible ? 0.0f : m_AnimTime);
  }
}


//================================================================================


CGUIAlphaFadeAnimation::CGUIAlphaFadeAnimation(CGUIControllerList * pList, 
                                               CGUIControl * pControl, 
                                               const float animTime, 
                                               const float minAlpha, 
                                               const float maxAlpha, 
                                               const bool visible) :
  CGUIFadeAnimation(pList, pControl, animTime, visible), m_MinAlpha(minAlpha), m_MaxAlpha(maxAlpha) {}

CGUIAlphaFadeAnimation::~CGUIAlphaFadeAnimation() {}

void CGUIAlphaFadeAnimation::SetAlphaRange(const float min, const float max) {
  m_MinAlpha = min;
  m_MaxAlpha = max;
}

const glm::vec4 CGUIAlphaFadeAnimation::CalcColor() const {
  glm::vec4 color = this->m_pControl->GetColor();
  color.a = glm::mix(m_MinAlpha, m_MaxAlpha, m_Time / m_AnimTime);
  return color;
}


//================================================================================


CGUIColorFadeAnimation::CGUIColorFadeAnimation(CGUIControllerList * pList, 
                                               CGUIControl * pControl, 
                                               const float animTime, 
                                               const glm::vec4 & minColor, 
                                               const glm::vec4 & maxColor, 
                                               const bool visible) :
  CGUIFadeAnimation(pList, pControl, animTime, visible), m_MinColor(minColor), m_MaxColor(maxColor) {}

CGUIColorFadeAnimation::~CGUIColorFadeAnimation() {}

void CGUIColorFadeAnimation::SetColorRange(const glm::vec4 & min, const glm::vec4 & max) {
  m_MinColor = min;
  m_MaxColor = max;
}

const glm::vec4 CGUIColorFadeAnimation::CalcColor() const {
  return glm::mix(m_MinColor, m_MaxColor, m_Time / m_AnimTime);
}


//================================================================================

CGUITimer::CGUITimer(CGUIControllerList* pList, const float waitTime, CGUIController* pTarget, const bool started) :
  CGUIController(pList), m_pTarget(pTarget), m_WaitTime(waitTime), m_Time(0.0f) 
{
}

CGUITimer::~CGUITimer() {}

void CGUITimer::Update(const float timeDelta) {
  if(!IsRunning())
    return;

  m_Time = glm::clamp(m_Time + timeDelta, 0.0f, m_WaitTime);
  if(m_Time >= m_WaitTime) {
    Done();
    if(m_pTarget) {
      m_pTarget->Run();
    }
  }
}

void CGUITimer::StateChange(const State oldState, State & newState) {
  if(oldState == STOPPED && newState == RUNNING) {
    m_Time = 0.0f;
  }
}
