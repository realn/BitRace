#ifndef __BITRACE_LOGICPROCESS_H__
#define __BITRACE_LOGICPROCESS_H__

#include <SDL_types.h>

class ILogicProcess {
protected:
  ILogicProcess();

public:
  virtual ~ILogicProcess();

  virtual void Update(const float timeDelta) = 0;
};

#endif // !__BITRACE_LOGICPROCESS_H__

