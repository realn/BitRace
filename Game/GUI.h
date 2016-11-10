#pragma once

#include "../Common/Vector.h"
#include "../Common/Files.h"

#include <SDL.h>
#include <glm/glm.hpp>
#include <vector>

class CGame;

class CGUI {
private:
  unsigned int m_FontTexture;
  unsigned int m_uFontList;

  bool LoadFontTexture(std::string file);
  bool InitFont();

public:
  CGUI();
  ~CGUI();

  bool Init();
  void Free();

  void Begin(const glm::vec2& size);
  void End();

  void Print(float x, float y, std::string str, ...);

  static void RenderProgressBar(vec2 vPos, vec2 vSize, float fProgress);
  static void RenderFSQuad(const glm::vec2& size);
  static void RenderFSQuadTex(const glm::vec2& size);
};

class CGUIMenuItem {
private:
  vec2		m_vPos;
  vec2		m_vCurrPos;
  std::string m_strName;
  Uint32		m_uID;
  Uint32		m_uUserDefID;
  Uint32		m_uFlag;
  float		m_fFocusLight;

public:
  enum MENUITEMFLAG {
    MIF_FOCUS = 0x0001,
    MIF_ENABLED = 0x0002,
    MIF_SHOWANIM = 0x0004,
    MIF_HIDEANIM = 0x0008,
    MIF_HIDDEN = 0x0010
  };
  CGUIMenuItem(Uint32 uID, std::string strName, vec2 vPos, Uint32 uUserDefID);
  ~CGUIMenuItem();

  bool Update(CGame* pGame, float fDT);
  void Render(CGUI* GUI);

  bool HasFocus();
  bool IsEnabled();
  bool IsAnimating();
  bool IsShowing();
  bool IsHiding();
  bool IsHidden();

  Uint32 GetID();
  Uint32 GetUserDefID();
  std::string GetName();
  vec2 GetPos();

  void SetFocus(bool bSet);
  void SetEnable(bool bSet);
  void SetUserDefID(Uint32 uSet);
  void SetName(std::string strName);
  void Show();
  void Hide();
  void ForceShow();
  void ForceHide();
};

class CGUIMenu {
private:
  std::vector<CGUIMenuItem*> m_aItem;
  std::string m_strName;
  Uint32	m_uID;
  Uint32	m_uFlag;
  float	m_fTime;
  Uint32	m_uIndex;
  Uint32	m_uClickedID;

public:
  enum MENUFLAG {
    MF_SHOWANIM = 0x01,
    MF_HIDEANIM = 0x02,
    MF_HIDDEN = 0x04
  };
  CGUIMenu(Uint32 uID, std::string strName);
  ~CGUIMenu();

  bool Update(CGame* pGame, float fDT);
  void Render(CGUI* GUI);

  CGUIMenuItem* AddMenuItem(Uint32 uID, std::string strName, vec2 vPos, Uint32 uUserDefID);
  void DelMenuItem(Uint32 uID);
  CGUIMenuItem* GetMenuItem(Uint32 uID);
  void Clear();

  void Hide();
  void Show();
  void ForceHide();
  void ForceShow();
  bool IsAnimating();
  bool IsHiden();
  bool IsHideing();
  bool IsShowing();
  Uint32 GetClickedID();
  Uint32 GetID();
  std::string GetName();
  void SetName(std::string strName);
};

class CGUIMenuManager {
private:
  std::vector<CGUIMenu*>	m_aMenu;
  glm::vec2 m_Size;
  glm::vec2	m_MousePos;
  Uint32	m_uCurrMenu;
  Uint32	m_uGoToMenu;

public:
  CGUIMenuManager(const glm::vec2& size = glm::vec2(640.0f, 480.0f));
  ~CGUIMenuManager();

  void SetSize(const glm::vec2& size);

  const glm::vec2&  GetSize() const;

  bool Update(CGame* pGame, float timeDelta);
  void Render(CGUI* GUI);

  CGUIMenu* AddMenu(Uint32 uID, std::string strName);
  void DelMenu(Uint32 uID);
  CGUIMenu* GetMenu(Uint32 uID);
  void Clear();
  CGUIMenu* GetCurrentMenu();
  bool SwitchToMenu(Uint32 uID);
  bool ForceSwitchToMenu(Uint32 uID);
};