#ifndef __BITRACE_PERFTIMER_H__
#define __BITRACE_PERFTIMER_H__

#include <SDL_types.h>

class CPerfTimer {
private:
  Uint64 mFrequency;
  Uint64 mLastTick;
  float mLastTimeDelta;

public:
  CPerfTimer();

  const bool Init();

  const float Update();

  const float GetTimeDelta() const;
  const Uint64 GetLastTick() const;
  const float GetFramesPerSecond() const;
};

#endif // !__BITRACE_PERFTIMER_H__
