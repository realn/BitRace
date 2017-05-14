#include "stdafx.h"
#include "GameState.h"
#include "Config.h"
#include "FileSystem.h"
#include "InputDevice.h"
#include "GameEntity.h"
#include "GameDifficulty.h"
#include "GamePlayer.h"
#include "UIScreen.h"
#include "UIItems.h"

static const cb::string DEFFONT_FILEPATH = L"font.xml";
static const cb::string DIFFSETTING_FILEPATH = L"diffSet.xml";
static const cb::string ENTTYPES_FILEPATH = L"entityTypes.xml";
static const cb::string UISCREEN_FILEPATH = L"screen.xml";

CGameState::CGameState(CConfig& config, 
                       IFileSystem& fileSystem,
                       CInputDeviceMap& inputDevMap,
                       CModelRepository* pModelRepo)
  : mConfig(config)
  , mIDevMap(inputDevMap) 
  , mpModelRepo(pModelRepo)
  , mLevel(pModelRepo, mEntityTypes, fileSystem)
  , mPoints(0)
{
  mpDiffSetting = new CGameDifficultySetting();
  mpMainUI = new CUIScreen(config.Screen.GetSize());

  {
    CGamePlayerType type;
    type.Name = L"Http1.0";
    type.ModelFile = L"mdl_http10.xml";
    type.MaxHealth = 100.0f;
    type.Speed = glm::vec2(50.0f, 120.0f);
    type.Color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
    type.Weapon.ProjectileNumber = 1;
    type.Weapon.ProjectileDamage = 15.0f;
    type.Weapon.ProjectileColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

    mpPlayer = new CGamePlayer(type);

    CGamePlayerType::TypeMapT playerTypeMap;
    playerTypeMap[L"http10"] = type;

    CGamePlayerType::Save(playerTypeMap, fileSystem, L"playerTypes.xml");
  }

}

CGameState::~CGameState() {
  mpPlayer.Delete();
  mpDiffSetting.Delete();
  mpMainUI.Delete();
}

const bool CGameState::LoadResources(IFileSystem& fs) {
  if(!mFont.Load(fs, DEFFONT_FILEPATH)) {
    return false;
  }
  if(!CGameEntityType::Load(mEntityTypes, fs, ENTTYPES_FILEPATH)) {
    return false;
  }
  
  if(!mpMainUI->Load(fs, UISCREEN_FILEPATH)) {
    return false;
  }
  if(!mpDiffSetting->Load(fs, DIFFSETTING_FILEPATH)) {
    return false;
  }
  if(!mpPlayer->LoadResources(*mpModelRepo)) {
    return false;
  }

  mLevel.Init();

  mpFPSCounter = mpMainUI->GetItem<CUITextNumber<Sint32>>(L"fpsCounter");
  mpUIHealthBar = mpMainUI->GetItem<CUIProgressBar>(L"healthBar");
  mpUIPoints = mpMainUI->GetItem<CUITextNumber<Sint32>>(L"pointsDisplay");

  mpDiffSetting->Reset();
  mSpawnTimer.SetLimit(mpDiffSetting->GetCurrent().EntitySpawnPause);
  mSpawnTimer.SetEnabled(true);

  return true;
}

void CGameState::Free() {
  mLevel.Free();
  mpPlayer->Free();
}

void CGameState::ResetGame() {
  mpDiffSetting->Reset();
}

void CGameState::Update(const float timeDelta) {
  float xdelta = mIDevMap.GetRange(InputDevice::Mouse, (Uint32)MouseType::AxisDelta, (Uint32)MouseAxisId::AxisX) * mConfig.Screen.Width;
  mpPlayer->ModRotation(-xdelta);
  if(mIDevMap.GetState(InputDevice::Mouse, (Uint32)MouseType::ButtonPress, SDL_BUTTON_LEFT)) {
    mLevel.FireWeapon();
  }

  if(mSpawnTimer.Update(timeDelta)) {
    cb::string entId = mpDiffSetting->GetRandomEntity(std::rand());
    if(!entId.empty()) {
      mLevel.AddEntity(entId);
    }
  }

  mpPlayer->Update(timeDelta);

  mBackground.SetSepHeight(40.0f);
  mBackground.SetDynamicVec(mpPlayer->GetDirection() * glm::vec3(-1.0f, 1.0f, 1.0f));
  mBackground.Update(timeDelta);

  mLevel.Update(*mpPlayer, timeDelta);
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

  mBackground.UpdateRender();

  mpMainUI->UpdateRender(mFont);
}

void CGameState::Render() const {
  glm::mat4 proj = glm::perspective(glm::radians(50.0f),
                                    mConfig.Screen.GetAspectRatio(),
                                    1.f, 1000.0f);
  glm::mat4 mat = proj *
    glm::translate(glm::vec3(0.0f, 12.0f, -12.0f)) *
    glm::rotate(glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  mBackground.Render(mat);
  
  mat *= glm::translate(glm::vec3(0.0f, -17.0f, 0.0f));

  mLevel.Render(mat);
  mpPlayer->Render(mat);
}

void CGameState::RenderUI() const {
  mpMainUI->Render(mFont);
}

const bool CGameState::IsDone() const {
  return false;
}

IEngineState * CGameState::GetNext(CEngine & engine) {
  return nullptr;
}
