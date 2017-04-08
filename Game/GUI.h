#pragma once

#include <SDL.h>
#include <glm/glm.hpp>
#include <map>

#include <CBIO/Defines.h>

class CInputDeviceMap;

class CGUI {
public:
  struct CFontChar {
    Uint8 m_Code;
    glm::vec2   m_TCoord[4];
    glm::vec2   m_Size;
    float m_Adv;
  };

private:
  std::map<Uint8, CFontChar> m_CharList;
  unsigned int m_FontTexture;
  unsigned int m_uFontList;
  glm::vec2 m_Size;
  glm::vec2 m_Vertex[4];

  bool LoadFontTexture(const cb::string& filename);
  bool InitFont();

public:
  CGUI();
  ~CGUI();

  bool Init();
  void Free();

  void Begin(const glm::vec2& size);
  void End();

  void Print(const glm::vec2& pos, std::string format, ...) const;

  const glm::vec2 GetPrintSize(const std::string format, ...) const;

  void RenderQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const Uint32 texId = 0);
  void RenderQuadLines(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
  void RenderQuadFullScreen(const glm::vec2& size, const glm::vec4& color, const Uint32 texId = 0);
  void RenderProgressBar(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const float progress);

private:
  const CFontChar&  GetChar(const char charCode) const;
  const std::string FormatText(const std::string& format, va_list va) const;
};

class CScreen;
class CScreenItem {
public:
  enum ITEMTYPE {
    IT_TEXT = 0,
    IT_RECTFILL = 1,
    IT_RECTLINE = 2,
  };
  enum ITEMALIGN {
    IA_LEFT   = 0x1,
    IA_RIGHT  = 0x2,
    IA_TOP    = 0x4,
    IA_BOTTOM = 0x8,
    IA_CENTER = (IA_LEFT | IA_RIGHT),
    IA_MIDDLE = (IA_TOP | IA_BOTTOM),
    IA_DEFAULT = (IA_LEFT | IA_TOP)
  };

private:
  CScreen*  m_pScreen;
  ITEMTYPE  m_Type;
  glm::vec2 m_Pos;
  glm::vec2 m_Size;
  glm::vec4 m_Color;
  Uint32    m_Align;
  std::string m_Text;
  bool      m_Visible;

public:
  CScreenItem(CScreen* pScreen, const ITEMTYPE type, const glm::vec2& pos, const glm::vec2& size);
  ~CScreenItem();

  void  Render();

  const bool Contains(const glm::vec2& point) const;

  void  SetPos(const glm::vec2& pos);
  void  SetSize(const glm::vec2& size);
  void  SetColor(const glm::vec4& color);
  void  SetAlign(const Uint32 flags);
  void  SetText(const std::string& text, bool changeSize = true);
  void  SetVisible(const bool visible);

  const glm::vec2&  GetPos() const;
  const glm::vec2&  GetSize() const;
  const glm::vec4&  GetColor() const;
  const Uint32      GetAlign() const;
  const std::string&  GetText() const;
  const bool  IsVisible() const;

private:
  const glm::vec2 GetRenderPos() const;
};

class CScreen {
private:
  std::vector<CScreenItem*> m_Items;
  CGUI* m_pGUI;
  glm::vec2 m_Size;

public:
  CScreen(CGUI* pGUI, const glm::vec2& size);
  ~CScreen();

  void  AddItem(CScreenItem* pItem);
  void  RemoveItem(CScreenItem* pItem, bool deleteItem = false);
  void  ClearItems(bool deleteItems = true);

  void  Render();

  CGUI* GetGUI() const;
  const glm::vec2 GetSize() const;
};

class CGUIMenu;
class CGUIMenuItem {
private:
  CGUIMenu*     m_pMenu;
  CScreenItem*  m_pTextItem;
  CScreenItem*  m_pRectItem;
  glm::vec2		m_ShowPos;
  glm::vec2   m_HidePos;
  Uint32		m_Id;
  Uint32		m_UserDefId;
  Uint32		m_Flags;
  float	    m_FocusLight;
  float     m_AnimSpeed;
  float     m_FocusUpSpeed;
  float     m_FocusDownSpeed;

public:
  enum MENUITEMFLAG {
    MIF_FOCUS = 0x0001,
    MIF_ENABLED = 0x0002,
    MIF_SHOWANIM = 0x0004,
    MIF_HIDEANIM = 0x0008,
    MIF_HIDDEN = 0x0010
  };
  CGUIMenuItem(CGUIMenu* pMenu, const Uint32 id, const std::string& name, const glm::vec2& pos, Uint32 userDefID);
  ~CGUIMenuItem();

  const bool Update(const CInputDeviceMap& input, const float fDT);

  const bool HasFocus() const;
  const bool IsEnabled() const;
  const bool IsAnimating() const;
  const bool IsVisible() const;
  const bool IsShowing() const;
  const bool IsHiding() const;
  const bool IsHidden() const;

  const Uint32 GetID() const;
  const Uint32 GetUserDefID() const;
  const std::string GetName() const;
  const glm::vec2 GetPos() const;

  void SetFocus(const bool focus);
  void SetEnable(const bool enable);
  void SetUserDefID(const Uint32 id);
  void SetName(const std::string& name);
  void Show();
  void Hide();
  void ForceShow();
  void ForceHide();
};

class CGUIMenuManager;
class CGUIMenu {
private:
  CGUIMenuManager*  m_pMenuMng;
  CScreen*      m_pScreen;
  CScreenItem*  m_pTitleItem;
  std::vector<CGUIMenuItem*> m_Items;
  Uint32	m_Id;
  Uint32	m_Flags;
  float	  m_Time;
  Uint32	m_Index;
  Uint32	m_ClickedID;

public:
  enum MENUFLAG {
    MF_SHOWANIM = 0x01,
    MF_HIDEANIM = 0x02,
    MF_HIDDEN = 0x04
  };
  CGUIMenu(CGUIMenuManager* pMenuMng, const Uint32 id, const std::string& title, const glm::vec2& size);
  ~CGUIMenu();

  const bool Update(const CInputDeviceMap& input, const float timeDelta);
  void Render();

  CGUIMenuItem* AddMenuItem(const Uint32 id, const std::string& name, const glm::vec2& pos, const Uint32 userDefId = 0);
  CGUIMenuItem* AddCheckBox(const Uint32 id, const std::string& name, const bool check, const glm::vec2& pos, const Uint32 userDefId = 0);
  CGUIMenuItem* RemoveMenuItem(const Uint32 id, const bool deleteItem = false);
  CGUIMenuItem* GetMenuItem(const Uint32 id) const;
  void Clear(bool deleteItems = true);

  void SetTitle(const std::string& title);
  void Hide();
  void Show();
  void ForceHide();
  void ForceShow();

  CScreen*  GetScreen() const;
  const std::string GetTitle() const;
  const bool IsAnimating() const;
  const bool IsHidden() const;
  const bool IsHiding() const;
  const bool IsVisible() const;
  const bool IsShowing() const;
  const Uint32 GetClickedID() const;
  const Uint32 GetID() const;

private:
  const bool  AllItemsVisible() const;
  const bool  AllItemsHidden() const;
};

class CGUIMenuManager {
private:
  CGUI* m_pGUI;
  std::vector<CGUIMenu*>	m_aMenu;
  glm::vec2 m_Size;
  glm::vec2	m_MousePos;
  Uint32	m_uCurrMenu;
  Uint32	m_uGoToMenu;

public:
  CGUIMenuManager(CGUI* pGUI, const glm::vec2& size = glm::vec2(640.0f, 480.0f));
  ~CGUIMenuManager();

  void SetSize(const glm::vec2& size);

  const glm::vec2&  GetSize() const;

  bool Update(const CInputDeviceMap& input, float timeDelta);
  void Render();

  CGUI* GetGUI() const;

  CGUIMenu* AddMenu(Uint32 uID, std::string strName);
  void DelMenu(Uint32 uID);
  CGUIMenu* GetMenu(Uint32 uID);
  void Clear();
  CGUIMenu* GetCurrentMenu();
  bool SwitchToMenu(Uint32 uID);
  bool ForceSwitchToMenu(Uint32 uID);
};