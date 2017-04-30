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
                       CInputDeviceMap& inputDevMap)
  : mConfig(config)
  , mIDevMap(inputDevMap) 
  , mDiffSetting(nullptr)
  , mLevel(mEntityTypes, fileSystem)
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

  this->mRacer.Init((Uint32)ModelType::MT_HTTP20);
  this->mRacer.SetColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
  this->mLevel.Init();
  this->mLevel.SetRacer(&mRacer);

  mFPSCounter = mMainUI->GetItem<CUITextNumber<Sint32>>(L"fpsCounter");
  mUIHealthBar = mMainUI->GetItem<CUIProgressBar>(L"healthBar");
  mUIPoints = mMainUI->GetItem<CUITextNumber<Sint32>>(L"pointsDisplay");

  mDiffSetting->Reset();
  mSpawnTimer.SetLimit(mDiffSetting->GetCurrent().EntitySpawnPause);

  return true;
}

void CGameState::Free() {
  mLevel.Free();
  mRacer.Free();
}

void CGameState::ResetGame() {
  mLevel.ResetGame();
  mDiffSetting->Reset();
}

const CRaceTrack & CGameState::GetRaceTrack() const {
  return mLevel;
}

const bool CGameState::IsGameOver() const {
  return mLevel.IsGameOver();
}

void CGameState::Update(const float timeDelta) {
  static bool down = false;
  if(mLevel.IsGameRuning()) {
    float xdelta = mIDevMap.GetRange(InputDevice::Mouse, (Uint32)MouseType::AxisDelta, (Uint32)MouseAxisId::AxisX) * mConfig.Screen.Width;
    mRacer.ModRotation(xdelta);
    if(mIDevMap.GetState(InputDevice::Mouse, (Uint32)MouseType::ButtonPress, SDL_BUTTON_LEFT)) {
      mLevel.FireWeapon();
    }
  }
  //if(mLevel.IsGameOver()) {
  //  mMenuProcess.GetMenuManager().ForceSwitchToMenu(CMenuProcess::MENU_MAIN);
  //  mMenuProcess.GetMenuManager().GetMenu(CMenuProcess::MENU_MAIN)->GetMenuItem(CMenuProcess::MI_RETURN)->SetEnable(false);
  //  mHS.SetTempScore(m_RaceTrack.GetPoints());
  //  m_uGameState = GS_HIGH;
  //  return;
  //}

  //if(mIDevMap.GetState(InputDevice::Keyboard, (Uint32)KeyboardType::KeyPress, SDL_SCANCODE_ESCAPE)) {
  //  mMenuProcess.GetMenuManager().ForceSwitchToMenu(CMenuProcess::MENU_MAIN);
  //  mMenuProcess.GetMenuManager().GetMenu(CMenuProcess::MENU_MAIN)->GetMenuItem(CMenuProcess::MI_RETURN)->SetEnable(true);
  //  m_uGameState = GS_MENU;
  //  return;
  //}

  //if(timeDelta > 0.0f)
  //mFPSDT += timeDelta;

  if(mSpawnTimer.Update(timeDelta)) {
    cb::string entId = mDiffSetting->GetRandomEntity(std::rand());
    if(!entId.empty()) {
      mLevel.AddEntity(entId);
    }
  }

  mLevel.Update(timeDelta);
  mPoints = mLevel.GetPoints();
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
  //mFPSDT = 0.0f;

  mMainUI->UpdateRender(mFont);
}

void CGameState::Render() const {
  glm::mat4 proj = glm::perspective(glm::radians(50.0f),
                                    mConfig.Screen.GetAspectRatio(),
                                    1.f, 1000.0f);

  mLevel.Render(proj);
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
