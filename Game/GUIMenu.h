#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class CGame;
class CGUI;
class CGUIMenu;
class CGUIScreen;
class CGUIScreenItem;
class CGUIScreenTextItem;
class CGUIScreenRectItem;

class CGUIMenuItem {
private:
  CGUIMenu*     m_pMenu;
  CGUIScreenTextItem*  m_pTextItem;
  CGUIScreenRectItem*  m_pRectItem;
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

  const bool Update(CGame* pGame, const float fDT);

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
  CGUIMenuManager*    m_pMenuMng;
  CGUIScreen*         m_pScreen;
  CGUIScreenTextItem* m_pTitleItem;
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

  const bool Update(CGame* pGame, const float timeDelta);
  void Render();

  CGUIMenuItem* AddMenuItem(const Uint32 id, const std::string& name, const glm::vec2& pos, const Uint32 userDefId = 0);
  CGUIMenuItem* RemoveMenuItem(const Uint32 id, const bool deleteItem = false);
  CGUIMenuItem* GetMenuItem(const Uint32 id) const;
  void Clear(bool deleteItems = true);

  void SetTitle(const std::string& title);
  void Hide();
  void Show();
  void ForceHide();
  void ForceShow();

  CGUIScreen*  GetScreen() const;
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

  bool Update(CGame* pGame, float timeDelta);
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
