#include "stdafx.h"
#include "GameState.h"
#include "InputDevice.h"
#include "Config.h"
#include "FileSystem.h"
#include "GameEntity.h"
#include "UIScreen.h"
#include "UIItems.h"
#include "UIScreenXml.h"

static const cb::string ENTTYPES_FILEPATH = L"entityTypes.xml";
static const cb::string ENTTYPES_ROOTNAME = L"EntityTypes";

CGameState::CGameState(CConfig& config, 
                       IFileSystem& fileSystem,
                       CInputDeviceMap& inputDevMap)
  : mConfig(config)
  , mIDevMap(inputDevMap) 
  , mRaceTrack(mEntityTypes)
  , mMainUI(nullptr)
  , mFPSDT(0.0f)
{
  fileSystem.ReadXml(ENTTYPES_FILEPATH, ENTTYPES_ROOTNAME, mEntityTypes);
  mFont.Load(fileSystem, L"font.xml");

  mMainUI = new CUIScreen(config.Screen.GetSize(), glm::vec4(50.0f));
  fileSystem.ReadXml(L"screen.xml", L"Screen", *mMainUI);
  mFPSCounter = mMainUI->GetItem<CUITextNumber<Sint32>>(L"fpsCounter");

  //{
  //  CUIItemList* pItemList = new CUIItemList();
  //  pItemList->SetMargin(glm::vec2(8.0f, 3.0f));
  //  {
  //    CUIStack* pStack = new CUIStack(UIOrientation::Vertical);
  //    pStack->SetMargin(glm::vec2(10.0f, 5.0f));
  //    pStack->AddItem(new CUIText(L"POINTS: 0"));
  //    pStack->AddItem(new CUIText(L"DIFFCULTY LEVEL: EASY"));
  //    CUIPanel* pPanel = new CUIPanel(pStack,
  //                                    glm::vec4(0.4f, 0.4f, 1.0f, 0.6f),
  //                                    glm::vec2(400.0f, 40.0f));

  //    pItemList->AddItem(pPanel, UIHAlign::Left, UIVAlign::Top);

  //    mFPSCounter = new CUIText(L"FPS: 0");
  //    pItemList->AddItem(mFPSCounter, UIHAlign::Right, UIVAlign::Top);
  //  }
  //  mMainUI->SetItem(pItemList);
  //}
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
  if(mMainUI) {
    delete mMainUI;
    mMainUI = nullptr;
  }
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

  //if(timeDelta > 0.0f)
  //mFPSDT += timeDelta;
  mRaceTrack.Update(timeDelta);
}

void CGameState::UpdateRender(const float timeDelta) {
  if(timeDelta > 0.0f && mFPSCounter) {
    Uint32 fps = (Uint32)(1.0f / timeDelta);
    mFPSCounter->SetValue(fps);
  }
  //mFPSDT = 0.0f;

  mMainUI->UpdateRender(mFont);
}

void CGameState::Render() const {
  glm::mat4 proj = glm::perspective(glm::radians(50.0f),
                                    mConfig.Screen.GetAspectRatio(),
                                    1.f, 1000.0f);

  mRaceTrack.Render(proj);
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
