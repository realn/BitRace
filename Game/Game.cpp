#include "stdafx.h"
//#include "Game.h"
//#include "InputDevice.h"
//#include "Config.h"
//
//CGameProcess::CGameProcess(CConfig& config, CInputDeviceMap& inputDevMap)
//  : mConfig(config)
//  , mIDevMap(inputDevMap) {}
//
//CGameProcess::~CGameProcess() {}
//
//const bool CGameProcess::Init() {
//  this->mRacer.Init((Uint32)ModelType::MT_HTTP20);
//  this->mRacer.SetColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
//  this->mRaceTrack.Init();
//  this->mRaceTrack.SetRacer(&mRacer);
//  return true;
//}
//
//void CGameProcess::Free() {
//  mRaceTrack.Free();
//  mRacer.Free();
//}
//
//void CGameProcess::ResetGame() {
//  mRaceTrack.ResetGame();
//}
//
//const CRaceTrack & CGameProcess::GetRaceTrack() const {
//  return mRaceTrack;
//}
//
//const bool CGameProcess::IsGameOver() const {
//  return mRaceTrack.IsGameOver();
//}
//
//void CGameProcess::Update(const float timeDelta) {
//  static bool down = false;
//  if(mRaceTrack.IsGameRuning()) {
//    float xdelta = mIDevMap.GetRange(InputDevice::Mouse, (Uint32)MouseType::AxisDelta, (Uint32)MouseAxisId::AxisX) * mConfig.Screen.Width;
//    mRacer.ModRotation(xdelta);
//    if(mIDevMap.GetState(InputDevice::Mouse, (Uint32)MouseType::ButtonPress, SDL_BUTTON_LEFT)) {
//      mRaceTrack.FireWeapon();
//    }
//  }
//  //if(mRaceTrack.IsGameOver()) {
//  //  mMenuProcess.GetMenuManager().ForceSwitchToMenu(CMenuProcess::MENU_MAIN);
//  //  mMenuProcess.GetMenuManager().GetMenu(CMenuProcess::MENU_MAIN)->GetMenuItem(CMenuProcess::MI_RETURN)->SetEnable(false);
//  //  mHS.SetTempScore(m_RaceTrack.GetPoints());
//  //  m_uGameState = GS_HIGH;
//  //  return;
//  //}
//
//  //if(mIDevMap.GetState(InputDevice::Keyboard, (Uint32)KeyboardType::KeyPress, SDL_SCANCODE_ESCAPE)) {
//  //  mMenuProcess.GetMenuManager().ForceSwitchToMenu(CMenuProcess::MENU_MAIN);
//  //  mMenuProcess.GetMenuManager().GetMenu(CMenuProcess::MENU_MAIN)->GetMenuItem(CMenuProcess::MI_RETURN)->SetEnable(true);
//  //  m_uGameState = GS_MENU;
//  //  return;
//  //}
//
//  mRaceTrack.Update(timeDelta);
//}
//
//CGameView::CGameView(CGameProcess& gameProcess) 
//  : mProcess(gameProcess)
//{}
//
//CGameView::~CGameView() {}
//
//void CGameView::Render(const glm::mat4 & transform) const {
//  mProcess.GetRaceTrack().Render(transform);
//}
//
//void CGameView::RenderUI(CGUI & gui) const {
//  mProcess.GetRaceTrack().RenderUI(gui);
//}
