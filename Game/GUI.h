#pragma once

#include "../Common/GLDevice.h"
#include "../Common/Vector.h"
#include "../Common/Files.h"
#define DIRECTINPUT_VERSION 0x0800
#include <DInput.h>
#include <vector>

class CGUI {
private:
  CGLDevice* m_pDevice;
  unsigned int m_uFontID;
  unsigned int m_uFontList;

  bool LoadFontTexture(std::string file);
  bool InitFont();

public:
  CGUI();
  ~CGUI();

  bool Init(CGLDevice* pDevice);
  void Free();

  void GUIMode_Start();
  void GUIMode_End();

  void Print(float x, float y, std::string str, ...);

  static void RenderProgressBar(vec2 vPos, vec2 vSize, float fProgress);
  static void RenderFSQuad();
  static void RenderFSQuadTex();
};

class CGUIMenuItem {
private:
  vec2		m_vPos;
  vec2		m_vCurrPos;
  std::string m_strName;
  UINT		m_uID;
  UINT		m_uUserDefID;
  UINT		m_uFlag;
  float		m_fFocusLight;

public:
  enum MENUITEMFLAG {
    MIF_FOCUS = 0x0001,
    MIF_ENABLED = 0x0002,
    MIF_SHOWANIM = 0x0004,
    MIF_HIDEANIM = 0x0008,
    MIF_HIDDEN = 0x0010
  };
  CGUIMenuItem(UINT uID, std::string strName, vec2 vPos, UINT uUserDefID);
  ~CGUIMenuItem();

  bool Engine(vec2 vMousePos, BYTE* MouseKey, float fDT);
  void Render(CGUI* GUI);

  bool HasFocus();
  bool IsEnabled();
  bool IsAnimating();
  bool IsShowing();
  bool IsHideing();
  bool IsHiden();

  UINT GetID();
  UINT GetUserDefID();
  std::string GetName();
  vec2 GetPos();

  void SetFocus(bool bSet);
  void SetEnable(bool bSet);
  void SetUserDefID(UINT uSet);
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
  UINT	m_uID;
  UINT	m_uFlag;
  float	m_fTime;
  UINT	m_uIndex;
  UINT	m_uClickedID;

public:
  enum MENUFLAG {
    MF_SHOWANIM = 0x01,
    MF_HIDEANIM = 0x02,
    MF_HIDDEN = 0x04
  };
  CGUIMenu(UINT uID, std::string strName);
  ~CGUIMenu();

  bool Engine(vec2 vMousePos, BYTE* MouseKey, float fDT);
  void Render(CGUI* GUI);

  CGUIMenuItem* AddMenuItem(UINT uID, std::string strName, vec2 vPos, UINT uUserDefID);
  void DelMenuItem(UINT uID);
  CGUIMenuItem* GetMenuItem(UINT uID);
  void Clear();

  void Hide();
  void Show();
  void ForceHide();
  void ForceShow();
  bool IsAnimating();
  bool IsHiden();
  bool IsHideing();
  bool IsShowing();
  UINT GetClickedID();
  UINT GetID();
  std::string GetName();
  void SetName(std::string strName);
};

class CGUIMenuManager {
private:
  std::vector<CGUIMenu*>	m_aMenu;
  vec2	m_vMouseAbsolute;
  UINT	m_uCurrMenu;
  UINT	m_uGoToMenu;
  bool	m_bPress;

public:
  CGUIMenuManager();
  ~CGUIMenuManager();

  bool Update( DIMOUSESTATE2* Mouse, float fDT);
  void Render(CGUI* GUI);

  CGUIMenu* AddMenu(UINT uID, std::string strName);
  void DelMenu(UINT uID);
  CGUIMenu* GetMenu(UINT uID);
  void Clear();
  CGUIMenu* GetCurrentMenu();
  bool SwitchToMenu(UINT uID);
  bool ForceSwitchToMenu(UINT uID);
};