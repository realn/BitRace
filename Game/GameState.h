#ifndef __BITRACE_GAMESTATE_H__
#define __BITRACE_GAMESTATE_H__

#include <CBIO/Ptr.h>

#include "EngineState.h"
#include "GameLevel.h"
#include "UIFont.h"
#include "FrameTimer.h"
#include "LevelBackground.h"

class CConfig;
class IFileSystem;
class CInputDeviceMap;
class CModelRepository;

class CUIScreen;
class CUIProgressBar;
class CUIRect;
template<typename _Type> class CUITextNumber;

class CGameEntityType;
class CGameDifficultySetting;
class CGamePlayer;
typedef std::map<cb::string, CGameEntityType> GameEntityTypeMapT;

class CGameState :
  public IEngineState {
private:
  CConfig& mConfig;
  CInputDeviceMap& mIDevMap;
  cb::ptr<CModelRepository> mpModelRepo;

  GameEntityTypeMapT mEntityTypes;
  cb::ptr<CGameDifficultySetting> mpDiffSetting;
  CFrameTimer mSpawnTimer;

  CLevelBackground mBackground;
  CGameLevel mLevel;
  cb::ptr<CGamePlayer> mpPlayer;
  CUIFont mFont;

  Uint32 mPoints;

  cb::ptr<CUIScreen> mpMainUI;
  cb::ptr<CUITextNumber<Sint32>> mpFPSCounter;
  cb::ptr<CUIProgressBar> mpUIHealthBar;
  cb::ptr<CUITextNumber<Sint32>> mpUIPoints;

public:
  CGameState(CConfig& config,
             IFileSystem& fileSystem,
             CInputDeviceMap& inputDevMap,
             CModelRepository* pModelRepo);
  virtual ~CGameState();

  const bool LoadResources(IFileSystem& fs);
  void Free();

  void ResetGame();

  // Inherited via IEngineState
  virtual void Update(const float timeDelta) override;

  virtual void UpdateRender(const float timeDelta) override;

  virtual void Render() const override;

  virtual void RenderUI() const override;

  virtual const bool IsDone() const override;

  virtual IEngineState * GetNext(CEngine & engine) override;
};

#endif // !__BITRACE_GAMESTATE_H__

