#pragma once

#include <SDL_types.h>
#include <vector>

enum class InstState {
  START = 0,
  RUNNING = 1,
  FINISHED = 2,
  PAUSED = 3
};

class CInstruction {
protected:
  InstState m_State;

public:
  CInstruction();
  virtual ~CInstruction();

  virtual void Update(const float timeDelta) = 0;

  const InstState GetState() const;

  void Run();
  void Reset();
  void Pause();
  void End();

protected:
  virtual void OnReset() {}
  virtual void OnRun() {}
  virtual void OnPause() {}
  virtual void OnEnd() {}
};

class CInstructionList : public CInstruction {
protected:
  typedef std::vector<CInstruction*> instvector_t;

  instvector_t m_List;

public:
  CInstructionList();
  virtual ~CInstructionList();

  void Add(CInstruction* pInst);

  void ResetAll();
  void RunAll();
  void PauseAll();
  void EndAll();
};

class CSequenceInstruction : public CInstructionList 
{
protected:
  Uint32  m_Position;
public:
  CSequenceInstruction();
  virtual ~CSequenceInstruction();

  virtual void Update(const float timeDelta) override;

protected:
  virtual void OnReset() override;
  virtual void OnRun() override;
  virtual void OnPause() override;
  virtual void OnEnd() override;
};

class CParellelInstruction : public CInstructionList 
{
public:
  CParellelInstruction();
  virtual ~CParellelInstruction();

  virtual void Update(const float timeDelta) override;

protected:
  virtual void OnReset() override;
  virtual void OnRun() override;
  virtual void OnPause() override;
  virtual void OnEnd() override;
};

class CWaitInstruction :
  public CInstruction 
{
protected:
  float m_Time;
  float m_TimeTarget;

public:
  CWaitInstruction(const float timeTarget);
  virtual ~CWaitInstruction();

  virtual void Update(const float timeDelta) override;

  const float GetTimeTarget() const;
  const float GetTime() const;

  void SetTimeTarget(const float timeTarget);
  void SetTime(const float time);

protected:
  virtual void OnReset() override;
  virtual void OnEnd() override;
};

class CRangeWaitInstruction :
  public CWaitInstruction 
{
protected:
  float m_Min;
  float m_Max;

public:
  CRangeWaitInstruction(const float min, const float max, const float waitTime);
  virtual ~CRangeWaitInstruction();

  const float GetValue() const;
  const float GetMaxValue() const;
  const float GetMinValue() const;
};

