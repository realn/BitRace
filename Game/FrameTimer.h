#ifndef __BITRACE_FRAMETIMER_H__
#define __BITRACE_FRAMETIMER_H__

#include <SDL_types.h>
#include <CBStr/Defines.h>

class CFrameTimer {
private:
  float mTime;
  float mLimit;
  bool mEnabled;

public:
  CFrameTimer(const float limit = 1.0f, const float startTime = 0.0f);
  CFrameTimer(const CFrameTimer& other);

  void SetTime(const float time);
  void SetLimit(const float limit);
  void SetEnabled(const bool value);

  const float GetTime() const;
  const float GetLimit() const;
  const bool IsEnabled() const;

  const bool Update(const float timeDelta);
};

class IFrameTimerObserver {
protected:
  IFrameTimerObserver();

public:
  virtual ~IFrameTimerObserver();

  virtual void OnTimerTicked(const cb::string& id, CFrameTimer& timer) = 0;
};

class CFrameTimerRegistry {
private:
  typedef std::map<cb::string, CFrameTimer> TimerMapT;
  typedef std::vector<IFrameTimerObserver*> ObserverListT;

  TimerMapT mTimerMap;
  ObserverListT mObserverList;

public:
  CFrameTimerRegistry();
  ~CFrameTimerRegistry();

  void AddTmer(const cb::string& id, const CFrameTimer& timer);
  void AddObserver(IFrameTimerObserver* pObserver);

  void RemoveTimer(const cb::string& id);
  void RemoveObserver(IFrameTimerObserver* pObserver);

  void Update(const float timeDelta);
};

#endif // !__BITRACE_FRAMETIMER_H__

