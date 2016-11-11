#include "GUI.h"
#include "Game.h"
#include "../Common/FGXFile.h"
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

bool CGUI::LoadFontTexture(std::string filename) {
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
  const CFGXFile::CData& Data = imgFile.GetData();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, &Data[0]);
  glGenerateMipmap(GL_TEXTURE_2D);

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
    fontChar.m_Adv = 10.0f;

    this->m_CharList[(Uint8)i] = fontChar;
    //glNewList(this->m_uFontList + i, GL_COMPILE);
    //glBegin(GL_QUADS);

    //glTexCoord2f(cx, 1 - cy - 0.0625f);
    //glVertex2i(0, 16);

    //glTexCoord2f(cx + 0.0625f, 1 - cy - 0.0625f);
    //glVertex2i(16, 16);

    //glTexCoord2f(cx + 0.0625f, 1 - cy);
    //glVertex2i(16, 0);

    //glTexCoord2f(cx, 1 - cy);
    //glVertex2i(0, 0);

    //glEnd();
    //glTranslatef(10.0f, 0.0f, 0.0f);

    //glEndList();
  }

  return true;
}

bool CGUI::Init() {
  if (!LoadFontTexture("font.fgx"))
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

  glBindTexture(GL_TEXTURE_2D, this->m_FontTexture);
}

void CGUI::End() {
  glPopAttrib();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void CGUI::Print(float x, float y, std::string format, ...) {
  if (format.empty())
    return;

  std::string text;
  int len;
  va_list ap;

  va_start(ap, format);
  len = _vscprintf(format.c_str(), ap);
  text.resize(len);

  vsprintf_s(&text[0], len + 1, format.c_str(), ap);
  va_end(ap);

  std::vector<glm::vec2> vertList;
  std::vector<glm::vec2> texList;

  vertList.resize(4 * text.length());
  texList.resize(4 * text.length());

  glm::vec2 pos = glm::vec2(x, y);
  for (size_t i = 0; i < text.length(); i++) {
    Uint8 code = (Uint8)text[i] - 32;

    CFontChar& fontChar = this->m_CharList[code];

    for (size_t j = 0; j < 4; j++) {
      vertList[i * 4 + j] = this->m_Vertex[j] + pos;
      texList[i * 4 + j] = fontChar.m_TCoord[j];
    }

    pos += glm::vec2(fontChar.m_Adv, 0.0f);
  }

  glVertexPointer(2, GL_FLOAT, 0, &vertList[0]);
  glTexCoordPointer(2, GL_FLOAT, 0, &texList[0]);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glDrawArrays(GL_QUADS, 0, vertList.size());

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  //glPushMatrix();
  //glTranslatef(x, y, 0.0f);
  //glListBase(this->m_uFontList - 32);
  //glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, (unsigned char*)text);
  //glPopMatrix();
}

void CGUI::RenderProgressBar(glm::vec2 vPos, glm::vec2 vSize, float fProgress) {
  glDisable(GL_TEXTURE_2D);
  glBegin(GL_LINES);
  glVertex2fv((GLfloat*)&vPos);
  glVertex2f(vPos.x + vSize.x, vPos.y);

  glVertex2f(vPos.x + vSize.x, vPos.y);
  glVertex2f(vPos.x + vSize.x, vPos.y + vSize.y);

  glVertex2f(vPos.x + vSize.x, vPos.y + vSize.y);
  glVertex2f(vPos.x, vPos.y + vSize.y);

  glVertex2f(vPos.x, vPos.y + vSize.y);
  glVertex2fv(glm::value_ptr(vPos));
  glEnd();

  float fSizeX = vSize.x * (fProgress / 100.0f);

  glBegin(GL_QUADS);
  glVertex2fv(glm::value_ptr(vPos));

  glVertex2f(vPos.x + fSizeX, vPos.y);

  glVertex2f(vPos.x + fSizeX, vPos.y + vSize.y);

  glVertex2f(vPos.x, vPos.y + vSize.y);
  glEnd();
  glEnable(GL_TEXTURE_2D);
}

void CGUI::RenderFSQuad(const glm::vec2& size) {
  glDisable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glVertex2f(0.0f, 0.0f);
  glVertex2f(size.x, 0.0f);
  glVertex2f(size.x, size.y);
  glVertex2f(0.0f, size.y);
  glEnd();
  glEnable(GL_TEXTURE_2D);
}

void CGUI::RenderFSQuadTex(const glm::vec2& size) {
  glBegin(GL_QUADS);
  glTexCoord2i(0, 1);
  glVertex2f(0.0f, 0.0f);
  glTexCoord2i(1, 1);
  glVertex2f(size.x, 0.0f);
  glTexCoord2i(1, 0);
  glVertex2f(size.x, size.y);
  glTexCoord2i(0, 0);
  glVertex2f(0.0f, size.y);
  glEnd();
}

//==========================================================

CGUIMenuItem::CGUIMenuItem(Uint32 uID, std::string strName, glm::vec2 vPos, Uint32 uUserDefID) :
  m_uID(uID),
  m_strName(strName),
  m_vPos(vPos),
  m_vCurrPos(vPos.x, -20.0f),
  m_uUserDefID(uUserDefID),
  m_uFlag(MIF_ENABLED | MIF_HIDDEN),
  m_fFocusLight(0.0f) {

}

CGUIMenuItem::~CGUIMenuItem() {

}

bool CGUIMenuItem::Update(CGame* pGame, float fDT) {
  if (IsShowing()) {
    m_vCurrPos += (m_vPos - glm::vec2(m_vPos.x, -20.0f)) * 3.0f * fDT;
    if (m_vPos.y - m_vCurrPos.y < 0.1f) {
      this->m_uFlag &= ~MIF_SHOWANIM;
      m_vCurrPos = m_vPos;
    }
    else return false;
  }
  if (IsHiding()) {
    m_vCurrPos += (glm::vec2(m_vPos.x, -20.0f) - m_vPos) * 3.0f * fDT;
    if (m_vCurrPos.y + 20.0f < 0.1f) {
      this->m_uFlag &= ~MIF_HIDEANIM;
      this->m_uFlag |= MIF_HIDDEN;
      m_vCurrPos = glm::vec2(m_vPos.x, -20.0f);
    }
    else return false;
  }

  if (!IsEnabled())
    return false;

  if (HasFocus() && m_fFocusLight != 1.0f) {
    m_fFocusLight += 5.0f * fDT;
    if (m_fFocusLight > 1.0f)
      m_fFocusLight = 1.0f;
  }
  else if (m_fFocusLight != 0.0f) {
    m_fFocusLight -= 2.0f * fDT;
    if (m_fFocusLight < 0.0f)
      m_fFocusLight = 0.0f;
  }

  float width = 16.0f * float(m_strName.length());

  glm::vec2 vMousePos = glm::vec2(pGame->GetMousePos());

  if (vMousePos.x > m_vPos.x && vMousePos.x < m_vPos.x + width &&
      vMousePos.y > m_vPos.y && vMousePos.y < m_vPos.y + 16.0f) {
    SetFocus(true);
    if (pGame->IsMouseButtonPressed(SDL_BUTTON_LEFT))
      return true;
  }
  else SetFocus(false);
  return false;
}

void CGUIMenuItem::Render(CGUI *GUI) {
  if (!IsEnabled() || IsHidden())
    return;

  if (!IsAnimating()) {
    glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glColor4f(1.0f, 1.0f, 1.0f, m_fFocusLight);
    glVertex2f(0.0f, m_vPos.y);
    glVertex2f(0.0f, m_vPos.y + 16.0f);
    glColor4f(1.0f, 1.0f, 1.0f, m_fFocusLight * 0.2f);
    glVertex2f(640.0f, m_vPos.y + 16.0f);
    glVertex2f(640.0f, m_vPos.y);
    glEnd();
    glEnable(GL_TEXTURE_2D);
  }

  glColor3f(1.0f, 1.0f, 1.0f);
  GUI->Print(m_vCurrPos.x, m_vCurrPos.y, m_strName);
}

bool CGUIMenuItem::HasFocus() {
  return (m_uFlag & MIF_FOCUS) ? true : false;
}

bool CGUIMenuItem::IsEnabled() {
  return (m_uFlag & MIF_ENABLED) ? true : false;
}

bool CGUIMenuItem::IsAnimating() {
  return (IsShowing() || IsHiding());
}

bool CGUIMenuItem::IsHiding() {
  return (m_uFlag & MIF_HIDEANIM) ? true : false;
}

bool CGUIMenuItem::IsHidden() {
  return (m_uFlag & MIF_HIDDEN) ? true : false;
}

bool CGUIMenuItem::IsShowing() {
  return (m_uFlag & MIF_SHOWANIM) ? true : false;
}

Uint32 CGUIMenuItem::GetID() {
  return m_uID;
}

std::string CGUIMenuItem::GetName() {
  return m_strName;
}

glm::vec2 CGUIMenuItem::GetPos() {
  return m_vPos;
}

Uint32 CGUIMenuItem::GetUserDefID() {
  return m_uUserDefID;
}

void CGUIMenuItem::SetEnable(bool bSet) {
  if (bSet)
    m_uFlag |= MIF_ENABLED;
  else m_uFlag &= ~MIF_ENABLED;
}

void CGUIMenuItem::SetFocus(bool bSet) {
  if (bSet)
    m_uFlag |= MIF_FOCUS;
  else m_uFlag &= ~MIF_FOCUS;
}

void CGUIMenuItem::SetName(std::string strName) {
  m_strName = strName;
}

void CGUIMenuItem::SetUserDefID(Uint32 uSet) {
  m_uUserDefID = uSet;
}

void CGUIMenuItem::Show() {
  m_uFlag |= MIF_SHOWANIM;
  m_uFlag &= ~MIF_HIDDEN;
}

void CGUIMenuItem::Hide() {
  m_uFlag |= MIF_HIDEANIM;
}

void CGUIMenuItem::ForceShow() {
  m_uFlag &= ~MIF_HIDDEN;
  m_vCurrPos = m_vPos;
}

void CGUIMenuItem::ForceHide() {
  m_uFlag |= MIF_HIDDEN;
  m_vCurrPos = glm::vec2(m_vPos.x, -20.0f);
}

//========================================================

CGUIMenu::CGUIMenu(Uint32 uID, std::string strName) :
  m_strName(strName),
  m_uID(uID),
  m_uFlag(MF_HIDDEN),
  m_fTime(0.0f),
  m_uIndex(0),
  m_uClickedID(0) {

}

CGUIMenu::~CGUIMenu() {
  Clear();
}

bool CGUIMenu::Update(CGame* pGame, float fDT) {
  size_t i, j;
  if (IsShowing()) {
    m_fTime += fDT;
    if (m_fTime > 0.2f) {
      if (m_uIndex == 0) {
        for (j = 0, i = 0; i < m_aItem.size(); i++)
          if (!m_aItem[i]->IsShowing())
            j++;
        if (j == m_aItem.size())
          m_uFlag &= ~MF_SHOWANIM;
      }
      else {
        m_uIndex--;
        m_aItem[m_uIndex]->Show();
      }
      m_fTime = 0.0f;
    }
  }
  if (IsHideing()) {
    m_fTime += fDT;
    if (m_fTime > 0.2f) {
      if (m_uIndex >= Uint32(m_aItem.size())) {
        for (j = 0, i = 0; i < m_aItem.size(); i++)
          if (m_aItem[i]->IsHidden())
            j++;
        if (j == m_aItem.size()) {
          m_uFlag &= ~MF_HIDEANIM;
          m_uFlag |= MF_HIDDEN;
        }
      }
      else {
        m_aItem[m_uIndex]->Hide();
        m_uIndex++;
      }
    }
  }

  for (i = 0; i < m_aItem.size(); i++) {
    if (m_aItem[i]->Update(pGame, fDT)) {
      m_uClickedID = m_aItem[i]->GetID();
      return true;
    }
  }
  return false;
}

void CGUIMenu::Render(CGUI *GUI) {
  if (IsHiden())
    return;

  glPushMatrix();
  glScalef(3.0f, 3.0f, 1.0f);
  glColor3f(1.0f, 1.0f, 1.0f);
  GUI->Print(3.0f, 3.0f, m_strName);
  glPopMatrix();

  size_t i;
  for (i = 0; i < m_aItem.size(); i++)
    m_aItem[i]->Render(GUI);
}

CGUIMenuItem* CGUIMenu::AddMenuItem(Uint32 uID, std::string strName, glm::vec2 vPos, Uint32 uUserDefID) {
  size_t i;
  for (i = 0; i < m_aItem.size(); i++)
    if (m_aItem[i]->GetID() == uID)
      return NULL;

  CGUIMenuItem* m = new CGUIMenuItem(uID, strName, vPos, uUserDefID);
  m_aItem.push_back(m);
  return m;
}

void CGUIMenu::DelMenuItem(Uint32 uID) {
  size_t i;
  for (i = 0; i < m_aItem.size(); i++) {
    if (m_aItem[i]->GetID() == uID) {
      delete m_aItem[i];
      m_aItem.erase(m_aItem.begin() + i);
    }
  }
}

CGUIMenuItem* CGUIMenu::GetMenuItem(Uint32 uID) {
  size_t i;
  for (i = 0; i < m_aItem.size(); i++)
    if (m_aItem[i]->GetID() == uID)
      return m_aItem[i];
  return NULL;
}

void CGUIMenu::Clear() {
  int i;
  for (i = int(m_aItem.size()) - 1; i >= 0; i--) {
    delete m_aItem[i];
    m_aItem.pop_back();
  }
}

void CGUIMenu::Hide() {
  m_uFlag &= ~MF_SHOWANIM;
  m_uFlag |= MF_HIDEANIM;
  m_uIndex = 0;
}

void CGUIMenu::Show() {
  m_uFlag &= ~MF_HIDDEN;
  m_uFlag &= ~MF_HIDEANIM;
  m_uFlag |= MF_SHOWANIM;
  m_uIndex = Uint32(m_aItem.size());
}

bool CGUIMenu::IsAnimating() {
  return (IsShowing() || IsHideing());
}

bool CGUIMenu::IsHideing() {
  return (m_uFlag & MF_HIDEANIM) ? true : false;
}

bool CGUIMenu::IsHiden() {
  return (m_uFlag & MF_HIDDEN) ? true : false;
}

bool CGUIMenu::IsShowing() {
  return (m_uFlag & MF_SHOWANIM) ? true : false;
}

Uint32 CGUIMenu::GetClickedID() {
  return m_uClickedID;
}

std::string CGUIMenu::GetName() {
  return m_strName;
}

void CGUIMenu::SetName(std::string strName) {
  m_strName = strName;
}

Uint32 CGUIMenu::GetID() {
  return m_uID;
}

void CGUIMenu::ForceShow() {
  m_uFlag &= ~MF_HIDDEN;
  size_t i;
  for (i = 0; i < m_aItem.size(); i++)
    m_aItem[i]->ForceShow();
}

void CGUIMenu::ForceHide() {
  m_uFlag |= MF_HIDDEN;
  size_t i;
  for (i = 0; i < m_aItem.size(); i++)
    m_aItem[i]->ForceHide();
}

//===========================================================

CGUIMenuManager::CGUIMenuManager(const glm::vec2& size) :
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
      if (!Menu->IsHideing())
        Menu->Hide();
      if (Menu->IsHiden()) {
        Menu->ForceHide();
        this->m_uCurrMenu = this->m_uGoToMenu;
      }
    }
  }
  else {
    Menu = GetCurrentMenu();
    if (Menu != NULL && Menu->IsHiden() && !Menu->IsShowing())
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

void CGUIMenuManager::Render(CGUI *GUI) {
  CGUIMenu* Menu = this->GetCurrentMenu();
  if (Menu == NULL)
    return;

  Menu->Render(GUI);

  glDisable(GL_TEXTURE_2D);
  glColor3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_TRIANGLES);
  glVertex2f(m_MousePos.x, m_MousePos.y);
  glVertex2f(m_MousePos.x + 20.0f, m_MousePos.y + 10.0f);
  glVertex2f(m_MousePos.x + 10.0f, m_MousePos.y + 20.0f);
  glEnd();
  glEnable(GL_TEXTURE_2D);
}

CGUIMenu* CGUIMenuManager::AddMenu(Uint32 uID, std::string strName) {
  size_t i;
  for (i = 0; i < m_aMenu.size(); i++)
    if (m_aMenu[i]->GetID() == uID)
      return NULL;
  CGUIMenu* m = new CGUIMenu(uID, strName);
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