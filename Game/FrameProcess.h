#ifndef __BITRACE_FrameProcess_H__
#define __BITRACE_FrameProcess_H__

#include <SDL_types.h>

class IFrameProcess {
protected:
  IFrameProcess();

public:
  virtual ~IFrameProcess();

  virtual void Update(const float timeDelta) = 0;

  //virtual const bool IsFinished() const = 0;
};

#endif // !__BITRACE_FrameProcess_H__

