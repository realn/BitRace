#include "GUIMenu.h"
#include "GUIScreen.h"
#include "Game.h"

CGUIMenuItem::CGUIMenuItem(CGUIMenu* pMenu, const Uint32 id, const std::string& text, const glm::vec2& pos, const Uint32 userDefID) :
  m_pMenu(pMenu),
  m_ShowPos(pos),
  m_HidePos(pos.x, -20.0f),
  m_Id(id),
  m_UserDefId(userDefID),
  m_Flags(MIF_ENABLED | MIF_HIDDEN),
  m_FocusLight(0.0f),
  m_AnimSpeed(3.0f),
  m_FocusUpSpeed(5.0f),
  m_FocusDownSpeed(2.0f) 
{
  glm::vec2 screenSize = pMenu->GetScreen()->GetSize();

  m_pTextItem = new CGUIScreenTextItem(pMenu->GetScreen(), m_HidePos, text);
  m_pTextItem->SetColor(glm::vec4(1.0f));

  m_pRectItem = new CGUIScreenRectItem(pMenu->GetScreen(),
                                       glm::vec2(0.0f, m_ShowPos.y),
                                       glm::vec2(screenSize.x, m_pTextItem->GetSize().y));
  m_pRectItem->SetColor(glm::vec4(glm::vec3(1.0f), 0.0f));

  m_pMenu->GetScreen()->AddItem(m_pRectItem);
  m_pMenu->GetScreen()->AddItem(m_pTextItem);
}

CGUIMenuItem::~CGUIMenuItem() {
  this->m_pMenu->GetScreen()->RemoveItem(m_pTextItem, true);
  this->m_pMenu->GetScreen()->RemoveItem(m_pRectItem, true);
}

const bool CGUIMenuItem::Update(CGame* pGame, const float fDT) {
  if(!IsEnabled()) {
    this->m_pRectItem->SetVisible(false);
    this->m_pTextItem->SetVisible(false);
    return false;
  }

  if(IsShowing()) {
    glm::vec2 pos = this->m_pTextItem->GetPos();
    pos += (m_ShowPos - m_HidePos) * m_AnimSpeed * fDT;
    if(m_ShowPos.y - pos.y < 0.1f) {
      this->m_Flags &= ~MIF_SHOWANIM;
      this->m_pTextItem->SetPos(m_ShowPos);
    }
    else {
      this->m_pTextItem->SetPos(pos);
    }
    return false;
  }
  if(IsHiding()) {
    glm::vec2 pos = this->m_pTextItem->GetPos();
    pos += (m_HidePos - m_ShowPos) * m_AnimSpeed * fDT;
    if(pos.y - m_HidePos.y < 0.1f) {
      this->m_Flags &= ~MIF_HIDEANIM;
      this->m_Flags |= MIF_HIDDEN;
      this->m_pTextItem->SetPos(m_HidePos);
      this->m_pTextItem->SetVisible(false);
    }
    else {
      this->m_pTextItem->SetPos(pos);
    }
    return false;
  }


  if(HasFocus() && m_FocusLight < 1.0f) {
    m_FocusLight = glm::clamp(m_FocusLight + m_FocusUpSpeed * fDT, 0.0f, 1.0f);
  }
  else if(!HasFocus() && m_FocusLight > 0.0f) {
    m_FocusLight = glm::clamp(m_FocusLight - m_FocusDownSpeed * fDT, 0.0f, 1.0f);
  }

  this->m_pRectItem->SetColor(glm::vec4(glm::vec3(1.0f), this->m_FocusLight));
  this->m_pTextItem->SetColor(glm::vec4(glm::vec3(1.0f - this->m_FocusLight), 1.0f));

  glm::vec2 vMousePos = glm::vec2(pGame->GetMousePos());
  if(this->m_pTextItem->Contains(vMousePos)) {
    this->SetFocus(true);
    if(pGame->IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
      return true;
    }
  }
  else
    this->SetFocus(false);

  return false;
}

const bool CGUIMenuItem::HasFocus() const {
  return (m_Flags & MIF_FOCUS) > 0;
}

const bool CGUIMenuItem::IsVisible() const {
  return (m_Flags & MIF_HIDDEN) == 0;
}

const bool CGUIMenuItem::IsEnabled() const {
  return (m_Flags & MIF_ENABLED) > 0;
}

const bool CGUIMenuItem::IsAnimating() const {
  return (IsShowing() || IsHiding());
}

const bool CGUIMenuItem::IsHiding() const {
  return (m_Flags & MIF_HIDEANIM) > 0;
}

const bool CGUIMenuItem::IsHidden() const {
  return (m_Flags & MIF_HIDDEN) > 0;
}

const bool CGUIMenuItem::IsShowing() const {
  return (m_Flags & MIF_SHOWANIM) > 0;
}

const Uint32 CGUIMenuItem::GetID() const {
  return m_Id;
}

const std::string CGUIMenuItem::GetName() const {
  return m_pTextItem->GetText();
}

const glm::vec2 CGUIMenuItem::GetPos() const {
  return m_pTextItem->GetPos();
}

const Uint32 CGUIMenuItem::GetUserDefID() const {
  return m_UserDefId;
}

void CGUIMenuItem::SetEnable(bool enable) {
  if(enable)
    m_Flags |= MIF_ENABLED;
  else
    m_Flags &= ~MIF_ENABLED;
  this->m_pTextItem->SetVisible(enable);
  this->m_pRectItem->SetVisible(enable);
}

void CGUIMenuItem::SetFocus(bool bSet) {
  if(bSet)
    m_Flags |= MIF_FOCUS;
  else
    m_Flags &= ~MIF_FOCUS;
}

void CGUIMenuItem::SetName(const std::string& name) {
  this->m_pTextItem->SetText(name);
}

void CGUIMenuItem::SetUserDefID(Uint32 uSet) {
  m_UserDefId = uSet;
}

void CGUIMenuItem::Show() {
  m_Flags |= MIF_SHOWANIM;
  m_Flags &= ~MIF_HIDDEN;
  this->m_pTextItem->SetVisible(true);

}

void CGUIMenuItem::Hide() {
  m_Flags |= MIF_HIDEANIM;
  this->m_pTextItem->SetVisible(true);
}

void CGUIMenuItem::ForceShow() {
  m_Flags &= ~MIF_HIDDEN;
  m_pTextItem->SetPos(m_ShowPos);
}

void CGUIMenuItem::ForceHide() {
  m_Flags |= MIF_HIDDEN;
  m_pTextItem->SetPos(m_HidePos);
}

//========================================================

CGUIMenu::CGUIMenu(CGUIMenuManager* pMenuMng, const Uint32 id, const std::string& title, const glm::vec2& size) :
  m_pMenuMng(pMenuMng),
  m_Id(id),
  m_Flags(MF_HIDDEN),
  m_Time(0.0f),
  m_Index(0),
  m_ClickedID(0) {
  m_pScreen = new CGUIScreen(m_pMenuMng->GetGUI(), size);
  m_pTitleItem = new CGUIScreenTextItem(m_pScreen, glm::vec2(9.0f, 9.0f), title);
  m_pScreen->AddItem(m_pTitleItem);
}

CGUIMenu::~CGUIMenu() {
  Clear(true);
  delete m_pScreen;
}

const bool CGUIMenu::Update(CGame* pGame, float timeDelta) {
  if(IsShowing()) {
    m_Time += timeDelta;
    if(m_Time > 0.2f) {
      m_Time = 0.0f;

      if(m_Index == 0 && AllItemsVisible()) {
        m_Flags &= ~MF_SHOWANIM;
      }
      else if(m_Index > 0) {
        m_Index--;
        m_Items[m_Index]->Show();
      }
    }
  }
  if(IsHiding()) {
    m_Time += timeDelta;
    if(m_Time > 0.2f) {
      m_Time = 0.0f;

      if(m_Index >= m_Items.size() && AllItemsHidden()) {
        m_Flags &= ~MF_HIDEANIM;
        m_Flags |= MF_HIDDEN;
      }
      else if(m_Index < m_Items.size()) {
        m_Items[m_Index]->Hide();
        m_Index++;
      }
    }
  }

  for(std::vector<CGUIMenuItem*>::iterator it = m_Items.begin(); it != m_Items.end(); it++) {
    if((*it)->Update(pGame, timeDelta)) {
      m_ClickedID = (*it)->GetID();
      return true;
    }
  }
  return false;
}

void CGUIMenu::Render() {
  if(this->IsHidden())
    return;

  this->m_pScreen->Render();
}

CGUIMenuItem* CGUIMenu::AddMenuItem(const Uint32 id, const std::string& name, const glm::vec2& pos, const Uint32 userDefID) {
  if(GetMenuItem(id) != nullptr)
    return nullptr;

  CGUIMenuItem* pItem = new CGUIMenuItem(this, id, name, pos, userDefID);
  m_Items.push_back(pItem);
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
  this->m_pTitleItem->SetText(title);
}

void CGUIMenu::Hide() {
  m_Flags &= ~MF_SHOWANIM;
  m_Flags |= MF_HIDEANIM;
  m_Index = 0;
}

void CGUIMenu::Show() {
  m_Flags &= ~MF_HIDDEN;
  m_Flags &= ~MF_HIDEANIM;
  m_Flags |= MF_SHOWANIM;
  m_Index = m_Items.size();
}

const bool CGUIMenu::IsAnimating() const {
  return (IsShowing() || IsHiding());
}

const bool CGUIMenu::IsHiding() const {
  return (m_Flags & MF_HIDEANIM) > 0;
}

const bool CGUIMenu::IsHidden() const {
  return (m_Flags & MF_HIDDEN) > 0;
}

const bool CGUIMenu::IsVisible() const {
  return (m_Flags & MF_HIDDEN) == 0;
}

const bool CGUIMenu::IsShowing() const {
  return (m_Flags & MF_SHOWANIM) > 0;
}

const Uint32 CGUIMenu::GetClickedID() const {
  return m_ClickedID;
}

const std::string CGUIMenu::GetTitle() const {
  return m_pTitleItem->GetText();
}

const Uint32 CGUIMenu::GetID() const {
  return m_Id;
}

const bool CGUIMenu::AllItemsVisible() const {
  size_t itemsVisible = 0;
  for(std::vector<CGUIMenuItem*>::const_iterator it = m_Items.begin(); it != m_Items.end(); it++) {
    if(((*it)->IsVisible() && !(*it)->IsShowing()) || !(*it)->IsEnabled())
      itemsVisible++;
  }
  return m_Items.size() == itemsVisible;
}

const bool CGUIMenu::AllItemsHidden() const {
  size_t itemsHidden = 0;
  for(std::vector<CGUIMenuItem*>::const_iterator it = m_Items.begin(); it != m_Items.end(); it++) {
    if(((*it)->IsHidden() && !(*it)->IsHiding()) || !(*it)->IsEnabled())
      itemsHidden++;
  }
  return itemsHidden == m_Items.size();
}

void CGUIMenu::ForceShow() {
  m_Flags &= ~MF_HIDDEN;
  for(std::vector<CGUIMenuItem*>::iterator it = m_Items.begin(); it != m_Items.end(); it++) {
    (*it)->ForceShow();
  }
}

void CGUIMenu::ForceHide() {
  m_Flags |= MF_HIDDEN;
  for(std::vector<CGUIMenuItem*>::iterator it = m_Items.begin(); it != m_Items.end(); it++) {
    (*it)->ForceHide();
  }
}

CGUIScreen * CGUIMenu::GetScreen() const {
  return m_pScreen;
}

//===========================================================

CGUIMenuManager::CGUIMenuManager(CGUI* pGUI, const glm::vec2& size) :
  m_pGUI(pGUI),
  m_Size(size),
  m_MousePos(0.0f),
  m_uCurrMenu(0),
  m_uGoToMenu(0) {}

CGUIMenuManager::~CGUIMenuManager() {
  Clear();
}

void CGUIMenuManager::SetSize(const glm::vec2 & size) {
  this->m_Size = size;
}

const glm::vec2 & CGUIMenuManager::GetSize() const {
  return this->m_Size;
}

bool CGUIMenuManager::Update(CGame* pGame, float timeDelta) {
  this->m_MousePos = pGame->GetMousePos();

  if(m_MousePos.x < 0.0f)
    m_MousePos.x = 0.0f;
  if(m_MousePos.x > m_Size.x)
    m_MousePos.x = m_Size.x;
  if(m_MousePos.y < 0.0f)
    m_MousePos.y = 0.0f;
  if(m_MousePos.y > m_Size.y)
    m_MousePos.y = m_Size.y;

  if(this->m_aMenu.size() == 0)
    return false;

  if(this->m_uCurrMenu >= this->m_aMenu.size())
    this->m_uCurrMenu = 0;
  CGUIMenu* Menu = NULL;
  if(this->m_uCurrMenu != this->m_uGoToMenu) {
    Menu = this->GetCurrentMenu();
    if(Menu != NULL) {
      if(!Menu->IsHiding())
        Menu->Hide();
      if(Menu->IsHidden()) {
        Menu->ForceHide();
        this->m_uCurrMenu = this->m_uGoToMenu;
      }
    }
  }
  else {
    Menu = GetCurrentMenu();
    if(Menu != NULL && Menu->IsHidden() && !Menu->IsShowing())
      Menu->Show();
  }

  Menu = this->GetCurrentMenu();
  if(Menu != NULL) {
    if(Menu->Update(pGame, timeDelta)) {
      return true;
    }
  }
  return false;
}

void CGUIMenuManager::Render() {
  CGUIMenu* Menu = this->GetCurrentMenu();
  if(Menu == NULL)
    return;

  Menu->Render();

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

CGUIMenu* CGUIMenuManager::AddMenu(Uint32 uID, std::string strName) {
  size_t i;
  for(i = 0; i < m_aMenu.size(); i++)
    if(m_aMenu[i]->GetID() == uID)
      return NULL;

  CGUIMenu* m = new CGUIMenu(this, uID, strName, m_Size);
  m_aMenu.push_back(m);
  return m;
}

void CGUIMenuManager::DelMenu(Uint32 uID) {
  size_t i;
  for(i = 0; i < m_aMenu.size(); i++)
    if(m_aMenu[i]->GetID() == uID) {
      delete m_aMenu[i];
      m_aMenu.erase(m_aMenu.begin() + i);
      return;
    }
}

CGUIMenu* CGUIMenuManager::GetMenu(Uint32 uID) {
  size_t i;
  for(i = 0; i < m_aMenu.size(); i++)
    if(m_aMenu[i]->GetID() == uID)
      return m_aMenu[i];
  return NULL;
}

void CGUIMenuManager::Clear() {
  size_t i;
  for(i = 0; i < m_aMenu.size(); i++)
    delete m_aMenu[i];
  m_aMenu.clear();
}

CGUIMenu* CGUIMenuManager::GetCurrentMenu() {
  if(this->m_uCurrMenu >= Uint32(m_aMenu.size()))
    return NULL;
  return m_aMenu[m_uCurrMenu];
}

bool CGUIMenuManager::SwitchToMenu(Uint32 uID) {
  size_t i;
  for(i = 0; i < m_aMenu.size(); i++) {
    if(m_aMenu[i]->GetID() == uID) {
      this->m_uGoToMenu = Uint32(i);
      return true;
    }
  }
  return false;
}

bool CGUIMenuManager::ForceSwitchToMenu(Uint32 uID) {
  size_t i;
  for(i = 0; i < m_aMenu.size(); i++) {
    if(m_aMenu[i]->GetID() == uID) {
      this->m_uCurrMenu = Uint32(i);
      this->m_uGoToMenu = Uint32(i);
      return true;
    }
  }
  return false;
}

