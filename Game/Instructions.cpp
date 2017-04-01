#include "stdafx.h"
#include "Instructions.h"
#include "helper.h"

CInstruction::CInstruction() : m_State(InstState::START) {}

CInstruction::~CInstruction() {}

void CInstruction::Update(const float timeDelta) {}

const InstState CInstruction::GetState() const {
  return m_State;
}

void CInstruction::Run() {
  OnRun();
  m_State = InstState::RUNNING;
}

void CInstruction::Reset() {
  OnReset();
  m_State = InstState::START;
}

void CInstruction::Pause() {
  OnPause();
  m_State = InstState::PAUSED;
}

void CInstruction::End() {
  OnEnd();
  m_State = InstState::FINISHED;
}



CInstructionList::CInstructionList() {}

CInstructionList::~CInstructionList() {
  helper::deletevector(m_List);
}

void CInstructionList::Add(CInstruction * pInst) {
  m_List.push_back(pInst);
}

void CInstructionList::ResetAll() {
  for(instvector_t::iterator it = m_List.begin(); it != m_List.end(); it++) {
    (*it)->Reset();
  }
}

void CInstructionList::RunAll() {
  for(instvector_t::iterator it = m_List.begin(); it != m_List.end(); it++) {
    (*it)->Run();
  }
}

void CInstructionList::PauseAll() {
  for(instvector_t::iterator it = m_List.begin(); it != m_List.end(); it++) {
    (*it)->Pause();
  }
}

void CInstructionList::EndAll() {
  for(instvector_t::iterator it = m_List.begin(); it != m_List.end(); it++) {
    (*it)->End();
  }
}



CSequenceInstruction::CSequenceInstruction() : m_Position(0) {}

CSequenceInstruction::~CSequenceInstruction() {}

void CSequenceInstruction::Update(const float timeDelta) {
  if(m_State != InstState::RUNNING)
    return;

  CInstruction* pInst = m_List[m_Position];

  pInst->Update(timeDelta);
  if(pInst->GetState() == InstState::FINISHED)
    m_Position++;
  if(m_Position == m_List.size())
    End();
}

void CSequenceInstruction::OnReset() {
  ResetAll();
  m_Position = 0;
}

void CSequenceInstruction::OnRun() {
  RunAll();
}

void CSequenceInstruction::OnPause() {
  PauseAll();
}

void CSequenceInstruction::OnEnd() {
  EndAll();
}



CParellelInstruction::CParellelInstruction() {}

CParellelInstruction::~CParellelInstruction() {}

void CParellelInstruction::Update(const float timeDelta) {
  if(m_State != InstState::RUNNING)
    return;

  Uint32 instFinished = 0;
  for(instvector_t::iterator it = m_List.begin(); it != m_List.end(); it++) {
    (*it)->Update(timeDelta);
    if((*it)->GetState() == InstState::FINISHED)
      instFinished++;
  }

  if(m_List.size() == instFinished)
    End();
}

void CParellelInstruction::OnReset() {
  ResetAll();
}

void CParellelInstruction::OnRun() {
  RunAll();
}

void CParellelInstruction::OnPause() {
  PauseAll();
}

void CParellelInstruction::OnEnd() {
  EndAll();
}



CWaitInstruction::CWaitInstruction(const float timeTarget) 
  : m_Time(0.0f), m_TimeTarget(timeTarget) {}

CWaitInstruction::~CWaitInstruction() {}

void CWaitInstruction::Update(const float timeDelta) {
  if(m_State != InstState::RUNNING)
    return;

  m_Time = glm::clamp(m_Time + timeDelta, 0.0f, m_TimeTarget);
  if(m_Time == m_TimeTarget)
    End();
}

const float CWaitInstruction::GetTimeTarget() const {
  return m_TimeTarget;
}

const float CWaitInstruction::GetTime() const {
  return m_Time;
}

void CWaitInstruction::SetTimeTarget(const float timeTarget) {
  m_TimeTarget = timeTarget;
}

void CWaitInstruction::SetTime(const float time) {
  m_Time = time;
}

void CWaitInstruction::OnReset() {
  m_Time = 0.0;
}

void CWaitInstruction::OnEnd() {
  m_Time = m_TimeTarget;
}



CRangeWaitInstruction::CRangeWaitInstruction(const float min, 
                                             const float max, 
                                             const float waitTime)
  : CWaitInstruction(waitTime)
  , m_Min(min)
  , m_Max(max) 
{}

CRangeWaitInstruction::~CRangeWaitInstruction() {}

const float CRangeWaitInstruction::GetValue() const {
  float v = m_Time / m_TimeTarget;
  return m_Min + (m_Max - m_Min) * v;
}

const float CRangeWaitInstruction::GetMaxValue() const {
  return m_Max;
}

const float CRangeWaitInstruction::GetMinValue() const {
  return m_Min;
}
