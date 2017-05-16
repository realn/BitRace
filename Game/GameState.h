#ifndef __BITRACE_GAMESTATE_H__
#define __BITRACE_GAMESTATE_H__

#include <CBIO/Ptr.h>

#include "EngineState.h"
#include "FrameTimer.h"

class CConfig;
class IFileSystem;
class CInputDeviceMap;
class CModelRepository;

class CUIFont;
class CUIScreen;
class CUIProgressBar;
class CUIRect;
template<typename _Type> class CUITextNumber;

class CGameSkyBox;
class CGameLevel;
class CGameEntityType;
class CGameDifficultySetting;
class CGamePlayerType;
class CGamePlayer;
class CGameWeapon;

class CGameState :
  public IEngineState {
public:
  typedef std::map<cb::string, CGameEntityType> EntityTypeMapT;
  typedef std::map<cb::string, CGamePlayerType> PlayerTypeMapT;

private:
  CConfig& mConfig;
  CInputDeviceMap& mIDevMap;

  cb::ptr<CUIFont> mpFont;
  cb::ptr<CModelRepository> mpModelRepo;
  cb::ptr<CGameDifficultySetting> mpDiffSetting;
  cb::ptr<CGameSkyBox> mpSkyBox;
  cb::ptr<CGameLevel> mpLevel;
  cb::ptr<CGamePlayer> mpPlayer;

  EntityTypeMapT mEntityTypes;
  PlayerTypeMapT mPlayerTypes;
  CFrameTimer mSpawnTimer;
  Uint32 mPoints;

  cb::ptr<CUIScreen> mpMainUI;
  cb::ptr<CUITextNumber<Sint32>> mpFPSCounter;
  cb::ptr<CUIProgressBar> mpUIHealthBar;
  cb::ptr<CUITextNumber<Sint32>> mpUIPoints;

public:
  CGameState(CConfig& config,
             CInputDeviceMap& inputDevMap,
             CModelRepository* pModelRepo);
  virtual ~CGameState();

  const bool LoadResources(IFileSystem& fs);

  // Inherited via IEngineState
  virtual void Update(const float timeDelta) override;

  virtual void UpdateRender(const float timeDelta) override;

  virtual void Render() const override;

  virtual void RenderUI() const override;

  virtual const bool IsDone() const override;

  virtual IEngineState * GetNext(CEngine & engine) override;

private:
  void FireWeapon(const CGameWeapon& weapon);
};

#endif // !__BITRACE_GAMESTATE_H__

