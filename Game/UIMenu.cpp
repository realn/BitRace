#include "stdafx.h"
#include "UIMenu.h"
#include "UIFont.h"

CUIMenu::CItem::CItem(const Uint32 id, const cb::string & text) 
  : Id(id)
  , Text(text)
  , Flags(0)
  , HLValue(0.0f)
{}

CUIMenu::CItem::CItem(const CItem & other)
  : Id(other.Id)
  , Text(other.Text)
  , Flags(other.Flags)
  , HLValue(other.HLValue)
{}

CUIMenu::CUIMenu(const cb::string& title)
  : mTitle(title)
{}

CUIMenu::~CUIMenu() {}

const cb::string & CUIMenu::GetTitle() const {
  return mTitle;
}

CUIMenu::iterator CUIMenu::AddItem(const Uint32 id, const cb::string & text) {
  iterator it = GetItem(id);
  if(it != End()) {
    it->Text = text;
    return it;
  }

  CItem item(id, text);
  mItems.push_back(item);

  return GetItem(id);
}

CUIMenu::iterator CUIMenu::GetItem(const Uint32 id) {
  for(iterator it = Begin(); it != End(); it++) {
    if(it->Id == id)
      return it;
  }
  return End();
}

CUIMenu::const_iterator CUIMenu::GetItem(const Uint32 id) const {
  for(const_iterator it = Begin(); it != End(); it++) {
    if(it->Id == id)
      return it;
  }
  return End();
}

CUIMenu::iterator CUIMenu::Erase(const_iterator it) {
  return mItems.erase(it);
}

void CUIMenu::Clear() {
  mItems.clear();
}

const Uint32 CUIMenu::GetSize() const {
  return mItems.size();
}

CUIMenu::iterator CUIMenu::Begin() {
  return mItems.begin();
}

CUIMenu::const_iterator CUIMenu::Begin() const {
  return mItems.begin();
}

CUIMenu::iterator CUIMenu::End() {
  return mItems.end();
}

CUIMenu::const_iterator CUIMenu::End() const {
  return mItems.end();
}

const CUIMenu::CItem & CUIMenu::Get(const Uint32 index) const {
  return mItems[index];
}

CUIMenuView::CUIMenuView(const glm::vec2& screenSize) 
  : mScreenSize(screenSize)
  , mItemColor(0.5f, 0.5f, 0.5f, 1.0f)
  , mItemHighLightColor(1.0f)
{}

void CUIMenuView::Render(const CUIFont & font, 
                         const CUIText & text, 
                         const CUIMenuLayout& layout,
                         const CUIMenu & menu) const
{
  CUITextContext ctx;

  text.Bind();

  glm::vec2 pos, size;
  pos = layout.GetTitlePos(font, menu, size);

  ctx.Scale = layout.TitleScale;
  text.Render(font, pos * text.GetCharSize(), menu.GetTitle(), ctx);

  ctx.Scale = glm::vec2(1.0f);

  CUIMenuLayout::ItemVectorT items = layout.GetItems(font, menu);
  for(CUIMenuLayout::ItemVectorT::iterator it = items.begin(); it != items.end(); it++) {
    ctx.Color = glm::mix(mItemColor, mItemHighLightColor, it->pItem->HLValue);
    text.Render(font, it->Pos * text.GetCharSize(), it->pItem->Text, ctx);
  }

  text.UnBind();
}

const glm::vec2 CUIMenuLayout::GetTitlePos(const CUIFont& font, const CUIMenu & menu, glm::vec2& titleSize) const {
  titleSize = font.GetSize(menu.GetTitle()) * TitleScale;
  return Margin;
}

const CUIMenuLayout::ItemVectorT CUIMenuLayout::GetItems(const CUIFont & font, const CUIMenu & menu) const {
  glm::vec2 size;
  glm::vec2 pos = GetTitlePos(font, menu, size);
  pos += glm::vec2(0.0f, size.y);

  ItemVectorT result;
  for(CUIMenu::const_iterator it = menu.Begin(); it != menu.End(); it++) {
    size = font.GetSize(it->Text);

    pos += glm::vec2(0.0f, ItemPadding.y);

    CItem item;
    item.pItem = &(*it);
    item.Pos = pos;
    item.Size = size;

    pos += glm::vec2(0.0f, size.y);

    result.push_back(item);
  }

  return result;
}