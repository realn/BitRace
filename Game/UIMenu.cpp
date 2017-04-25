#include "stdafx.h"
//#include "UIMenu.h"
//#include "UIFont.h"
//
//CUIMenuLayout::CUIMenuLayout() 
//  : ItemsPos(2.0f, 4.0f)
//  , ItemPadding(1.0f, 0.5f)
//  , TitlePos(3.0f, 1.0f)
//  , TitleScale(2.0f)
//{}
//
//
//CUIMenu::CItem::CItem(const Uint32 id, const cb::string & text) 
//  : Id(id)
//  , Text(text)
//  , Flags(0)
//  , HLValue(0.0f)
//{}
//
//CUIMenu::CItem::CItem(const CItem & other)
//  : Id(other.Id)
//  , Text(other.Text)
//  , Flags(other.Flags)
//  , HLValue(other.HLValue)
//{}
//
//CUIMenu::CUIMenu(const cb::string& title)
//  : mTitle(title)
//{}
//
//CUIMenu::~CUIMenu() {}
//
//const cb::string & CUIMenu::GetTitle() const {
//  return mTitle;
//}
//
//void CUIMenu::CalcItemsPosSize(const CUIFont & font, const CUIMenuLayout & layout) {
//  glm::vec2 pos = layout.ItemsPos;
//  for(iterator it = Begin(); it != End(); it++) {
//    it->Pos = pos;
//    it->Size = font.GetSize(it->Text);
//
//    pos += glm::vec2(0.0f, it->Size.y);
//    pos += glm::vec2(0.0f, layout.ItemPadding.y);
//  }
//}
//
//CUIMenu::iterator CUIMenu::AddItem(const Uint32 id, const cb::string & text) {
//  iterator it = FindItem(id);
//  if(it != End()) {
//    it->Text = text;
//    return it;
//  }
//
//  CItem item(id, text);
//  mItems.push_back(item);
//
//  return FindItem(id);
//}
//
//CUIMenu::iterator CUIMenu::FindItem(const Uint32 id) {
//  for(iterator it = Begin(); it != End(); it++) {
//    if(it->Id == id)
//      return it;
//  }
//  return End();
//}
//
//CUIMenu::const_iterator CUIMenu::FindItem(const Uint32 id) const {
//  for(const_iterator it = Begin(); it != End(); it++) {
//    if(it->Id == id)
//      return it;
//  }
//  return End();
//}
//
//const CUIMenu::CItem & CUIMenu::GetItem(const Uint32 index) const {
//  return mItems[index];
//}
//
//CUIMenu::iterator CUIMenu::Erase(const_iterator it) {
//  return mItems.erase(it);
//}
//
//void CUIMenu::Clear() {
//  mItems.clear();
//}
//
//const Uint32 CUIMenu::GetSize() const {
//  return mItems.size();
//}
//
//CUIMenu::iterator CUIMenu::Begin() {
//  return mItems.begin();
//}
//
//CUIMenu::const_iterator CUIMenu::Begin() const {
//  return mItems.begin();
//}
//
//CUIMenu::iterator CUIMenu::End() {
//  return mItems.end();
//}
//
//CUIMenu::const_iterator CUIMenu::End() const {
//  return mItems.end();
//}
//
//CUIMenuView::CUIMenuView(const glm::vec2& screenSize) 
//  : mScreenSize(screenSize)
//  , mItemColor(0.5f, 0.5f, 0.5f, 1.0f)
//  , mItemHighLightColor(1.0f)
//{}
//
//void CUIMenuView::Render(const CUIFont & font, 
//                         const CUIText & text, 
//                         const CUIMenuLayout& layout,
//                         const CUIMenu & menu) const
//{
//  CUITextContext ctx;
//
//  text.Bind();
//
//  //ctx.Scale = layout.TitleScale;
//  //text.Render(font, layout.TitlePos * text.GetCharSize(), menu.GetTitle(), ctx);
//
//  //ctx.Scale = glm::vec2(1.0f);
//  //for(CUIMenu::const_iterator it = menu.Begin(); it != menu.End(); it++) {
//  //  ctx.Color = glm::mix(mItemHighLightColor, mItemColor, it->HLValue);
//  //  text.Render(font, 
//  //              it->Pos * text.GetCharSize(), 
//  //              it->Text, 
//  //              ctx);
//  //}
//
//  text.UnBind();
//}
