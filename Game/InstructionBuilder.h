#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <vector>

enum class InstListType {
  Sequence = 0,
  Parallel = 1,
};

class CInstruction;
class CWaitInstruction;
class CRangeWaitInstruction;

class CInstructionList;
class CInstructionBuilder {
protected:
  typedef std::vector<CInstructionList*> instlistvector_t;

  instlistvector_t m_ListStack;

public:
  CInstructionBuilder();
  virtual ~CInstructionBuilder();

  void Begin(const InstListType type = InstListType::Sequence);
  CInstructionList* Currrent() const;
  CInstructionList* End();

  CWaitInstruction* Wait(const float wait);
  CRangeWaitInstruction* WaitRange(const float min, const float max, const float wait);

protected:
  void AddInst(CInstruction* pInst);
  void AddList(CInstructionList* pList);
};