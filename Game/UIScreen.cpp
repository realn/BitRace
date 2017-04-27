#include "stdafx.h"
#include "UIScreen.h"
#include "UIFont.h"
#include "UIBrush.h"

IUIItem::IUIItem() {}

IUIItem::~IUIItem() {}


CUIScreen::CUIScreen(const glm::vec2& size, const glm::vec2& margin)
  : mSize(size)
  , mMargin(margin)
  , mItem(nullptr) {}

CUIScreen::~CUIScreen() {
  delete mItem;
}

void CUIScreen::SetItem(IUIItem * pItem) {
  mItem = pItem;
  UpdateItemLists();
}

IUIItem * CUIScreen::GetItemById(const cb::string & id) const {
  UIItemIdMapT::const_iterator it = mItemIdMap.find(id);
  if(it == mItemIdMap.end())
    return nullptr;
  return it->second;
}

void CUIScreen::UpdateRender(const CUIFont& font) {
  CUIBrush brush(mSize, &font);

  if(mItem) {
    mItem->UpdateRender(mSize - mMargin * 2.0f, brush);
  }
}

void CUIScreen::Render(const CUIFont & font) const {
  if(mItem == nullptr) {
    return;
  }

  CUIBrush brush(mSize, &font);

  brush.Bind();

  mItem->Render(
    mMargin,
    brush);

  brush.UnBind();
}

void CUIScreen::UpdateItemLists() {
  mAllItems.clear();
  mItemIdMap.clear();

  if(mItem == nullptr)
    return;

  mAllItems.push_back(mItem);
  mItem->GetAllSubItems(mAllItems);

  for(UIItemVectorT::iterator it = mAllItems.begin(); it != mAllItems.end(); it++) {
    cb::string id = (*it)->GetId();
    if(id.empty()) {
      continue;
    }

    mItemIdMap[id] = *it;
  }
}

