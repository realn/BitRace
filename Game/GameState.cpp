#include "stdafx.h"
#include "GameState.h"
#include "Config.h"
#include "FileSystem.h"
#include "InputDevice.h"
#include "GameEntity.h"
#include "GameDifficulty.h"
#include "UIScreen.h"
#include "UIItems.h"
#include "UIScreenXml.h"

static const cb::string DEFFONT_FILEPATH = L"font.xml";
static const cb::string DIFFSETTING_FILEPATH = L"diffSet.xml";
static const cb::string ENTTYPES_FILEPATH = L"entityTypes.xml";
static const cb::string ENTTYPES_ROOTNAME = L"EntityTypes";
static const cb::string UISCREEN_FILEPATH = L"screen.xml";
static const cb::string UISCREEN_ROOTNAME = L"Screen";

CGameState::CGameState(CConfig& config, 
                       IFileSystem& fileSystem,
                       CInputDeviceMap& inputDevMap,
                       CModelRepository* pModelRepo)
  : mConfig(config)
  , mIDevMap(inputDevMap) 
  , mModelRepo(pModelRepo)
  , mDiffSetting(nullptr)
  , mLevel(pModelRepo, mEntityTypes, fileSystem)
  , mPoints(0)
  , mMainUI(nullptr)
{
  mDiffSetting = new CGameDifficultySetting();
  mMainUI = new CUIScreen(config.Screen.GetSize());
}

CGameState::~CGameState() {
  delete mDiffSetting;
  delete mMainUI;
}

const bool CGameState::LoadResources(IFileSystem& fs) {
  if(!mFont.Load(fs, DEFFONT_FILEPATH)) {
    return false;
  }
  if(!fs.ReadXml(ENTTYPES_FILEPATH, ENTTYPES_ROOTNAME, mEntityTypes)) {
    return false;
  }
  if(!fs.ReadXml(UISCREEN_FILEPATH, UISCREEN_ROOTNAME, *mMainUI)) {
    return false;
  }
  if(!mDiffSetting->Load(fs, DIFFSETTING_FILEPATH)) {
    return false;
  }

  mRacer.Init((Uint32)ModelType::MT_HTTP20, *mModelRepo, L"mdl_http10.xml");
  mRacer.SetColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
  mLevel.Init();

  mFPSCounter = mMainUI->GetItem<CUITextNumber<Sint32>>(L"fpsCounter");
  mUIHealthBar = mMainUI->GetItem<CUIProgressBar>(L"healthBar");
  mUIPoints = mMainUI->GetItem<CUITextNumber<Sint32>>(L"pointsDisplay");

  mDiffSetting->Reset();
  mSpawnTimer.SetLimit(mDiffSetting->GetCurrent().EntitySpawnPause);
  mSpawnTimer.SetEnabled(true);

  return true;
}

void CGameState::Free() {
  mLevel.Free();
  mRacer.Free();
}

void CGameState::ResetGame() {
  mDiffSetting->Reset();
}

void CGameState::Update(const float timeDelta) {
  float xdelta = mIDevMap.GetRange(InputDevice::Mouse, (Uint32)MouseType::AxisDelta, (Uint32)MouseAxisId::AxisX) * mConfig.Screen.Width;
  mRacer.ModRotation(xdelta);
  if(mIDevMap.GetState(InputDevice::Mouse, (Uint32)MouseType::ButtonPress, SDL_BUTTON_LEFT)) {
    mLevel.FireWeapon();
  }

  if(mSpawnTimer.Update(timeDelta)) {
    cb::string entId = mDiffSetting->GetRandomEntity(std::rand());
    if(!entId.empty()) {
      mLevel.AddEntity(entId);
    }
  }

  mRacer.Engine(timeDelta);

  mBackground.SetSepHeight(40.0f);
  mBackground.SetDynamicVec(mRacer.GetVec() * glm::vec3(-1.0f, 1.0f, 1.0f));
  mBackground.Update(timeDelta);

  mLevel.Update(mRacer, timeDelta);
}

void CGameState::UpdateRender(const float timeDelta) {
  if(timeDelta > 0.0f && mFPSCounter) {
    Uint32 fps = (Uint32)(1.0f / timeDelta);
    mFPSCounter->SetValue(fps);
  }
  if(mUIHealthBar) {
    mUIHealthBar->SetValue(mRacer.GetBitRate());
  }
  if(mUIPoints) {
    mUIPoints->SetValue((Sint32)mPoints);
  }

  mBackground.UpdateRender();

  mMainUI->UpdateRender(mFont);
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
  mRacer.Render(mat);
}

void CGameState::RenderUI() const {
  mMainUI->Render(mFont);
}

const bool CGameState::IsDone() const {
  return false;
}

IEngineState * CGameState::GetNext(CEngine & engine) {
  return nullptr;
}
