#include "stdafx.h"
#include "PerfTimer.h"

#include <SDL_timer.h>

CPerfTimer::CPerfTimer()
  : mFrequency(0)
  , mLastTick(0)
  , mLastTimeDelta(0.0f)
{}

const bool CPerfTimer::Init() {
  cb::info(L"Initializing performance timer.");
  mFrequency = SDL_GetPerformanceFrequency();
  if(mFrequency == 0) {
    cb::error(L"Invalid timer frequency (0)");
    return false;
  }
  mLastTick = SDL_GetPerformanceCounter();
  mLastTimeDelta = 0.01f;
  return true;
}

const float CPerfTimer::Update() {
  Uint64 thisTick = SDL_GetPerformanceCounter();
  Uint64 diff = thisTick - mLastTick;
  mLastTimeDelta = float(double(diff) / double(mFrequency));
  mLastTick = thisTick;

  return mLastTimeDelta;
}

const float CPerfTimer::GetTimeDelta() const {
  return mLastTimeDelta;
}

const Uint64 CPerfTimer::GetLastTick() const {
  return mLastTick;
}

const float CPerfTimer::GetFramesPerSecond() const {
  if(mLastTimeDelta == 0.0f)
    return 100.0f;
  return 1.0f / mLastTimeDelta;
}
