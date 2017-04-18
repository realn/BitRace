#include "stdafx.h"
#include "Menu.h"
#include "Config.h"
#include "UIMenu.h"
#include "InputDevice.h"
#include "MeshFunctions.h"

CMenuProcess::CMenuProcess(CInputDeviceMap& inputDevMap) 
  : mpCurrentMenu(nullptr)
  , mIDevMap(inputDevMap)
{
}

CMenuProcess::~CMenuProcess() {
  Free();
}

const CUIFont & CMenuProcess::GetFont() const {
  return mFont;
}

const CUIMenu * CMenuProcess::GetCurrent() const {
  return mpCurrentMenu;
}

const glm::vec2 CMenuProcess::GetPointerPos() const {
  return mPointerPos;
}

const CUIMenuLayout & CMenuProcess::GetLayout() const {
  return mLayout;
}

const bool CMenuProcess::Init() {

  CUIMenu* pMenu = new CUIMenu(L"BIT_RACE");
  pMenu->AddItem(1, L"New Game");
  pMenu->AddItem(2, L"Exit");
  mMenuMap[L"MAIN"] = pMenu;

  mpCurrentMenu = pMenu;
  return true;
}

void CMenuProcess::Free() {
  for(MenuMapT::iterator it = mMenuMap.begin(); it != mMenuMap.end(); it++) {
    delete it->second;
  }
  mMenuMap.clear();
}


void CMenuProcess::Update(const float timeDelta) {
  mPointerPos = glm::vec2(
    mIDevMap.GetRange(InputDevice::Mouse, (Uint32)MouseType::AxisPos, (Uint32)MouseAxisId::AxisX),
    mIDevMap.GetRange(InputDevice::Mouse, (Uint32)MouseType::AxisPos, (Uint32)MouseAxisId::AxisY)
    );

  if(mpCurrentMenu == nullptr)
    return;

  for(CUIMenu::iterator it = mpCurrentMenu->Begin(); it != mpCurrentMenu->End(); it++) {
    if((it->Flags & CUIMenu::IF_HIDDEN) > 0)
      continue;

    if((it->Flags & CUIMenu::IF_FOCUS) > 0) {
      it->HLValue = glm::clamp(it->HLValue + timeDelta, 0.0f, 1.0f);
    }
    else if(it->Flags > 0.0f) {
      it->HLValue = glm::clamp(it->HLValue - timeDelta, 0.0f, 1.0f);
    }
  }
}

CMenuView::CMenuView(const CMenuProcess& process, const glm::vec2& screenSize)
  : mProcess(process)
  , mText(screenSize)
  , mMenuView(screenSize)
  , mScreenSize(screenSize)
{}

CMenuView::~CMenuView() {}

const bool CMenuView::Init() {
  if(!mText.Init(L"font.fgx")) {
    return false;
  }

  return true;
}

void CMenuView::Free() {}

void CMenuView::RenderView() const {
}

void CMenuView::RenderUI() const {
  if(mProcess.GetCurrent() == nullptr)
    return;

  CUIMenuLayout layout;
  layout.ItemPadding = glm::vec2(1.0f, 0.0f);
  layout.Margin = glm::vec2(1.0f);
  layout.TitleScale = glm::vec2(2.0f);

  mMenuView.Render(mProcess.GetFont(), mText, layout, *mProcess.GetCurrent());

  glm::mat4 proj = glm::ortho(0.0f, mScreenSize.x, mScreenSize.y, 0.0f);

  glLoadMatrixf(glm::value_ptr(proj));
  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
  RenderQuad(mProcess.GetPointerPos() * mScreenSize, glm::vec2(20.0f));
}
