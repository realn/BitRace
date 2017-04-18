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

CUIMenuView::CUIMenuView(const glm::vec2& size) 
  : mSize(size)
  , mMargin(20.0f, 20.0f)
  , mItemColor(0.5f, 0.5f, 0.5f, 1.0f)
  , mItemHighLightColor(1.0f)
{}

void CUIMenuView::Render(const CUIFont & font, 
                         CUIText & text, 
                         const CUIMenu & menu) 
{
  text.Bind(mSize);
  text.SetColor(glm::vec4(1.0f));

  glm::vec2 pos = mMargin;

  text.Render(font, pos, menu.GetTitle());

  pos += glm::vec2(0.0f, 20.0f);

  for(CUIMenu::const_iterator it = menu.Begin(); it != menu.End(); it++) {
    pos += glm::vec2(0.0f, 20.0f);

    glm::vec4 color = glm::mix(mItemColor, mItemHighLightColor, it->HLValue);
    text.SetColor(color);
    text.Render(font, pos, it->Text);
  }

  text.UnBind();
}

