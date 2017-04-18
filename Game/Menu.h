#ifndef __BITRACE_MENU_H__
#define __BITRACE_MENU_H__

#include "GraphicView.h"
#include "LogicProcess.h"

#include "UIFont.h"
#include "UIMenu.h"

class CInputDeviceMap;

class CMenuProcess
  : public ILogicProcess
{
public:
  typedef std::map<cb::string, CUIMenu*> MenuMapT;

private:
  CUIFont mFont;
  MenuMapT mMenuMap;
  CUIMenu* mpCurrentMenu;
  CInputDeviceMap& mIDevMap;
  glm::vec2 mPointerPos;
  CUIMenuLayout mLayout;

public:
  CMenuProcess(CInputDeviceMap& inputDevMap);
  virtual ~CMenuProcess();

  const CUIFont& GetFont() const;
  const CUIMenu* GetCurrent() const;
  const glm::vec2 GetPointerPos() const;
  const CUIMenuLayout& GetLayout() const;

  const bool Init();
  void Free();

  // Inherited via ILogicProcess
  virtual void Update(const float timeDelta) override;
};

class CMenuView 
  : public IGraphicView
{
private:
  const CMenuProcess& mProcess;
  CUIText mText;
  CUIMenuView mMenuView;
  glm::vec2 mScreenSize;

public:
  CMenuView(const CMenuProcess& process, const glm::vec2& screenSize);
  virtual ~CMenuView();

  const bool Init();
  void Free();

  // Inherited via IGraphicView
  virtual void RenderView() const override;
  virtual void RenderUI() const override;
};

#endif // !__BITRACE_MENU_H__

