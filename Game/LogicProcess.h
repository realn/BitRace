#ifndef __BITRACE_LOGICPROCESS_H__
#define __BITRACE_LOGICPROCESS_H__

#include <SDL_types.h>

class ILogicProcess {
protected:
  ILogicProcess();

public:
  virtual ~ILogicProcess();

  virtual void Update(const float timeDelta) = 0;

  //virtual const bool IsFinished() const = 0;
};

#endif // !__BITRACE_LOGICPROCESS_H__

