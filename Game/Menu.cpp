#include "stdafx.h"
#include "Menu.h"
#include "RaceTrack.h"
#include "Config.h"

CMenuProcess::CMenuProcess(CGUI& gui, CInputDeviceMap& inputDevMap)
  : mPauseMenu(false)
  , mMenuMng(&gui)
  , mIDevMap(inputDevMap) {}

CMenuProcess::~CMenuProcess() {}

const bool CMenuProcess::Init(const CConfig& config) {
  char szBuffer[1000];
  mMenuMng.SetSize(config.Screen.GetSize());

  CGUIMenu* Menu = mMenuMng.AddMenu(MENU_MAIN, "BitRace");
  Menu->AddMenuItem(MI_RETURN, "Return to Game", glm::vec2(40.0f, 70.0f), 0)->SetEnable(false);
  Menu->AddMenuItem(MI_NEWGAME, "New Game", glm::vec2(40.0f, 100.0f), 0);
  Menu->AddMenuItem(MI_HIGH, "High Scores", glm::vec2(40.0f, 130.0f), MENU_HIGH);
  Menu->AddMenuItem(MI_OPTIONS, "Options", glm::vec2(40.0f, 160.0f), MENU_OPTIONS);
  Menu->AddMenuItem(MI_EXIT, "Exit Game", glm::vec2(40.0f, 190.0f), 0);
  Menu = mMenuMng.AddMenu(MENU_OPTIONS, "Options");
  sprintf_s(szBuffer, 1000, "Resolution: %d X %d",
            config.Screen.Width, config.Screen.Height);

  Menu->AddMenuItem(MI_RESOLUTION, szBuffer, glm::vec2(40.0f, 100.0f), config.Screen.DevId);

  Menu->AddCheckBox(MI_FULLSCREEN, "Fullscreen", config.Screen.Fullscreen, glm::vec2(40.0f, 130.0f));
  Menu->AddCheckBox(MI_SMOOTHSHADE, "Smooth Shading", config.Render.SmoothShade, glm::vec2(40.0f, 160.0f), 0);
  Menu->AddCheckBox(MI_SMOOTHLINE, "Smooth Lines", config.Render.SmoothLines, glm::vec2(40.0f, 190.0f), 0);
  Menu->AddCheckBox(MI_FPSCOUNTER, "FPS Counter", config.Diag.FPSCounter, glm::vec2(40.0f, 220.0f), 0);
  Menu->AddCheckBox(MI_VSYNC, "Vertical Sync", config.Screen.VSync, glm::vec2(40.0f, 250.0f), 0);

  //if(WGLEW_EXT_swap_control) {
  //  Menu->GetMenuItem(MI_VSYNC)->SetEnable(false);
  //}

  Menu->AddMenuItem(MI_OPWARNING, "WARNING: You must restart the game, to apply changes", glm::vec2(20.0f, 300.0f), 0)->SetEnable(false);
  Menu->AddMenuItem(MI_GOBACK, "Return to Main Menu", glm::vec2(40.0f, 330.0f), MENU_MAIN);
  Menu = mMenuMng.AddMenu(MENU_HIGH, "High Scores");
  Menu->AddMenuItem(MI_HS1, "1. --EMPTY SCORE--", glm::vec2(40.0f, 100.0f), 0);
  Menu->AddMenuItem(MI_HS2, "2. --EMPTY SCORE--", glm::vec2(40.0f, 120.0f), 0);
  Menu->AddMenuItem(MI_HS3, "3. --EMPTY SCORE--", glm::vec2(40.0f, 140.0f), 0);
  Menu->AddMenuItem(MI_HS4, "4. --EMPTY SCORE--", glm::vec2(40.0f, 160.0f), 0);
  Menu->AddMenuItem(MI_HS5, "5. --EMPTY SCORE--", glm::vec2(40.0f, 180.0f), 0);
  Menu->AddMenuItem(MI_HS6, "6. --EMPTY SCORE--", glm::vec2(40.0f, 200.0f), 0);
  Menu->AddMenuItem(MI_HS7, "7. --EMPTY SCORE--", glm::vec2(40.0f, 220.0f), 0);
  Menu->AddMenuItem(MI_HS8, "8. --EMPTY SCORE--", glm::vec2(40.0f, 240.0f), 0);
  Menu->AddMenuItem(MI_HS9, "9. --EMPTY SCORE--", glm::vec2(40.0f, 260.0f), 0);
  Menu->AddMenuItem(MI_HS10, "10. --EMPTY SCORE--", glm::vec2(40.0f, 280.0f), 0);
  Menu->AddMenuItem(MI_HSRESET, "Reset Hight Scores", glm::vec2(40.0f, 400.0f), 0);
  Menu->AddMenuItem(MI_GOBACK, "Return to Main Menu", glm::vec2(40.0f, 440.0f), MENU_MAIN);

  return true;
}

void CMenuProcess::AddObserver(IMenuEventObserver * pObserver) {
  if(std::find(mObservers.begin(), mObservers.end(), pObserver) == mObservers.end()) {
    mObservers.push_back(pObserver);
  }
}

void CMenuProcess::RemoveObserver(IMenuEventObserver * pObserver) {
  ObserverVectorT::iterator it = find(mObservers.begin(),
                                      mObservers.end(),
                                      pObserver);
  if(it != mObservers.end()) {
    mObservers.erase(it);
  }
}

void CMenuProcess::SetPauseMenu(const bool value) {
  mPauseMenu = value;
}

const bool CMenuProcess::IsPauseMenu() const {
  return mPauseMenu;
}

CGUIMenuManager & CMenuProcess::GetMenuManager() {
  return mMenuMng;
}

void CMenuProcess::Update(const float timeDelta) {
  static bool down = false;
  if(mMenuMng.Update(mIDevMap, timeDelta)) {
    if(down)
      return;
    CGUIMenu* pMenu = mMenuMng.GetCurrentMenu();
    CGUIMenuItem* pItem = pMenu->GetMenuItem(pMenu->GetClickedID());

    for(ObserverVectorT::iterator it = mObservers.begin(); it != mObservers.end(); it++) {
      (*it)->MenuItemAction(mMenuMng, *pMenu, *pItem);
    }
    down = true;
  }
  else down = false;
}

CMenuView::CMenuView(CMenuProcess& menuProcess)
  : mMenuProcess(menuProcess) {}

CMenuView::~CMenuView() {}

const bool CMenuView::Init(const glm::uvec2 & screenSize) {
  mScreenSize = screenSize;
  return true;
}

void CMenuView::Render(const glm::mat4& transform) const {
  if(mMenuProcess.IsPauseMenu()) {
    //mRaceTrack.Render(transform);
  }
}

void CMenuView::RenderUI(CGUI & gui) const {
  if(mMenuProcess.IsPauseMenu()) {
    gui.RenderQuadFullScreen(mScreenSize, glm::vec4(0.0f, 0.0f, 0.0f, 0.4f));
  }
  mMenuProcess.GetMenuManager().Render();
}
