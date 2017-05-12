#ifndef __BITRACE_GAMESTATE_H__
#define __BITRACE_GAMESTATE_H__

#include "EngineState.h"
#include "RaceTrack.h"
#include "Racer.h"
#include "UIFont.h"
#include "FrameTimer.h"
#include "LevelBackground.h"

class CConfig;
class IFileSystem;
class CInputDeviceMap;
class CModelRepository;

class CUIScreen;
template<typename _Type> class CUITextNumber;
class CUIProgressBar;
class CUIRect;

class CGameEntityType;
typedef std::map<cb::string, CGameEntityType> GameEntityTypeMapT;
class CGameDifficultySetting;

class CGameState :
  public IEngineState {
private:
  CConfig& mConfig;
  CInputDeviceMap& mIDevMap;
  CModelRepository* mModelRepo;

  GameEntityTypeMapT mEntityTypes;
  CGameDifficultySetting* mDiffSetting;
  CFrameTimer mSpawnTimer;

  CLevelBackground mBackground;
  CRaceTrack mLevel;
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
             CInputDeviceMap& inputDevMap,
             CModelRepository* pModelRepo);
  virtual ~CGameState();

  const bool LoadResources(IFileSystem& fs);
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

