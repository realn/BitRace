#include "stdafx.h"
#include "GUIMenu.h"
#include "GUIScreen.h"
#include "GUIController.h"
#include "GUI.h"
#include "Input.h"

CGUIMenuItem::CGUIMenuItem(CGUIMenu* pMenu, const Uint32 id, const std::string& text, const Uint32 userDefID) :
  m_pMenu(pMenu),
  m_Id(id),
  m_UserDefId(userDefID),
  m_Flags(MIF_ENABLED | MIF_VISIBLE)
{
  glm::vec2 screenSize = m_pMenu->GetScreen()->GetSize();
  glm::vec4 rectCol = m_pMenu->GetStyleColor(CGUIMenu::SC_ITEMRECT);
  glm::vec4 rectColHS = m_pMenu->GetStyleColor(CGUIMenu::SC_ITEMRECTHIGHLIGHT);
  glm::vec4 textCol = m_pMenu->GetStyleColor(CGUIMenu::SC_ITEMTEXT);
  glm::vec4 textColHS = m_pMenu->GetStyleColor(CGUIMenu::SC_ITEMTEXTHIGHLIGHT);

  m_pTextControl = new CGUITextControl(text, textCol);
  m_pRectControl = new CGUIRectControl(glm::vec2(0.0f), rectCol);

  m_pMenu->GetScreen()->AddControl(m_pRectControl);
  m_pMenu->GetScreen()->AddControl(m_pTextControl);

  CGUIControllerList* pList = m_pMenu->GetControllerList();

  m_pRectFadeAnim = new CGUIColorFadeAnimation(pList, m_pRectControl, 0.5f, rectCol, rectColHS, false);
  m_pRectFadeAnim->ForceRun();
  m_pTextFadeAnim = new CGUIColorFadeAnimation(pList, m_pTextControl, 0.5f, textCol, textColHS, false);
  m_pTextFadeAnim->ForceRun();
}

CGUIMenuItem::~CGUIMenuItem() {
  this->m_pMenu->GetScreen()->RemoveControl(m_pTextControl, true);
  this->m_pMenu->GetScreen()->RemoveControl(m_pRectControl, true);
}

const bool CGUIMenuItem::Update(CInput* pInput, const float fDT) {
  bool enabled = IsEnabled();
  m_pTextControl->SetVisible(enabled);
  m_pRectControl->SetVisible(enabled);
  if(!enabled) {
    return false;
  }

  glm::vec2 screenSize = m_pMenu->GetScreen()->GetSize();

  m_pRectFadeAnim->SetVisible(HasFocus());
  m_pTextFadeAnim->SetVisible(HasFocus());

  m_pRectFadeAnim->Run();
  m_pTextFadeAnim->Run();
  
  this->m_pRectControl->SetSize(glm::vec2(screenSize.x, m_pTextControl->GetSize().y));

  glm::vec2 vMousePos = glm::vec2(pInput->GetMousePos());
  CGUIScreenItem* pItem = m_pMenu->GetScreen()->GetItem(m_pTextControl);
  if(pItem->Contains(vMousePos)) {
    this->SetFocus(true);
    if(pInput->IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
      return true;
    }
  }
  else {
    this->SetFocus(false);
  }

  return false;
}

void CGUIMenuItem::SetEnable(bool enable) {
  if (enable)
    m_Flags |= MIF_ENABLED;
  else
    m_Flags &= ~MIF_ENABLED;
}

void CGUIMenuItem::SetVisible(const bool visible) {
  if (visible)
    m_Flags |= MIF_VISIBLE;
  else
    m_Flags &= ~MIF_VISIBLE;
}

void CGUIMenuItem::SetFocus(bool focus) {
  if (focus)
    m_Flags |= MIF_FOCUS;
  else
    m_Flags &= ~MIF_FOCUS;
}

void CGUIMenuItem::SetPos(const glm::vec2 & pos) {
  m_pMenu->GetScreen()->GetItem(m_pTextControl)->SetPos(pos);
  m_pMenu->GetScreen()->GetItem(m_pRectControl)->SetPos(glm::vec2(0.0f, pos.y));
}

void CGUIMenuItem::SetName(const std::string& name) {
  this->m_pTextControl->SetText(name);
}

void CGUIMenuItem::SetUserDefID(Uint32 uSet) {
  m_UserDefId = uSet;
}

const bool CGUIMenuItem::HasFocus() const {
  return (m_Flags & MIF_FOCUS) > 0;
}

const bool CGUIMenuItem::IsVisible() const {
  return (m_Flags & MIF_VISIBLE) > 0;
}

const bool CGUIMenuItem::IsEnabled() const {
  return (m_Flags & MIF_ENABLED) > 0;
}

const Uint32 CGUIMenuItem::GetID() const {
  return m_Id;
}

const std::string CGUIMenuItem::GetName() const {
  return m_pTextControl->GetText();
}

const Uint32 CGUIMenuItem::GetUserDefID() const {
  return m_UserDefId;
}


//==============================================================================================


CGUIMenu::CGUIMenu(CGUIMenuManager* pMenuMng, const Uint32 id, const std::string& title, const glm::vec2& size) :
  m_pMenuMng(pMenuMng),
  m_pScreen(nullptr),
  m_pTitleControl(nullptr),
  m_Id(id),
  m_Flags(MF_VISIBLE),
  m_Index(0),
  m_ClickedID(0),
  m_ItemsPos(40.0f, 70.0f),
  m_ItemsPadding(0.0f, 30.0f)
{
  m_StyleColors[SC_ITEMRECT] = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
  m_StyleColors[SC_ITEMTEXT] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
  m_StyleColors[SC_ITEMRECTHIGHLIGHT] = glm::vec4(1.0f, 1.0f, 1.0f, 0.9f);
  m_StyleColors[SC_ITEMTEXTHIGHLIGHT] = glm::vec4(0.0f, 0.1f, 0.0f, 1.0f);

  m_pControllerList = new CGUIControllerList();
  m_pScreen = new CGUIScreen(m_pMenuMng->GetGUI(), size);
  m_pTitleControl = new CGUITextControl(title);
  m_pScreen->AddControl(m_pTitleControl, glm::vec2(10.0f, 10.0f));
}

CGUIMenu::~CGUIMenu() {
  Clear(true);
  delete m_pScreen;
  delete m_pControllerList;
}

const bool CGUIMenu::Update(CInput* pInput, float timeDelta) {
  this->m_pControllerList->Update(timeDelta);
  for(std::vector<CGUIMenuItem*>::iterator it = m_Items.begin(); it != m_Items.end(); it++) {
    if((*it)->Update(pInput, timeDelta)) {
      m_ClickedID = (*it)->GetID();
      return true;
    }
  }
  return false;
}

void CGUIMenu::Render() {
  if(!this->IsVisible())
    return;

  this->m_pScreen->Render();
}

CGUIMenuItem* CGUIMenu::AddMenuItem(const Uint32 id, const std::string& name, const Uint32 userDefID) {
  if(GetMenuItem(id) != nullptr)
    return nullptr;

  CGUIMenuItem* pItem = new CGUIMenuItem(this, id, name, userDefID);
  m_Items.push_back(pItem);

  this->UpdateItemsPos();

  return pItem;
}

CGUIMenuItem* CGUIMenu::RemoveMenuItem(const Uint32 id, const bool deleteItem) {
  std::vector<CGUIMenuItem*>::iterator it;
  for(it = m_Items.begin(); it != m_Items.end(); it++) {
    if((*it)->GetID() == id)
      break;
  }
  if(it == m_Items.end())
    return nullptr;

  CGUIMenuItem* pItem = (*it);
  m_Items.erase(it);

  if(deleteItem) {
    delete pItem;
    return nullptr;
  }

  return pItem;
}

CGUIMenuItem* CGUIMenu::GetMenuItem(const Uint32 id) const {
  for(std::vector<CGUIMenuItem*>::const_iterator it = m_Items.begin(); it != m_Items.end(); it++) {
    if((*it)->GetID() == id)
      return *it;
  }
  return nullptr;
}

void CGUIMenu::Clear(const bool deleteItems) {
  if(deleteItems) {
    for(std::vector<CGUIMenuItem*>::iterator it = m_Items.begin(); it != m_Items.end(); it++) {
      delete *it;
    }
  }
  m_Items.clear();
}

void CGUIMenu::SetTitle(const std::string & title) {
  this->m_pTitleControl->SetText(title);
}

void CGUIMenu::SetVisible(const bool visible) {
  if (visible)
    m_Flags |= MF_VISIBLE;
  else
    m_Flags &= ~MF_VISIBLE;
}

const bool CGUIMenu::IsVisible() const {
  return (m_Flags & MF_VISIBLE) > 0;
}


const Uint32 CGUIMenu::GetClickedID() const {
  return m_ClickedID;
}

const std::string CGUIMenu::GetTitle() const {
  return m_pTitleControl->GetText();
}

const Uint32 CGUIMenu::GetID() const {
  return m_Id;
}

const glm::vec4 CGUIMenu::GetStyleColor(const STYLECOLORS color) const {
  stylecolors::const_iterator it = m_StyleColors.find(color);
  if(it == m_StyleColors.end())
    return glm::vec4();
  return it->second;
}

void CGUIMenu::UpdateItemsPos() {
  glm::vec2 pos = m_ItemsPos;
  for (std::vector<CGUIMenuItem*>::iterator it = m_Items.begin(); it != m_Items.end(); it++) {
    if (!(*it)->IsVisible())
      continue;

    (*it)->SetPos(pos);

    pos += m_ItemsPadding;
  }
}

CGUIScreen * CGUIMenu::GetScreen() const {
  return m_pScreen;
}

CGUIControllerList * CGUIMenu::GetControllerList() const {
  return m_pControllerList;
}


//==============================================================================================


CGUIMenuManager::CGUIMenuManager(CGUI* pGUI, const glm::vec2& size) :
  m_pGUI(pGUI),
  m_Size(size),
  m_MousePos(0.0f),
  m_pCurrentMenu(nullptr),
  m_pTargetMenu(nullptr) {}

CGUIMenuManager::~CGUIMenuManager() {
  Clear();
}

void CGUIMenuManager::SetSize(const glm::vec2 & size) {
  this->m_Size = size;
}

const glm::vec2 & CGUIMenuManager::GetSize() const {
  return this->m_Size;
}

bool CGUIMenuManager::Update(CInput* pInput, float timeDelta) {
  this->m_MousePos = pInput->GetMousePos();
  this->m_MousePos = glm::clamp(m_MousePos, glm::vec2(0.0f), m_Size);

  if(this->m_MenuList.size() == 0)
    return false;

  if (m_pCurrentMenu == nullptr)
    m_pCurrentMenu = *m_MenuList.begin();

  if (m_pTargetMenu == nullptr)
    m_pTargetMenu = m_pCurrentMenu;

  if (m_pTargetMenu != m_pCurrentMenu) {
    m_pCurrentMenu->SetVisible(false);
    m_pCurrentMenu = m_pTargetMenu;
  }
  else if(!m_pCurrentMenu->IsVisible()) {
    m_pCurrentMenu->SetVisible(true);
  }

  if (m_pCurrentMenu->Update(pInput, timeDelta))
    return true;
  return false;
}

void CGUIMenuManager::Render() {
  if (m_pCurrentMenu == nullptr)
    return;

  m_pCurrentMenu->Render();

  m_pGUI->Begin(this->m_Size);
  glDisable(GL_TEXTURE_2D);
  glColor3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_TRIANGLES);
  glVertex2f(m_MousePos.x, m_MousePos.y);
  glVertex2f(m_MousePos.x + 20.0f, m_MousePos.y + 10.0f);
  glVertex2f(m_MousePos.x + 10.0f, m_MousePos.y + 20.0f);
  glEnd();
  glEnable(GL_TEXTURE_2D);
  m_pGUI->End();
}

CGUI * CGUIMenuManager::GetGUI() const {
  return m_pGUI;
}

CGUIMenu* CGUIMenuManager::AddMenu(const Uint32 id, const std::string& name) {
  if (GetMenu(id) != nullptr)
    return nullptr;

  CGUIMenu* pMenu = new CGUIMenu(this, id, name, m_Size);
  m_MenuList.push_back(pMenu);

  return pMenu;
}

CGUIMenu* CGUIMenuManager::GetMenu(const Uint32 id) const {
  for (std::vector<CGUIMenu*>::const_iterator it = m_MenuList.begin(); it != m_MenuList.end(); it++) {
    if ((*it)->GetID() == id)
      return *it;
  }
  return nullptr;
}

CGUIMenu* CGUIMenuManager::RemoveMenu(const Uint32 id, const bool deleteMenu) {
  CGUIMenu* pMenu = nullptr;
  for (std::vector<CGUIMenu*>::iterator it = m_MenuList.begin(); it != m_MenuList.end(); it++) {
    if ((*it)->GetID() == id) {
      pMenu = *it;
      m_MenuList.erase(it);
      break;
    }
  }

  if (deleteMenu) {
    delete pMenu;
    return nullptr;
  }
  return pMenu;
}

void CGUIMenuManager::Clear(const bool deleteMenus) {
  if (deleteMenus) {
    for (std::vector<CGUIMenu*>::iterator it = m_MenuList.begin(); it != m_MenuList.end(); it++) {
      delete *it;
    }
  }
  m_MenuList.clear();
}

CGUIMenu* CGUIMenuManager::GetCurrentMenu() const {
  return m_pCurrentMenu;
}

bool CGUIMenuManager::SwitchToMenu(const Uint32 id) {
  m_pTargetMenu = GetMenu(id);
  return m_pTargetMenu != nullptr;
}

bool CGUIMenuManager::ForceSwitchToMenu(const Uint32 id) {
  m_pTargetMenu = GetMenu(id);
  m_pCurrentMenu = GetMenu(id);

  return m_pCurrentMenu != nullptr;
}

