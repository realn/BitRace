#ifndef __BITRACE_TESTSTATE_H__
#define __BITRACE_TESTSTATE_H__

#include "EngineState.h"

class CTestState :
  public IEngineState {
private:
  float mRotation;

public:
  CTestState();
  virtual ~CTestState();

  // Inherited via IEngineState
  virtual void Update(const float timeDelta) override;
  virtual void UpdateRender() override;
  virtual void Render() const override;
  virtual void RenderUI() const override;
  virtual const bool IsDone() const override;
  virtual IEngineState * GetNext(CEngine & engine) override;
};


#endif // !__BITRACE_TESTSTATE_H__

