#include "GUI.h"
#include "../Common/FGXFile.h"
#include <stdarg.h>

CGUI::CGUI() : m_pDevice(NULL) {

}

CGUI::~CGUI() {
  Free();
}

bool CGUI::LoadFontTexture(std::string file) {
  FGXHEADER head;
  CFile fp;
  if (!fp.Open(file, "rb"))
    return false;

  fp.Read(&head, sizeof(FGXHEADER));

  if (strncmp(head.FILEID, FGX_FILEID, 3) != 0)
    return false;
  if (head.FILEVERSION != FGX_VERSION)
    return false;

  std::vector<BYTE> Data;
  Data.resize(head.IMAGEWIDTH * head.IMAGEHEIGHT * head.IMAGEDEPTH);

  fp.Read(&Data[0], 1, (unsigned int)Data.size());
  fp.Close();

  glGenTextures(1, &this->m_uFontID);
  glBindTexture(GL_TEXTURE_2D, this->m_uFontID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  UINT format = 0;
  switch (head.IMAGEDEPTH) {
  case 1: format = GL_LUMINANCE8;
  case 2: format = GL_LUMINANCE8_ALPHA8;
  case 3: format = GL_RGB;
  case 4: format = GL_RGBA;
  };

  int err;
  if ((err = gluBuild2DMipmaps(GL_TEXTURE_2D,
                               head.IMAGEDEPTH, head.IMAGEWIDTH, head.IMAGEHEIGHT,
                               format, GL_UNSIGNED_BYTE, &Data[0])) != 0) {
    Log("GLU ERROR: Can't create font texture: %s", (char*)gluErrorString(err));
    return false;
  }
  return true;
}

bool CGUI::InitFont() {
  this->m_uFontList = glGenLists(256);

  UINT i;
  float cx;
  float cy;
  for (i = 0; i < 256; ++i) {
    cx = float(i % 16) / 16.0f;
    cy = float(i / 16) / 16.0f;

    glNewList(this->m_uFontList + i, GL_COMPILE);
    glBegin(GL_QUADS);

    glTexCoord2f(cx, 1 - cy - 0.0625f);
    glVertex2i(0, 16);

    glTexCoord2f(cx + 0.0625f, 1 - cy - 0.0625f);
    glVertex2i(16, 16);

    glTexCoord2f(cx + 0.0625f, 1 - cy);
    glVertex2i(16, 0);

    glTexCoord2f(cx, 1 - cy);
    glVertex2i(0, 0);

    glEnd();
    glTranslatef(10.0f, 0.0f, 0.0f);

    glEndList();
  }

  return true;
}

bool CGUI::Init(CGLDevice *pDevice) {
  if (!LoadFontTexture("font.fgx"))
    return false;
  if (!InitFont())
    return false;

  m_pDevice = pDevice;

  return true;
}

void CGUI::Free() {
  if (glIsList(this->m_uFontList))
    glDeleteLists(this->m_uFontList, 256);
  if (glIsTexture(this->m_uFontID))
    glDeleteTextures(1, &this->m_uFontID);
  m_pDevice = NULL;
}

void CGUI::GUIMode_Start() {
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0.0f, 640.0f, 480.0f, 0.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glPushAttrib(GL_ENABLE_BIT);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, this->m_uFontID);
}

void CGUI::GUIMode_End() {
  glPopAttrib();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void CGUI::Print(float x, float y, std::string str, ...) {
  char* text;
  int len;
  va_list ap;

  va_start(ap, str);

  len = _vscprintf(str.c_str(), ap) + 1;
  text = new char[len];

  vsprintf_s(text, len, str.c_str(), ap);
  va_end(ap);

  glPushMatrix();
  glTranslatef(x, y, 0.0f);
  glListBase(this->m_uFontList - 32);
  glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, (BYTE*)text);
  glPopMatrix();

  delete[] text;
}

void CGUI::RenderProgressBar(vec2 vPos, vec2 vSize, float fProgress) {
  glDisable(GL_TEXTURE_2D);
  glBegin(GL_LINES);
  glVertex2fv((GLfloat*)&vPos);
  glVertex2f(vPos.X + vSize.X, vPos.Y);

  glVertex2f(vPos.X + vSize.X, vPos.Y);
  glVertex2f(vPos.X + vSize.X, vPos.Y + vSize.Y);

  glVertex2f(vPos.X + vSize.X, vPos.Y + vSize.Y);
  glVertex2f(vPos.X, vPos.Y + vSize.Y);

  glVertex2f(vPos.X, vPos.Y + vSize.Y);
  glVertex2fv((GLfloat*)&vPos);
  glEnd();

  float fSizeX = vSize.X * (fProgress / 100.0f);

  glBegin(GL_QUADS);
  glVertex2fv((GLfloat*)&vPos);

  glVertex2f(vPos.X + fSizeX, vPos.Y);

  glVertex2f(vPos.X + fSizeX, vPos.Y + vSize.Y);

  glVertex2f(vPos.X, vPos.Y + vSize.Y);
  glEnd();
  glEnable(GL_TEXTURE_2D);
}

void CGUI::RenderFSQuad() {
  glDisable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
  glVertex2i(0, 0);
  glVertex2i(640, 0);
  glVertex2i(640, 480);
  glVertex2i(0, 480);
  glEnd();
  glEnable(GL_TEXTURE_2D);
}

void CGUI::RenderFSQuadTex() {
  glBegin(GL_QUADS);
  glTexCoord2i(0, 1);
  glVertex2i(0, 0);
  glTexCoord2i(1, 1);
  glVertex2i(640, 0);
  glTexCoord2i(1, 0);
  glVertex2i(640, 480);
  glTexCoord2i(0, 0);
  glVertex2i(0, 480);
  glEnd();
}

//==========================================================

CGUIMenuItem::CGUIMenuItem(UINT uID, std::string strName, vec2 vPos, UINT uUserDefID) :
  m_uID(uID),
  m_strName(strName),
  m_vPos(vPos),
  m_vCurrPos(vPos.X, -20.0f),
  m_uUserDefID(uUserDefID),
  m_uFlag(MIF_ENABLED | MIF_HIDDEN),
  m_fFocusLight(0.0f) {

}

CGUIMenuItem::~CGUIMenuItem() {

}

bool CGUIMenuItem::Engine(vec2 vMousePos, BYTE *MouseKey, float fDT) {
  if (IsShowing()) {
    m_vCurrPos += (m_vPos - vec2(m_vPos.X, -20.0f)) * 3.0f * fDT;
    if (m_vPos.Y - m_vCurrPos.Y < 0.1f) {
      this->m_uFlag &= ~MIF_SHOWANIM;
      m_vCurrPos = m_vPos;
    }
    else return false;
  }
  if (IsHideing()) {
    m_vCurrPos += (vec2(m_vPos.X, -20.0f) - m_vPos) * 3.0f * fDT;
    if (m_vCurrPos.Y + 20.0f < 0.1f) {
      this->m_uFlag &= ~MIF_HIDEANIM;
      this->m_uFlag |= MIF_HIDDEN;
      m_vCurrPos = vec2(m_vPos.X, -20.0f);
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

  if (vMousePos.X > m_vPos.X && vMousePos.X < m_vPos.X + width &&
      vMousePos.Y > m_vPos.Y && vMousePos.Y < m_vPos.Y + 16.0f) {
    SetFocus(true);
    if (MouseKey[0] & 0x80)
      return true;
  }
  else SetFocus(false);
  return false;
}

void CGUIMenuItem::Render(CGUI *GUI) {
  if (!IsEnabled() || IsHiden())
    return;

  if (!IsAnimating()) {
    glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glColor4f(1.0f, 1.0f, 1.0f, m_fFocusLight);
    glVertex2f(0.0f, m_vPos.Y);
    glVertex2f(0.0f, m_vPos.Y + 16.0f);
    glColor4f(1.0f, 1.0f, 1.0f, m_fFocusLight * 0.2f);
    glVertex2f(640.0f, m_vPos.Y + 16.0f);
    glVertex2f(640.0f, m_vPos.Y);
    glEnd();
    glEnable(GL_TEXTURE_2D);
  }

  glColor3f(1.0f, 1.0f, 1.0f);
  GUI->Print(m_vCurrPos.X, m_vCurrPos.Y, m_strName);
}

bool CGUIMenuItem::HasFocus() {
  return (m_uFlag & MIF_FOCUS) ? true : false;
}

bool CGUIMenuItem::IsEnabled() {
  return (m_uFlag & MIF_ENABLED) ? true : false;
}

bool CGUIMenuItem::IsAnimating() {
  return (IsShowing() || IsHideing());
}

bool CGUIMenuItem::IsHideing() {
  return (m_uFlag & MIF_HIDEANIM) ? true : false;
}

bool CGUIMenuItem::IsHiden() {
  return (m_uFlag & MIF_HIDDEN) ? true : false;
}

bool CGUIMenuItem::IsShowing() {
  return (m_uFlag & MIF_SHOWANIM) ? true : false;
}

UINT CGUIMenuItem::GetID() {
  return m_uID;
}

std::string CGUIMenuItem::GetName() {
  return m_strName;
}

vec2 CGUIMenuItem::GetPos() {
  return m_vPos;
}

UINT CGUIMenuItem::GetUserDefID() {
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

void CGUIMenuItem::SetUserDefID(UINT uSet) {
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
  m_vCurrPos = vec2(m_vPos.X, -20.0f);
}

//========================================================

CGUIMenu::CGUIMenu(UINT uID, std::string strName) :
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

bool CGUIMenu::Engine(vec2 vMousePos, BYTE *MouseKey, float fDT) {
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
      if (m_uIndex >= UINT(m_aItem.size())) {
        for (j = 0, i = 0; i < m_aItem.size(); i++)
          if (m_aItem[i]->IsHiden())
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
    if (m_aItem[i]->Engine(vMousePos, MouseKey, fDT)) {
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

CGUIMenuItem* CGUIMenu::AddMenuItem(UINT uID, std::string strName, vec2 vPos, UINT uUserDefID) {
  size_t i;
  for (i = 0; i < m_aItem.size(); i++)
    if (m_aItem[i]->GetID() == uID)
      return NULL;

  CGUIMenuItem* m = new CGUIMenuItem(uID, strName, vPos, uUserDefID);
  m_aItem.push_back(m);
  return m;
}

void CGUIMenu::DelMenuItem(UINT uID) {
  size_t i;
  for (i = 0; i < m_aItem.size(); i++) {
    if (m_aItem[i]->GetID() == uID) {
      delete m_aItem[i];
      m_aItem.erase(m_aItem.begin() + i);
    }
  }
}

CGUIMenuItem* CGUIMenu::GetMenuItem(UINT uID) {
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
  m_uIndex = UINT(m_aItem.size());
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

UINT CGUIMenu::GetClickedID() {
  return m_uClickedID;
}

std::string CGUIMenu::GetName() {
  return m_strName;
}

void CGUIMenu::SetName(std::string strName) {
  m_strName = strName;
}

UINT CGUIMenu::GetID() {
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

CGUIMenuManager::CGUIMenuManager() :
  m_vMouseAbsolute(0.0f),
  m_uCurrMenu(0),
  m_uGoToMenu(0),
  m_bPress(false) {

}

CGUIMenuManager::~CGUIMenuManager() {
  Clear();
}

bool CGUIMenuManager::Update(DIMOUSESTATE2 *Mouse, float fDT) {
  this->m_vMouseAbsolute.X += float(Mouse->lX);
  this->m_vMouseAbsolute.Y += float(Mouse->lY);

  if (m_vMouseAbsolute.X < 0.0f)
    m_vMouseAbsolute.X = 0.0f;
  if (m_vMouseAbsolute.X > 640.0f)
    m_vMouseAbsolute.X = 640.0f;
  if (m_vMouseAbsolute.Y < 0.0f)
    m_vMouseAbsolute.Y = 0.0f;
  if (m_vMouseAbsolute.Y > 480.0f)
    m_vMouseAbsolute.Y = 480.0f;

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
    if (Menu->Engine(this->m_vMouseAbsolute, Mouse->rgbButtons, fDT)) {
      if (!m_bPress) {
        m_bPress = true;
        return true;
      }
    }
    else m_bPress = false;
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
  glVertex2f(m_vMouseAbsolute.X, m_vMouseAbsolute.Y);
  glVertex2f(m_vMouseAbsolute.X + 20.0f, m_vMouseAbsolute.Y + 10.0f);
  glVertex2f(m_vMouseAbsolute.X + 10.0f, m_vMouseAbsolute.Y + 20.0f);
  glEnd();
  glEnable(GL_TEXTURE_2D);
}

CGUIMenu* CGUIMenuManager::AddMenu(UINT uID, std::string strName) {
  size_t i;
  for (i = 0; i < m_aMenu.size(); i++)
    if (m_aMenu[i]->GetID() == uID)
      return NULL;
  CGUIMenu* m = new CGUIMenu(uID, strName);
  m_aMenu.push_back(m);
  return m;
}

void CGUIMenuManager::DelMenu(UINT uID) {
  size_t i;
  for (i = 0; i < m_aMenu.size(); i++)
    if (m_aMenu[i]->GetID() == uID) {
      delete m_aMenu[i];
      m_aMenu.erase(m_aMenu.begin() + i);
      return;
    }
}

CGUIMenu* CGUIMenuManager::GetMenu(UINT uID) {
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
  if (this->m_uCurrMenu >= UINT(m_aMenu.size()))
    return NULL;
  return m_aMenu[m_uCurrMenu];
}

bool CGUIMenuManager::SwitchToMenu(UINT uID) {
  size_t i;
  for (i = 0; i < m_aMenu.size(); i++) {
    if (m_aMenu[i]->GetID() == uID) {
      this->m_uGoToMenu = UINT(i);
      return true;
    }
  }
  return false;
}

bool CGUIMenuManager::ForceSwitchToMenu(UINT uID) {
  size_t i;
  for (i = 0; i < m_aMenu.size(); i++) {
    if (m_aMenu[i]->GetID() == uID) {
      this->m_uCurrMenu = UINT(i);
      this->m_uGoToMenu = UINT(i);
      return true;
    }
  }
  return false;
}