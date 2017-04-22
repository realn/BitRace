#include "stdafx.h"
#include "GameState.h"
#include "InputDevice.h"
#include "Config.h"
#include "FileSystem.h"
#include "GameEntity.h"

static const cb::string ENTTYPES_FILEPATH = L"entityTypes.xml";
static const cb::string ENTTYPES_ROOTNAME = L"EntityTypes";

CGameState::CGameState(CConfig& config, 
                       IFileSystem& fileSystem,
                       CInputDeviceMap& inputDevMap)
  : mConfig(config)
  , mIDevMap(inputDevMap) 
  , mRaceTrack(mEntityTypes)
{
  fileSystem.ReadXml(ENTTYPES_FILEPATH, ENTTYPES_ROOTNAME, mEntityTypes);
}

CGameState::~CGameState() {}

const bool CGameState::Init() {
  this->mRacer.Init((Uint32)ModelType::MT_HTTP20);
  this->mRacer.SetColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
  this->mRaceTrack.Init();
  this->mRaceTrack.SetRacer(&mRacer);
  

  return true;
}

void CGameState::Free() {
  mRaceTrack.Free();
  mRacer.Free();
}

void CGameState::ResetGame() {
  mRaceTrack.ResetGame();
}

const CRaceTrack & CGameState::GetRaceTrack() const {
  return mRaceTrack;
}

const bool CGameState::IsGameOver() const {
  return mRaceTrack.IsGameOver();
}

void CGameState::Update(const float timeDelta) {
  static bool down = false;
  if(mRaceTrack.IsGameRuning()) {
    float xdelta = mIDevMap.GetRange(InputDevice::Mouse, (Uint32)MouseType::AxisDelta, (Uint32)MouseAxisId::AxisX) * mConfig.Screen.Width;
    mRacer.ModRotation(xdelta);
    if(mIDevMap.GetState(InputDevice::Mouse, (Uint32)MouseType::ButtonPress, SDL_BUTTON_LEFT)) {
      mRaceTrack.FireWeapon();
    }
  }
  //if(mRaceTrack.IsGameOver()) {
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

  mRaceTrack.Update(timeDelta);
}

void CGameState::UpdateRender() {
}

void CGameState::Render() const {
  glm::mat4 proj = glm::perspective(glm::radians(50.0f),
                                    mConfig.Screen.GetAspectRatio(),
                                    1.f, 1000.0f);

  mRaceTrack.Render(proj);
}

void CGameState::RenderUI() const {
  //mRaceTrack.RenderUI();
}

const bool CGameState::IsDone() const {
  return false;
}

IEngineState * CGameState::GetNext(CEngine & engine) {
  return nullptr;
}
