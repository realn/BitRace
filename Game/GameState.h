#ifndef __BITRACE_GAMESTATE_H__
#define __BITRACE_GAMESTATE_H__

#include "EngineState.h"
#include "RaceTrack.h"
#include "Racer.h"
#include "UIFont.h"

class CUIScreen;
template<typename _Type> class CUITextNumber;
class CUIProgressBar;
class CUIRect;

class CConfig;
class CInputDeviceMap;
class CGameEntityType;
class IFileSystem;
typedef std::map<cb::string, CGameEntityType> GameEntityTypeMapT;

class CGameState :
  public IEngineState {
private:
  CConfig& mConfig;
  GameEntityTypeMapT mEntityTypes;
  CInputDeviceMap& mIDevMap;
  CRaceTrack mRaceTrack;
  CRacer mRacer;
  CUIFont mFont;

  Uint32 mPoints;

  CUIScreen* mMainUI;
  CUITextNumber<Sint32>* mFPSCounter;
  CUIProgressBar* mUIHealthBar;
  CUITextNumber<Sint32>* mUIPoints;

public:
  CGameState(CConfig& config,
             IFileSystem& fileSystem,
             CInputDeviceMap& inputDevMap);
  virtual ~CGameState();

  const bool Init();
  void Free();

  void ResetGame();

  const CRaceTrack& GetRaceTrack() const;
  const bool IsGameOver() const;

  // Inherited via IEngineState
  virtual void Update(const float timeDelta) override;

  virtual void UpdateRender(const float timeDelta) override;

  virtual void Render() const override;

  virtual void RenderUI() const override;

  virtual const bool IsDone() const override;

  virtual IEngineState * GetNext(CEngine & engine) override;
};

#endif // !__BITRACE_GAMESTATE_H__

