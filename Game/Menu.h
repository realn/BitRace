//#ifndef __BITRACE_MENU_H__
//#define __BITRACE_MENU_H__
//
//#include "GraphicView.h"
//#include "FrameProcess.h"
//
//#include "UIFont.h"
//#include "UIMenu.h"
//
//class CMenuState {
//public:
//  typedef std::map<cb::string, CUIMenu*> MenuMapT;
//
//private:
//  CUIFont mFont;
//  CUIMenuLayout mLayout;
//  MenuMapT mMenuMap;
//  CUIMenu* mpCurrentMenu;
//  glm::vec2 mPointerPos;
//
//public:
//  CMenuState();
//  ~CMenuState();
//
//  void SetPointerPos(const glm::vec2& pos);
//
//  const CUIFont& GetFont() const;
//  const CUIMenuLayout GetLayout() const;
//  const CUIMenu& GetCurrent() const;
//  CUIMenu& GetCurrent();
//  const glm::vec2 GetPointerPos() const;
//
//};
//
//class CInputDeviceMap;
//
//class CMenuStateProcess
//  : public IFrameProcess
//{
//private:
//  CMenuState& mState;
//  CInputDeviceMap& mIDevMap;
//
//public:
//  CMenuStateProcess(CMenuState& state, CInputDeviceMap& inputDevMap);
//  virtual ~CMenuStateProcess();
//
//  // Inherited via IFrameProcess
//  virtual void Update(const float timeDelta) override;
//};
//
//class CMenuView 
//  : public IGraphicView
//{
//private:
//  const CMenuProcess& mProcess;
//  CUIText mText;
//  CUIMenuView mMenuView;
//  glm::vec2 mScreenSize;
//
//public:
//  CMenuView(const CMenuProcess& process, const glm::vec2& screenSize);
//  virtual ~CMenuView();
//
//  const bool Init();
//  void Free();
//
//  // Inherited via IGraphicView
//  virtual void RenderView() const override;
//  virtual void RenderUI() const override;
//};
//
//#endif // !__BITRACE_MENU_H__
//
