#ifndef __BITRACE_MENU_H__
#define __BITRACE_MENU_H__

#include "GraphicView.h"
#include "LogicProcess.h"
#include "GUI.h"

class CRaceTrack;
class CInputDeviceMap;
class CConfig;

class IMenuEventObserver {
protected:
  IMenuEventObserver() {}
public:
  virtual ~IMenuEventObserver() {};

  virtual void MenuItemAction(CGUIMenuManager& menuMng,
                              CGUIMenu& menu, 
                              CGUIMenuItem& item) = 0;
};

class CMenuProcess
  : public ILogicProcess
{
public:
  enum MENU_ID {
    MENU_MAIN = 0,
    MENU_HIGH,
    MENU_OPTIONS
  };
  enum MENUITEM_ID {
    MI_NEWGAME = 0,
    MI_HIGH,
    MI_OPTIONS,
    MI_EXIT,
    MI_GOBACK,
    MI_RETURN,

    MI_RESOLUTION,
    MI_FULLSCREEN,
    MI_SMOOTHSHADE,
    MI_SMOOTHLINE,
    MI_FPSCOUNTER,
    MI_VSYNC,
    MI_OPWARNING,

    MI_HS1,
    MI_HS2,
    MI_HS3,
    MI_HS4,
    MI_HS5,
    MI_HS6,
    MI_HS7,
    MI_HS8,
    MI_HS9,
    MI_HS10,
    MI_HSRESET
  };

private:
  typedef std::vector<IMenuEventObserver*> ObserverVectorT;

  bool  mPauseMenu;
  CGUIMenuManager	mMenuMng;
  CInputDeviceMap& mIDevMap;
  ObserverVectorT mObservers;
  glm::uvec2 mScreenSize;

public:
  CMenuProcess(CGUI& gui, CInputDeviceMap& inputDevMap);
  virtual ~CMenuProcess();

  const bool Init(const CConfig& config);

  void AddObserver(IMenuEventObserver* pObserver);
  void RemoveObserver(IMenuEventObserver* pObserver);

  void SetPauseMenu(const bool value);
  const bool IsPauseMenu() const;

  CGUIMenuManager& GetMenuManager();

  // Inherited via ILogicProcess
  virtual void Update(const float timeDelta) override;
};

class CMenuView 
  : public IGraphicView
{
private:
  CMenuProcess& mMenuProcess;
  //CRaceTrack& mRaceTrack;
  glm::uvec2 mScreenSize;

public:
  CMenuView(CMenuProcess& menuProcess);
  virtual ~CMenuView();

  const bool Init(const glm::uvec2& screenSize);

  // Inherited via IGraphicView
  virtual void Render(const glm::mat4& transform) const override;
  virtual void RenderUI(CGUI & gui) const override;
};

#endif // !__BITRACE_MENU_H__

