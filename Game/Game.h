#ifndef __BITRACE_GAME_H__
#define __BITRACE_GAME_H__

#include "LogicProcess.h"
#include "GraphicView.h"
#include "RaceTrack.h"
#include "Racer.h"

class CConfig;
class CInputDeviceMap;

class CGameProcess :
  public ILogicProcess {
private:
  CConfig& mConfig;
  CInputDeviceMap& mIDevMap;
  CRaceTrack mRaceTrack;
  CRacer mRacer;

public:
  CGameProcess(CConfig& config, CInputDeviceMap& inputDevMap);
  virtual ~CGameProcess();

  const bool Init();
  void Free();

  void ResetGame();

  const CRaceTrack& GetRaceTrack() const;
  const bool IsGameOver() const;

  // Inherited via ILogicProcess
  virtual void Update(const float timeDelta) override;
};

class CGameView :
  public IGraphicView {
private:
  CGameProcess& mProcess;

public:
  CGameView(CGameProcess& gameProcess);
  virtual ~CGameView();

  // Inherited via IGraphicView
  virtual void Render(const glm::mat4 & transform) const override;
  virtual void RenderUI(CGUI & gui) const override;
};

#endif // !__BITRACE_GAME_H__

