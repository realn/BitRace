#include "GUI.h"
#include "Game.h"
#include "FGXFile.h"
#include "GLDefines.h"
#include "MathHelper.h"

#include <stdarg.h>
#include <glm/gtc/type_ptr.hpp>

CGUI::CGUI() {
  this->m_Vertex[0] = glm::vec2(0.0f, 16.0f);
  this->m_Vertex[1] = glm::vec2(16.0f, 16.0f);
  this->m_Vertex[2] = glm::vec2(16.0f, 0.0f);
  this->m_Vertex[3] = glm::vec2(0.0f, 0.0f);
}

CGUI::~CGUI() {
  Free();
}

bool CGUI::LoadFontTexture(const cb::string& filename) {
  CFGXFile imgFile;
  if (!imgFile.Load(filename)) {
    return false;
  }

  if (!imgFile.IsValid()) {
    return false;
  }

  glGenTextures(1, &this->m_FontTexture);
  glBindTexture(GL_TEXTURE_2D, this->m_FontTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glm::ivec2 size = imgFile.GetSize();
  Uint32 format = 0;
  switch (imgFile.GetImgDepth()) {
  case 1: format = GL_LUMINANCE8;
  case 2: format = GL_LUMINANCE8_ALPHA8;
  case 3: format = GL_RGB;
  case 4: format = GL_RGBA;
  };
  const cb::bytevector& Data = imgFile.GetData();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, cb::vectorptr(Data));
  //glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
  return true;
}

bool CGUI::InitFont() {
  this->m_uFontList = glGenLists(256);

  Uint32 i;
  float cx;
  float cy;
  for (i = 0; i < 256; ++i) {
    cx = float(i % 16) / 16.0f;
    cy = float(i / 16) / 16.0f;

    CFontChar fontChar;
    
    fontChar.m_Code = (Uint8)i;

    fontChar.m_TCoord[0] = glm::vec2(cx, 1 - cy - 0.0625f);
    fontChar.m_TCoord[1] = glm::vec2(cx + 0.0625f, 1 - cy - 0.0625f);
    fontChar.m_TCoord[2] = glm::vec2(cx + 0.0625f, 1 - cy);
    fontChar.m_TCoord[3] = glm::vec2(cx, 1 - cy);

    fontChar.m_Size = glm::vec2(16.0f, 16.0f);

    fontChar.m_Adv = 10.0f;

    this->m_CharList[(Uint8)i] = fontChar;
  }

  return true;
}

bool CGUI::Init() {
  if (!LoadFontTexture(L"font.fgx"))
    return false;
  if (!InitFont())
    return false;

  return true;
}

void CGUI::Free() {
  if (glIsList(this->m_uFontList))
    glDeleteLists(this->m_uFontList, 256);
  if (glIsTexture(this->m_FontTexture))
    glDeleteTextures(1, &this->m_FontTexture);
}

void CGUI::Begin(const glm::vec2& size) {
  this->m_Size = size;

  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0.0f, size.x, size.y, 0.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushAttrib(GL_ENABLE_BIT);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindTexture(GL_TEXTURE_2D, this->m_FontTexture);
}

void CGUI::End() {
  glPopAttrib();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void CGUI::Print(const glm::vec2& pos, std::string format, ...) const {
  if (format.empty())
    return;

  std::string text;
  va_list ap;

  va_start(ap, format);
  text = FormatText(format, ap);
  va_end(ap);

  std::vector<glm::vec2> vertList;
  std::vector<glm::vec2> texList;

  vertList.resize(4 * text.length());
  texList.resize(4 * text.length());

  glm::vec2 textPos = pos;
  for (size_t i = 0; i < text.length(); i++) {
    const CFontChar& fontChar = this->GetChar(text[i]);

    for (size_t j = 0; j < 4; j++) {
      vertList[i * 4 + j] = this->m_Vertex[j] + textPos;
      texList[i * 4 + j] = fontChar.m_TCoord[j];
    }

    textPos += glm::vec2(fontChar.m_Adv, 0.0f);
  }

  glVertexPointer(2, GL_FLOAT, 0, &vertList[0]);
  glTexCoordPointer(2, GL_FLOAT, 0, &texList[0]);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  //glm::vec4 color(1.0f);
  //glColor4fv(glm::value_ptr(color));
  glDrawArrays(GL_QUADS, 0, vertList.size());

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

const glm::vec2 CGUI::GetPrintSize(const std::string format, ...) const {
  std::string text;

  va_list va;
  va_start(va, format);
  text = FormatText(format, va);
  va_end(va);

  glm::vec2 result;
  for(unsigned i = 0; i < text.length(); i++) {
    const CFontChar& fontChar = this->GetChar(text[i]);

    result.y = glm::max(fontChar.m_Size.y, result.y);
    result.x += fontChar.m_Size.x;
  }
  return result;
}

void CGUI::RenderQuad(const glm::vec2 & pos, const glm::vec2 & size, const glm::vec4 & color, const Uint32 texId) {
  glm::vec2 vert[4];
  vert[0] = pos;
  vert[1] = pos + glm::vec2(size.x, 0.0f);
  vert[2] = pos + size;
  vert[3] = pos + glm::vec2(0.0f, size.y);

  glm::vec2 texc[4];
  texc[0] = glm::vec2(0.0f, 0.0f);
  texc[1] = glm::vec2(1.0f, 0.0f);
  texc[2] = glm::vec2(1.0f, 1.0f);
  texc[3] = glm::vec2(0.0f, 1.0f);

  Uint16 ind[6];
  ind[0] = 0;
  ind[1] = 1;
  ind[2] = 2;
  ind[3] = 0;
  ind[4] = 2;
  ind[5] = 3;

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vert);
  glTexCoordPointer(2, GL_FLOAT, 0, texc);

  if(texId) {
    glBindTexture(GL_TEXTURE_2D, texId);
  }
  else
    glDisable(GL_TEXTURE_2D);

  glColor4fv(glm::value_ptr(color));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, ind);

  if(texId) {
    glBindTexture(GL_TEXTURE_2D, m_FontTexture);
  }
  else
    glEnable(GL_TEXTURE_2D);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void CGUI::RenderQuadLines(const glm::vec2 & pos, const glm::vec2 & size, const glm::vec4 & color) {
  glm::vec2 vert[4];
  vert[0] = pos;
  vert[1] = pos + glm::vec2(size.x, 0.0f);
  vert[2] = pos + size;
  vert[3] = pos + glm::vec2(0.0f, size.y);

  Uint16 ind[8];
  ind[0] = 0;
  ind[1] = 1;
  ind[2] = 1;
  ind[3] = 2;
  ind[4] = 2;
  ind[5] = 3;
  ind[6] = 3;
  ind[7] = 0;

  glDisable(GL_TEXTURE_2D);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vert);

  glColor4fv(glm::value_ptr(color));
  glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, ind);

  glDisableClientState(GL_VERTEX_ARRAY);
  glEnable(GL_TEXTURE_2D);
}

void CGUI::RenderQuadFullScreen(const glm::vec2& size, const glm::vec4& color, const Uint32 texId) {
  this->RenderQuad(glm::vec2(0.0f), size, color, texId);
}

void CGUI::RenderProgressBar(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const float progress) {
  RenderQuadLines(pos, size, color);

  float sizeX = size.x * (progress / 100.0f);

  RenderQuad(pos, glm::vec2(sizeX, size.y), color);
}

const CGUI::CFontChar & CGUI::GetChar(const char charCode) const {
  Uint8 code = (Uint8)charCode - 32;

  return this->m_CharList.at(code);
}

const std::string CGUI::FormatText(const std::string & format, va_list va) const {
  std::string text;
  int len = _vscprintf(format.c_str(), va);
  text.resize(len);
  vsprintf_s(&text[0], len + 1, format.c_str(), va);
  return text;
}

//==========================================================

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

  m_pTextItem = new CScreenItem(pMenu->GetScreen(), CScreenItem::IT_TEXT, m_HidePos, glm::vec2(0.0));
  m_pTextItem->SetText(text);
  m_pTextItem->SetColor(glm::vec4(1.0f));

  m_pRectItem = new CScreenItem(pMenu->GetScreen(), 
                                CScreenItem::IT_RECTFILL, 
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

  if (IsShowing()) {
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
  if (IsHiding()) {
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


  if (HasFocus() && m_FocusLight < 1.0f) {
    m_FocusLight = glm::clamp(m_FocusLight + m_FocusUpSpeed * fDT, 0.0f, 1.0f);
  }
  else if (!HasFocus() && m_FocusLight > 0.0f) {
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
  if (enable)
    m_Flags |= MIF_ENABLED;
  else 
    m_Flags &= ~MIF_ENABLED;
  this->m_pTextItem->SetVisible(enable);
  this->m_pRectItem->SetVisible(enable);
}

void CGUIMenuItem::SetFocus(bool bSet) {
  if (bSet)
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
  m_ClickedID(0) 
{
  m_pScreen = new CScreen(m_pMenuMng->GetGUI(), size);
  m_pTitleItem = new CScreenItem(m_pScreen, CScreenItem::IT_TEXT, glm::vec2(9.0f, 9.0f), glm::vec2(0.0));
  m_pTitleItem->SetText(title);
  m_pScreen->AddItem(m_pTitleItem);
}

CGUIMenu::~CGUIMenu() {
  Clear(true);
  delete m_pScreen;
}

const bool CGUIMenu::Update(CGame* pGame, float timeDelta) {
  if (IsShowing()) {
    m_Time += timeDelta;
    if (m_Time > 0.2f) {
      m_Time = 0.0f;

      if (m_Index == 0 && AllItemsVisible()) {
          m_Flags &= ~MF_SHOWANIM;
      }
      else if(m_Index > 0) {
        m_Index--;
        m_Items[m_Index]->Show();
      }
    }
  }
  if (IsHiding()) {
    m_Time += timeDelta;
    if (m_Time > 0.2f) {
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
  if (this->IsHidden())
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

CScreen * CGUIMenu::GetScreen() const {
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

  if (m_MousePos.x < 0.0f)
    m_MousePos.x = 0.0f;
  if (m_MousePos.x > m_Size.x)
    m_MousePos.x = m_Size.x;
  if (m_MousePos.y < 0.0f)
    m_MousePos.y = 0.0f;
  if (m_MousePos.y > m_Size.y)
    m_MousePos.y = m_Size.y;

  if (this->m_aMenu.size() == 0)
    return false;

  if (this->m_uCurrMenu >= this->m_aMenu.size())
    this->m_uCurrMenu = 0;
  CGUIMenu* Menu = NULL;
  if (this->m_uCurrMenu != this->m_uGoToMenu) {
    Menu = this->GetCurrentMenu();
    if (Menu != NULL) {
      if (!Menu->IsHiding())
        Menu->Hide();
      if (Menu->IsHidden()) {
        Menu->ForceHide();
        this->m_uCurrMenu = this->m_uGoToMenu;
      }
    }
  }
  else {
    Menu = GetCurrentMenu();
    if (Menu != NULL && Menu->IsHidden() && !Menu->IsShowing())
      Menu->Show();
  }

  Menu = this->GetCurrentMenu();
  if (Menu != NULL) {
    if (Menu->Update(pGame, timeDelta)) {
      return true;
    }
  }
  return false;
}

void CGUIMenuManager::Render() {
  CGUIMenu* Menu = this->GetCurrentMenu();
  if (Menu == NULL)
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
  for (i = 0; i < m_aMenu.size(); i++)
    if (m_aMenu[i]->GetID() == uID)
      return NULL;

  CGUIMenu* m = new CGUIMenu(this, uID, strName, m_Size);
  m_aMenu.push_back(m);
  return m;
}

void CGUIMenuManager::DelMenu(Uint32 uID) {
  size_t i;
  for (i = 0; i < m_aMenu.size(); i++)
    if (m_aMenu[i]->GetID() == uID) {
      delete m_aMenu[i];
      m_aMenu.erase(m_aMenu.begin() + i);
      return;
    }
}

CGUIMenu* CGUIMenuManager::GetMenu(Uint32 uID) {
  size_t i;
  for (i = 0; i < m_aMenu.size(); i++)
    if (m_aMenu[i]->GetID() == uID)
      return m_aMenu[i];
  return NULL;
}

void CGUIMenuManager::Clear() {
  size_t i;
  for (i = 0; i < m_aMenu.size(); i++)
    delete m_aMenu[i];
  m_aMenu.clear();
}

CGUIMenu* CGUIMenuManager::GetCurrentMenu() {
  if (this->m_uCurrMenu >= Uint32(m_aMenu.size()))
    return NULL;
  return m_aMenu[m_uCurrMenu];
}

bool CGUIMenuManager::SwitchToMenu(Uint32 uID) {
  size_t i;
  for (i = 0; i < m_aMenu.size(); i++) {
    if (m_aMenu[i]->GetID() == uID) {
      this->m_uGoToMenu = Uint32(i);
      return true;
    }
  }
  return false;
}

bool CGUIMenuManager::ForceSwitchToMenu(Uint32 uID) {
  size_t i;
  for (i = 0; i < m_aMenu.size(); i++) {
    if (m_aMenu[i]->GetID() == uID) {
      this->m_uCurrMenu = Uint32(i);
      this->m_uGoToMenu = Uint32(i);
      return true;
    }
  }
  return false;
}

CScreenItem::CScreenItem(CScreen * pScreen, const ITEMTYPE type, const glm::vec2 & pos, const glm::vec2 & size) :
  m_pScreen(pScreen), m_Type(type), m_Pos(pos), m_Size(size), m_Color(1.0f), m_Align(IA_DEFAULT), m_Visible(true) {}

CScreenItem::~CScreenItem() {}

void CScreenItem::Render() {
  if(!this->m_Visible)
    return;

  CGUI* pGUI = this->m_pScreen->GetGUI();
  glm::vec2 pos = this->GetRenderPos();

  switch(this->m_Type) {
  case IT_TEXT: pGUI->Print(pos, this->m_Text); break;
  case IT_RECTFILL: pGUI->RenderQuad(pos, this->m_Size, this->m_Color); break;
  case IT_RECTLINE: pGUI->RenderQuadLines(pos, this->m_Size, this->m_Color);  break;

  default:
    break;
  }
}

const bool CScreenItem::Contains(const glm::vec2 & point) const {
  glm::vec2 pos = this->GetRenderPos();
  return
    math::inrange(point.x, pos.x, pos.x + m_Size.x) &&
    math::inrange(point.y, pos.y, pos.y + m_Size.y);
}

void CScreenItem::SetPos(const glm::vec2 & pos) {
  this->m_Pos = pos;
}

void CScreenItem::SetSize(const glm::vec2 & size) {
  this->m_Size = size;
}

void CScreenItem::SetColor(const glm::vec4 & color) {
  this->m_Color = color;
}

void CScreenItem::SetAlign(const Uint32 flags) {
  this->m_Align = flags;
}

void CScreenItem::SetText(const std::string & text, bool changeSize) {
  this->m_Text = text;
  if(changeSize) {
    this->m_Size = this->m_pScreen->GetGUI()->GetPrintSize(this->m_Text);
  }
}

void CScreenItem::SetVisible(const bool visible) {
  this->m_Visible = visible;
}

const glm::vec2 & CScreenItem::GetPos() const {
  return this->m_Pos;
}

const glm::vec2 & CScreenItem::GetSize() const {
  return this->m_Size;
}

const glm::vec4 & CScreenItem::GetColor() const {
  return this->m_Color;
}

const Uint32 CScreenItem::GetAlign() const {
  return this->m_Align;
}

const std::string & CScreenItem::GetText() const {
  return this->m_Text;
}

const bool CScreenItem::IsVisible() const {
  return this->m_Visible;
}

const glm::vec2 CScreenItem::GetRenderPos() const {
  glm::vec2 result;
  glm::vec2 screenSize = this->m_pScreen->GetSize();
  
  if((this->m_Align & IA_LEFT) > 0 && (this->m_Align & IA_RIGHT) > 0) {
    result.x = ((screenSize.x - this->m_Size.x) / 2.0f) + this->m_Pos.x;
  }
  else if((this->m_Align & IA_RIGHT) > 0) {
    result.x = screenSize.x - this->m_Size.x - this->m_Pos.x;
  }
  else {
    result.x = this->m_Pos.x;
  }

  if((this->m_Align & IA_TOP) > 0 && (this->m_Align & IA_BOTTOM) > 0) {
    result.y = ((screenSize.y - this->m_Size.y) / 2.0f) + this->m_Pos.y;
  }
  else if((this->m_Align & IA_BOTTOM) > 0) {
    result.y = screenSize.y - this->m_Size.y - this->m_Pos.y;
  }
  else {
    result.y = this->m_Pos.y;
  }

  return result;
}

CScreen::CScreen(CGUI * pGUI, const glm::vec2 & size) :
  m_pGUI(pGUI), m_Size(size) {}

CScreen::~CScreen() {
  this->ClearItems(true);
}

void CScreen::AddItem(CScreenItem * pItem) {
  if(std::find(this->m_Items.begin(), this->m_Items.end(), pItem) != this->m_Items.end())
    return;

  this->m_Items.push_back(pItem);
}

void CScreen::RemoveItem(CScreenItem * pItem, bool deleteItem) {
  std::vector<CScreenItem*>::iterator it = std::find(m_Items.begin(), m_Items.end(), pItem);
  if(it == m_Items.end())
    return;

  m_Items.erase(it);
  if(deleteItem)
    delete pItem;
}

void CScreen::ClearItems(bool deleteItems) {
  if(deleteItems) {
    for(std::vector<CScreenItem*>::iterator it = m_Items.begin(); it != m_Items.end(); it++) {
      delete *it;
    }
  }
  m_Items.clear();
}

void CScreen::Render() {
  this->m_pGUI->Begin(this->m_Size);

  for(std::vector<CScreenItem*>::iterator it = m_Items.begin(); it != m_Items.end(); it++) {
    (*it)->Render();
  }

  this->m_pGUI->End();
}

CGUI * CScreen::GetGUI() const {
  return this->m_pGUI;
}

const glm::vec2 CScreen::GetSize() const {
  return m_Size;
}
