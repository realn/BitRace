#include "stdafx.h"
#include "FrameTimer.h"

CFrameTimer::CFrameTimer(const float limit, const float startTime) 
  : mTime(startTime)
  , mLimit(limit)
  , mEnabled(false)
{}

CFrameTimer::CFrameTimer(const CFrameTimer & other)
  : mTime(other.mTime)
  , mLimit(other.mLimit)
  , mEnabled(other.mEnabled)
{}

void CFrameTimer::SetTime(const float time) {
  mTime = time;
}

void CFrameTimer::SetLimit(const float limit) {
  mLimit = limit;
}

void CFrameTimer::SetEnabled(const bool value) {
  mEnabled = value;
}

const float CFrameTimer::GetTime() const {
  return mTime;
}

const float CFrameTimer::GetLimit() const {
  return mLimit;
}

const bool CFrameTimer::IsEnabled() const {
  return mEnabled;
}

const bool CFrameTimer::Update(const float timeDelta) {
  if(!mEnabled)
    return false;
  mTime += timeDelta;
  if(mTime >= mLimit) {
    mTime -= mLimit;
    return true;
  }
  return false;
}

IFrameTimerObserver::IFrameTimerObserver() {}

IFrameTimerObserver::~IFrameTimerObserver() {}

CFrameTimerRegistry::CFrameTimerRegistry() {}

CFrameTimerRegistry::~CFrameTimerRegistry() {}

void CFrameTimerRegistry::AddTmer(const cb::string & id, const CFrameTimer & timer) {
  mTimerMap[id] = timer;
}

void CFrameTimerRegistry::AddObserver(IFrameTimerObserver * pObserver) {
  if(std::find(mObserverList.begin(), mObserverList.end(), pObserver)
     == mObserverList.end()) {
    mObserverList.push_back(pObserver);
  }
}

void CFrameTimerRegistry::RemoveTimer(const cb::string & id) {
  TimerMapT::iterator it = mTimerMap.find(id);
  if(it != mTimerMap.end()) {
    mTimerMap.erase(it);
  }
}

void CFrameTimerRegistry::RemoveObserver(IFrameTimerObserver * pObserver) {
  ObserverListT::iterator it =
    std::find(mObserverList.begin(), mObserverList.end(), pObserver);
  if(it != mObserverList.end()) {
    mObserverList.erase(it);
  }
}

void CFrameTimerRegistry::Update(const float timeDelta) {
  for(TimerMapT::iterator it = mTimerMap.begin(); it != mTimerMap.end(); it++) {
    if(!it->second.Update(timeDelta)) {
      continue;
    }

    for(ObserverListT::iterator oit = mObserverList.begin(); oit != mObserverList.end(); oit++) {
      (*oit)->OnTimerTicked(it->first, it->second);
    }
  }
}
