#include "stdafx.h"
#include "GameState.h"
#include "GameSkyBox.h"
#include "GameLevel.h"
#include "GameEntity.h"
#include "GameDifficulty.h"
#include "GamePlayer.h"
#include "GameInputDefines.h"
#include "Config.h"
#include "UIFont.h"
#include "UIScreen.h"
#include "UIItems.h"

#include <glm/gtx/vector_angle.hpp>

static const cb::string DEFFONT_FILEPATH = L"font.xml";
static const cb::string DIFFSETTING_FILEPATH = L"diffSet.xml";
static const cb::string ENTTYPES_FILEPATH = L"entityTypes.xml";
static const cb::string UISCREEN_FILEPATH = L"screen.xml";
static const cb::string PLAYERTYPES_FILEPATH = L"playerTypes.xml";

CGameState::CGameState(CConfig& config,
                       CModelRepository* pModelRepo)
  : mConfig(config)
  , mpModelRepo(pModelRepo)
  , mPoints(0) {
  mpFont = new CUIFont();
  mpDiffSetting = new CGameDifficultySetting();

  mpSkyBox = new CGameSkyBox();
  mpLevel = new CGameLevel(mpModelRepo, &mEntityTypes);

  mpMainUI = new CUIScreen(config.Screen.GetSize());
}

CGameState::~CGameState() {
  mpMainUI.Delete();
  mpSkyBox.Delete();
  mpLevel.Delete();
  mpDiffSetting.Delete();
  mpFont.Delete();
}

const bool CGameState::LoadResources(IFileSystem& fs) {
  if(!mpFont->Load(fs, DEFFONT_FILEPATH)) {
    return false;
  }
  if(!mpMainUI->Load(fs, UISCREEN_FILEPATH)) {
    return false;
  }
  if(!mpDiffSetting->Load(fs, DIFFSETTING_FILEPATH)) {
    return false;
  }
  if(!CGameEntityType::Load(mEntityTypes, fs, ENTTYPES_FILEPATH)) {
    return false;
  }
  if(!CGamePlayerType::Load(mPlayerTypes, fs, PLAYERTYPES_FILEPATH)) {
    return false;
  }

  mpPlayer.Delete();
  mpPlayer = new CGamePlayer(mPlayerTypes[L"http10"]);
  if(!mpPlayer->LoadResources(*mpModelRepo)) {
    return false;
  }

  mpFPSCounter = mpMainUI->GetItem<CUITextValue<Sint32>>(L"fpsCounter");
  mpUIHealthBar = mpMainUI->GetItem<CUIProgressBar>(L"healthBar");
  mpUIPoints = mpMainUI->GetItem<CUITextValue<Sint32>>(L"pointsDisplay");
  mpUILevel = mpMainUI->GetItem<CUITextValue<cb::string>>(L"levelDisplay");

  mpDiffSetting->Reset();
  mSpawnTimer.SetLimit(mpDiffSetting->GetCurrent().EntitySpawnPause);
  mSpawnTimer.SetEnabled(true);

  return true;
}

void CGameState::ModPlayerPoints(const Sint32 value) {
  if(value < 0 && (Uint32)glm::abs(value) >= mPoints) {
      mPoints = 0;
  }
  else {
    mPoints += value;
  }
  if(mpDiffSetting->CanPass(mPoints)) {
    mpDiffSetting->Next();
    mSpawnTimer.SetLimit(mpDiffSetting->GetCurrent().EntitySpawnPause);
  }
}

void CGameState::Update(const float timeDelta) {
  if(mSpawnTimer.Update(timeDelta)) {
    cb::string entId = mpDiffSetting->GetRandomEntity(std::rand());
    if(!entId.empty()) {
      mpLevel->AddEntity(entId);
    }
  }

  mpPlayer->Update(timeDelta);
  glm::vec3 skyBoxDir(-mpPlayer->GetDir().x, 0.0f, mpPlayer->GetDir().y);

  mpSkyBox->SetSepHeight(40.0f);
  mpSkyBox->SetDynamicVec(skyBoxDir);
  mpSkyBox->Update(timeDelta);

  mpLevel->Update(glm::vec2(mpPlayer->GetDir().x, 0.0f), *this, timeDelta);

  CheckCollisions(2.0f);
}

void CGameState::UpdateRender(const float timeDelta) {
  if(timeDelta > 0.0f && mpFPSCounter) {
    Uint32 fps = (Uint32)(1.0f / timeDelta);
    mpFPSCounter->SetValue(fps);
  }
  if(mpUIHealthBar) {
    mpUIHealthBar->SetRange(0.0f, mpPlayer->GetMaxHealth());
    mpUIHealthBar->SetValue(mpPlayer->GetHealth());
  }
  if(mpUIPoints) {
    mpUIPoints->SetValue((Sint32)mPoints);
  }
  if(mpUILevel) {
    mpUILevel->SetValue(mpDiffSetting->GetCurrent().Name);
  }

  mpSkyBox->UpdateRender();
  mpLevel->UpdateRender();
  mpMainUI->UpdateRender(*mpFont);
}

void CGameState::Render() const {
  static const glm::mat4 transSkybox =  
    glm::translate(glm::vec3(0.0f, 12.0f, -12.0f)) *
    glm::rotate(glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  static const glm::mat4 transLevel =
    transSkybox *
    glm::translate(glm::vec3(0.0f, -17.0f, 0.0f));

  glm::mat4 proj = glm::perspective(glm::radians(50.0f),
                                    mConfig.Screen.GetAspectRatio(),
                                    1.f, 1000.0f);
  mpSkyBox->Render(proj * transSkybox);

  {
    glm::mat4 trans = proj * transLevel;
    mpLevel->Render(trans);
    mpPlayer->Render(trans);
  }
}

void CGameState::RenderUI() const {
  mpMainUI->Render(*mpFont);
}

const bool CGameState::IsDone() const {
  return false;
}

IEngineState * CGameState::GetNext(CEngine & engine) {
  return nullptr;
}

void CGameState::FireWeapon(const CGameWeapon& weapon) {
  glm::vec2 startPos = glm::vec2(0.0f, -2.0f);
  glm::vec2 dir = glm::vec2(0.0f, -1.0f);
  if(weapon.ProjectileNumber == 1) {
    mpLevel->AddProjectile(startPos, dir,
                           weapon.ProjectileColor,
                           weapon.ProjectileSpeed,
                           weapon.ProjectileDamage);
  }
  else {
    for(Uint32 i = 0; i < weapon.ProjectileNumber; i++) {
      float angle = -6.0f * float(weapon.ProjectileNumber - 1) / 2.0f + float(i) * 6.0f;
      dir = glm::rotate(glm::vec2(0.0f, -1.0f), glm::radians(angle));

      mpLevel->AddProjectile(startPos, dir,
                             weapon.ProjectileColor,
                             weapon.ProjectileSpeed,
                             weapon.ProjectileDamage);
    }
  }
}

void CGameState::CheckCollisions(const float playerRadius) {
  mpLevel->CheckEntityCollisions(*mpPlayer, playerRadius, *this);
  mpLevel->CheckProjectileCollisions(*mpPlayer, playerRadius, *this);

  typedef std::vector<CGameEntity> EntityVectorT;
  EntityVectorT& entities = mpLevel->GetEntities();
  for(EntityVectorT::iterator it = entities.begin(); it != entities.end(); it++) {
    if(it->IsDeleted()) {
      continue;
    }

    mpLevel->CheckEntityCollisions(*it, it->GetCollisionRadius(), *this);
    mpLevel->CheckProjectileCollisions(*it, it->GetCollisionRadius(), *this);
  }
}

void CGameState::ExecuteEvents(CGameObject & thisObj, CGameObject * pSenderObj, const EventVecT & events) {
  for(EventVecT::const_iterator it = events.begin(); it != events.end(); it++) {
    ExecuteEvent(thisObj, pSenderObj, *it);
  }
}

void CGameState::ExecuteEvent(CGameObject & thisObj, CGameObject * pSenderObj, const CGameObjectEvent & event) {
  for(CGameObjectEvent::ActionVectorT::const_iterator it = event.Actions.begin();
      it != event.Actions.end(); it++) {
    CGameObject* pTargetObj = GetEventActionTarget(it->Target,
                                                   thisObj,
                                                   pSenderObj);
    ExecuteEventAction(it->Action, pTargetObj, it->Value);
  }
}

void CGameState::ExecuteEventAction(const GameEventActionType type, CGameObject * pTargetObj, const float value) {
  switch(type) {
  case GameEventActionType::AddPoints:
    ModPlayerPoints((Sint32)value);
    break;

  case GameEventActionType::RemPoints:
    ModPlayerPoints(-(Sint32)value);
    break;

  case GameEventActionType::Heal:
    {
      CGameActor* pActor = dynamic_cast<CGameActor*>(pTargetObj);
      if(pActor) {
        pActor->ModHealth(value);
      }
    }
    break;

  case GameEventActionType::Damage:
    {
      CGameActor* pActor = dynamic_cast<CGameActor*>(pTargetObj);
      if(pActor) {
        pActor->ModHealth(-value);
      }
    }
    break;

  case GameEventActionType::Kill:
    {
      CGameActor* pActor = dynamic_cast<CGameActor*>(pTargetObj);
      if(pActor) {
        pActor->SetHealth(0.0f);
      }
    }
    break;

  case GameEventActionType::Delete:
    if(pTargetObj) {
      pTargetObj->Delete();
    }
    break;

  default:
    break;
  }
}

CGameObject * CGameState::GetEventActionTarget(const GameEventActionTarget target, CGameObject & thisObj, CGameObject * pSenderObj) {
  switch(target) {
  case GameEventActionTarget::This:   return &thisObj;
  case GameEventActionTarget::Sender: return pSenderObj;
  case GameEventActionTarget::Player: return mpPlayer;
  default:
    return nullptr;
  }
}

void CGameState::TriggerEvent(const GameEventTrigger triggerType,
                              CGameObject & thisObj,
                              CGameObject * pSenderObj) {
  GameObjectType senderType =
    (pSenderObj == nullptr) ? GameObjectType::Unknown : pSenderObj->GetType();
  EventVecT events = thisObj.GetEvents(triggerType, senderType);
  if(!events.empty()) {
    ExecuteEvents(thisObj, pSenderObj, events);
  }
}

void CGameState::OnInputEvent(const CInputEvent & event) {
  if(event.Context != INPUT_CTX_PLAYER)
    return;

  if(event.Id == INPUT_TGT_PLAYER_ROTATEX && 
     event.Type == InputEventType::Range) {
    float xdelta = event.Data.Range.Value - event.Data.Range.ValuePrev;
    mpPlayer->ModRotation(-xdelta * (float)mConfig.Screen.Width);
  }
  else if(event.Id == INPUT_TGT_PLAYER_ATTACK && 
          event.Type == InputEventType::Action) {
    FireWeapon(mpPlayer->GetWeapon());
  }
}
