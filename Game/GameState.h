#ifndef __BITRACE_GAMESTATE_H__
#define __BITRACE_GAMESTATE_H__

#include "EngineState.h"
#include "RaceTrack.h"
#include "Racer.h"

class CConfig;
class CInputDeviceMap;

class CGameState :
  public IEngineState
{
private:
  CConfig& mConfig;
  CInputDeviceMap& mIDevMap;
  CRaceTrack mRaceTrack;
  CRacer mRacer;

public:
  CGameState(CConfig& config, CInputDeviceMap& inputDevMap);
  virtual ~CGameState();

  const bool Init();
  void Free();

  void ResetGame();

  const CRaceTrack& GetRaceTrack() const;
  const bool IsGameOver() const;

  // Inherited via IEngineState
  virtual void Update(const float timeDelta) override;

  virtual void UpdateRender() override;

  virtual void Render() const override;

  virtual void RenderUI() const override;

  virtual const bool IsDone() const override;

  virtual IEngineState * GetNext(CEngine & engine) override;
};

#endif // !__BITRACE_GAMESTATE_H__

