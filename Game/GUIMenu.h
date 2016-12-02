#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class CGame;
class CGUI;
class CGUIMenu;
class CGUIScreen;
class CGUIControllerList;
class CGUIFadeAnimation;

class CGUITextControl;
class CGUIRectControl;

class CGUIMenuItem {
public:
  enum MENUITEMFLAG {
    MIF_FOCUS = 0x0001,
    MIF_ENABLED = 0x0002,
    MIF_VISIBLE = 0x0004
  };
private:
  CGUIMenu*     m_pMenu;
  CGUITextControl*  m_pTextControl;
  CGUIRectControl*  m_pRectControl;
  CGUIFadeAnimation*  m_pRectFadeAnim;
  Uint32		m_Id;
  Uint32		m_UserDefId;
  Uint32		m_Flags;

public:
  CGUIMenuItem(CGUIMenu* pMenu, const Uint32 id, const std::string& name, Uint32 userDefID);
  ~CGUIMenuItem();

  const bool Update(CGame* pGame, const float fDT);

  void SetPos(const glm::vec2& pos);
  void SetName(const std::string& name);
  void SetUserDefID(const Uint32 id);
  void SetFocus(const bool focus);
  void SetEnable(const bool enable);
  void SetVisible(const bool visible);

  const Uint32 GetID() const;
  const std::string GetName() const;
  const Uint32 GetUserDefID() const;
  const bool HasFocus() const;
  const bool IsEnabled() const;
  const bool IsVisible() const;
};

class CGUIMenuManager;
class CGUIMenu {
public:
  enum MENUFLAG {
    MF_VISIBLE = 0x1
  };
private:
  CGUIMenuManager*    m_pMenuMng;
  CGUIScreen*         m_pScreen;
  CGUITextControl*    m_pTitleControl;
  CGUIControllerList* m_pControllerList;
  std::vector<CGUIMenuItem*> m_Items;
  Uint32	m_Id;
  Uint32	m_Flags;
  Uint32	m_Index;
  Uint32	m_ClickedID;
  glm::vec2 m_ItemsPos;
  glm::vec2 m_ItemsPadding;
  glm::vec4 m_ItemsTextColor;
  glm::vec4 m_ItemsRectColor;

public:
  CGUIMenu(CGUIMenuManager* pMenuMng, const Uint32 id, const std::string& title, const glm::vec2& size);
  ~CGUIMenu();

  const bool Update(CGame* pGame, const float timeDelta);
  void Render();

  CGUIMenuItem* AddMenuItem(const Uint32 id, const std::string& name, const Uint32 userDefId = 0);
  CGUIMenuItem* RemoveMenuItem(const Uint32 id, const bool deleteItem = false);
  CGUIMenuItem* GetMenuItem(const Uint32 id) const;
  void Clear(bool deleteItems = true);

  void SetTitle(const std::string& title);
  void SetVisible(const bool visible);

  CGUIScreen*  GetScreen() const;
  CGUIControllerList* GetControllerList() const;
  const std::string GetTitle() const;
  const bool IsVisible() const;
  const Uint32 GetClickedID() const;
  const Uint32 GetID() const;

  const glm::vec4 GetItemsTextColor() const;
  const glm::vec4 GetItemsRectColor() const;

private:
  void  UpdateItemsPos();
};

class CGUIMenuManager {
private:
  CGUI* m_pGUI;
  std::vector<CGUIMenu*>	m_MenuList;
  
  glm::vec2 m_Size;
  glm::vec2	m_MousePos;

  CGUIMenu* m_pCurrentMenu;
  CGUIMenu* m_pTargetMenu;

public:
  CGUIMenuManager(CGUI* pGUI, const glm::vec2& size = glm::vec2(640.0f, 480.0f));
  ~CGUIMenuManager();

  void SetSize(const glm::vec2& size);

  const glm::vec2&  GetSize() const;

  bool Update(CGame* pGame, const float timeDelta);
  void Render();

  CGUI* GetGUI() const;

  CGUIMenu* AddMenu(const Uint32 id, const std::string& name);
  CGUIMenu* GetMenu(const Uint32 id) const;
  CGUIMenu* RemoveMenu(Uint32 id, const bool deleteMenu = false);
  void Clear(const bool deleteMenus = true);

  CGUIMenu* GetCurrentMenu() const;
  bool SwitchToMenu(const Uint32 id);
  bool ForceSwitchToMenu(const Uint32 id);
};
