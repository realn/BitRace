#ifndef __BITRACE_ENGINESTATE_H__
#define __BITRACE_ENGINESTATE_H__

class CEngine;
class IEngineState {
protected:
  IEngineState();

public:
  virtual ~IEngineState();

  virtual void Update(const float timeDelta) = 0;

  virtual void UpdateRender() = 0;

  virtual void Render() const = 0;
  virtual void RenderUI() const = 0;

  virtual const bool IsDone() const = 0;
  virtual IEngineState* GetNext(CEngine& engine) = 0;
};

#endif // !__BITRACE_ENGINESTATE_H__