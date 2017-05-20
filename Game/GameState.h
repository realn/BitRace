#ifndef __BITRACE_GAMESTATE_H__
#define __BITRACE_GAMESTATE_H__

#include <CBIO/Ptr.h>

#include "EngineState.h"
#include "FrameTimer.h"
#include "GameObjectDefines.h"

class CConfig;
class IFileSystem;
class CInputDeviceMap;
class CModelRepository;

class CUIFont;
class CUIScreen;
class CUIProgressBar;
class CUIRect;
template<typename _Type> class CUITextValue;

class CGameSkyBox;
class CGameLevel;
class CGameEntityType;
class CGameDifficultySetting;
class CGamePlayerType;
class CGamePlayer;
class CGameWeapon;
class CGameObject;
class CGameObjectEvent;

class CGameState 
  : public IEngineState 
  , public IGameObjectEventObserver
{
public:
  typedef std::map<cb::string, CGameEntityType> EntityTypeMapT;
  typedef std::map<cb::string, CGamePlayerType> PlayerTypeMapT;
  typedef std::vector<CGameObjectEvent> EventVecT;

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
  cb::ptr<CUITextValue<Sint32>> mpFPSCounter;
  cb::ptr<CUIProgressBar> mpUIHealthBar;
  cb::ptr<CUITextValue<Sint32>> mpUIPoints;
  cb::ptr<CUITextValue<cb::string>> mpUILevel;

public:
  CGameState(CConfig& config,
             CInputDeviceMap& inputDevMap,
             CModelRepository* pModelRepo);
  virtual ~CGameState();

  const bool LoadResources(IFileSystem& fs);

  void ModPlayerPoints(const Sint32 value);

  // Inherited via IEngineState
  virtual void Update(const float timeDelta) override;

  virtual void UpdateRender(const float timeDelta) override;

  virtual void Render() const override;

  virtual void RenderUI() const override;

  virtual const bool IsDone() const override;

  virtual IEngineState * GetNext(CEngine & engine) override;

private:
  void FireWeapon(const CGameWeapon& weapon);
  void CheckCollisions(const float playerRadius);
  void ExecuteEvents(CGameObject& thisObj, CGameObject* pSenderObj, const EventVecT& events);  
  void ExecuteEvent(CGameObject& thisObj, CGameObject* pSenderObj, const CGameObjectEvent& event);
  void ExecuteEventAction(const GameEventActionType type, 
                          CGameObject* pTargetObj, const float value);
  CGameObject* GetEventActionTarget(const GameEventActionTarget target,
                                    CGameObject& thisObj,
                                    CGameObject* pSenderObj);

  // Inherited via IGameObjectEventObserver
  virtual void TriggerEvent(const GameEventTrigger triggerType, CGameObject & thisObj, CGameObject * pSenderObj = nullptr) override;
};

#endif // !__BITRACE_GAMESTATE_H__

