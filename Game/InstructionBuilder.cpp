#include "stdafx.h"
#include "InstructionBuilder.h"
#include "Instructions.h"
#include "helper.h"

CInstructionBuilder::CInstructionBuilder() {}

CInstructionBuilder::~CInstructionBuilder() {
  helper::deletevector(m_ListStack);
}

void CInstructionBuilder::Begin(const InstListType type) {
  CInstructionList* pList = nullptr;
  switch(type) {
  case InstListType::Sequence: pList = new CSequenceInstruction(); break;
  case InstListType::Parallel: pList = new CParellelInstruction(); break;
  default:
    break;
  }
  AddInst(pList);
  AddList(pList);
}

CInstructionList * CInstructionBuilder::Currrent() const {
  if(m_ListStack.empty())
    return nullptr;
  return *m_ListStack.rbegin();
}

CInstructionList * CInstructionBuilder::End() {
  CInstructionList* pList = Currrent();
  m_ListStack.pop_back();
  return pList;
}

CWaitInstruction * CInstructionBuilder::Wait(const float wait) {
  CWaitInstruction* pInst = new CWaitInstruction(wait);
  AddInst(pInst);
  return pInst;
}

CRangeWaitInstruction * CInstructionBuilder::WaitRange(const float min, const float max, const float wait) {
  CRangeWaitInstruction* pInst = new CRangeWaitInstruction(min, max, wait);
  AddInst(pInst);
  return pInst;
}

void CInstructionBuilder::AddInst(CInstruction * pInst) {
  if(pInst) {
    CInstructionList* pList = Currrent();
    if(pList)
      pList->Add(pInst);
  }
}

void CInstructionBuilder::AddList(CInstructionList * pList) {
  if(pList)
    m_ListStack.push_back(pList);
}
