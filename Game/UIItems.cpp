#include "stdafx.h"
#include "UIItems.h"
#include "UIBrush.h"

const glm::vec2 GetAlignMulVal(const UIHAlign halign, const UIVAlign valign) {
  glm::vec2 result;
  switch(halign) {
  case UIHAlign::Left:    result.x = 0.0f; break;
  case UIHAlign::Center:  result.x = 0.5f; break;
  case UIHAlign::Right:   result.x = 1.0f; break;
  default:
    break;
  }
  switch(valign) {
  case UIVAlign::Top:     result.y = 0.0f; break;
  case UIVAlign::Middle:  result.y = 0.5f; break;
  case UIVAlign::Bottom:  result.y = 1.0f; break;
  default:
    break;
  }
  return result;
}


CUIItemBase::CUIItemBase(const glm::vec2 & size, const glm::vec2& margin)
  : mSize(size)
  , mMargin(margin)
  , mVisible(true) {}

CUIItemBase::~CUIItemBase() {}

void CUIItemBase::SetId(const cb::string & id) {
  mId = id;
}

void CUIItemBase::SetMargin(const glm::vec2 & margin) {
  mMargin = margin;
}

void CUIItemBase::SetSize(const glm::vec2 & size) {
  mSize = size;
}

void CUIItemBase::SetVisible(const bool value) {
  mVisible = value;
}

const cb::string & CUIItemBase::GetId() const {
  return mId;
}

const glm::vec2 & CUIItemBase::GetMargin() const {
  return mMargin;
}

const glm::vec2 & CUIItemBase::GetSize() const {
  return mSize;
}

const bool CUIItemBase::IsVisible() const {
  return mVisible;
}

const glm::vec2 CUIItemBase::GetClientSize() const {
  return mSize - mMargin * 2.0f;
}

const glm::vec2 CUIItemBase::GetClientPos(const glm::vec2 & pos) const {
  return pos + mMargin;
}

void CUIItemBase::GetAllSubItems(UIItemVectorT & items) {}


CUIText::CUIText(const cb::string & text,
                 const glm::vec4& color,
                 const glm::vec2& margin)
  : CUIItemBase(glm::vec2(0.0f), margin)
  , mText(text)
  , mColor(color) 
  , mAutoSize(true)
{}

CUIText::~CUIText() {}

void CUIText::SetText(const cb::string & text) {
  mText = text;
}

void CUIText::SetColor(const glm::vec4 & color) {
  mColor = color;
}

void CUIText::SetAutoSize(const bool value) {}

const cb::string & CUIText::GetText() const {
  return mText;
}

const glm::vec4 & CUIText::GetColor() const {
  return mColor;
}

const bool CUIText::IsAutoSize() const {
  return false;
}

void CUIText::UpdateRender(const glm::vec2& size, const CUIBrush& brush) {
  if(mAutoSize) {
    glm::vec2 textSize = brush.GetTextSize(mText);
    mSize = glm::min(size, textSize + mMargin * 2.0f);
  }
}

void CUIText::Render(const glm::vec2 & pos,
                     const CUIBrush & brush) const {
  if(!mVisible)
    return;

  CUIBrush localBrush(brush);

  localBrush.SetColor(mColor);
  localBrush.RenderText(GetClientPos(pos), mText);
}

CUIRect::CUIRect(const glm::vec4 & color,
                 const glm::vec2& size,
                 const glm::vec2& margin)
  : CUIItemBase(size, margin)
  , mColor(color)
  , mSizeHPolicy(UISizePolicy::Fixed)
  , mSizeVPolicy(UISizePolicy::Fixed)
  , mFillRect(true) {
  if(mSize.x == 0.0f) {
    mSizeHPolicy = UISizePolicy::FillOwner;
  }
  if(mSize.y == 0.0f) {
    mSizeVPolicy = UISizePolicy::FillOwner;
  }
}

CUIRect::~CUIRect() {}

void CUIRect::SetColor(const glm::vec4 & color) {
  mColor = color;
}

void CUIRect::SetSize(const glm::vec2 & size) {
  mSize = size;
  mSizeHPolicy = mSize.x == 0.0f ? UISizePolicy::FillOwner : UISizePolicy::Fixed;
  mSizeVPolicy = mSize.y == 0.0f ? UISizePolicy::FillOwner : UISizePolicy::Fixed;
}

void CUIRect::SetFillRect(const bool value) {
  mFillRect = value;
}

const glm::vec4 & CUIRect::GetColor() const {
  return mColor;
}

const bool CUIRect::GetFillRect() const {
  return mFillRect;
}

void CUIRect::UpdateRender(const glm::vec2& size, const CUIBrush & brush) {
  if(mSizeHPolicy == UISizePolicy::FillOwner) {
    mSize.x = size.x;
  }
  if(mSizeVPolicy == UISizePolicy::FillOwner) {
    mSize.y = size.y;
  }
}

void CUIRect::Render(const glm::vec2 & pos,
                     const CUIBrush & brush) const {
  if(!mVisible)
    return;

  CUIBrush localBrush(brush);
  localBrush.SetColor(mColor);

  if(mFillRect) {
    localBrush.RenderQuad(pos, mSize);
  }
  else {
    localBrush.RenderQuadLines(pos, mSize);
  }
}

CUIPanel::CUIPanel(IUIItem * pItem,
                   const glm::vec4 & color,
                   const glm::vec2 & size,
                   const glm::vec2 & margin)
  : CUIRect(color, size, margin)
  , mItem(pItem)
  , mHAlign(UIHAlign::Center)
  , mVAlign(UIVAlign::Middle) {}

CUIPanel::~CUIPanel() {
  delete mItem;
}

void CUIPanel::SetItemAlign(const UIHAlign halign, const UIVAlign valign) {
  mHAlign = halign;
  mVAlign = valign;
}

void CUIPanel::SetItem(IUIItem * pItem) {
  mItem = pItem;
}

IUIItem * CUIPanel::GetItem() const {
  return mItem;
}

const UIHAlign CUIPanel::GetHAlign() const {
  return mHAlign;
}

const UIVAlign CUIPanel::GetVAlign() const {
  return mVAlign;
}

void CUIPanel::UpdateRender(const glm::vec2 & size, const CUIBrush & brush) {
  CUIRect::UpdateRender(size, brush);

  if(mItem) {
    mItem->UpdateRender(GetClientSize(), brush);
  }
}

void CUIPanel::Render(const glm::vec2 & pos, const CUIBrush & brush) const {
  if(!mVisible)
    return;

  CUIRect::Render(pos, brush);

  if(mItem) {
    glm::vec2 clientPos = GetClientPos(pos);
    glm::vec2 clientSize = GetClientSize();
    glm::vec2 alignVal = GetAlignMulVal(mHAlign, mVAlign);

    mItem->Render(clientPos + alignVal * (clientSize - mItem->GetSize()), brush);
  }
}

void CUIPanel::GetAllSubItems(UIItemVectorT & items) {
  if(mItem) {
    items.push_back(mItem);
    mItem->GetAllSubItems(items);
  }
}



CUIStack::CUIStack(const UIOrientation orientation,
                   const UIHAlign halign,
                   const UIVAlign valign,
                   const glm::vec4& color,
                   const glm::vec2& size,
                   const glm::vec2& margin)
  : CUIRect(color, size, margin)
  , mOrientation(orientation)
  , mHAlign(halign)
  , mVAlign(valign) {}

CUIStack::~CUIStack() {
  for(UIItemVectorT::iterator it = mItems.begin(); it != mItems.end(); it++) {
    delete *it;
  }
}

void CUIStack::SetOrientation(const UIOrientation value) {
  mOrientation = value;
}

void CUIStack::SetItemsAlign(const UIHAlign halign, const UIVAlign valign) {
  mHAlign = halign;
  mVAlign = valign;
}

const UIOrientation CUIStack::GetOrientation() const {
  return mOrientation;
}

const UIHAlign CUIStack::GetHAlign() const {
  return mHAlign;
}

const UIVAlign CUIStack::GetVAlign() const {
  return mVAlign;
}

void CUIStack::AddItem(IUIItem * pItem) {
  if(std::find(mItems.begin(), mItems.end(), pItem) != mItems.end()) {
    return;
  }
  mItems.push_back(pItem);
}

void CUIStack::RemoveItem(IUIItem * pItem) {
  UIItemVectorT::iterator it = std::find(mItems.begin(), mItems.end(), pItem);
  if(it == mItems.end()) {
    return;
  }

  mItems.erase(it);
}

void CUIStack::UpdateRender(const glm::vec2 & size, const CUIBrush & brush) {
  CUIRect::UpdateRender(size, brush);
  glm::vec2 ori(
    mOrientation == UIOrientation::Horizontal ? 1.0f : 0.0f,
    mOrientation == UIOrientation::Vertical ? 1.0f : 0.0f
  );

  glm::vec2 locSize = GetClientSize();
  mItemsSize = glm::vec2(0.0f);
  for(UIItemVectorT::iterator it = mItems.begin(); it != mItems.end(); it++) {
    IUIItem* pItem = (*it);

    pItem->UpdateRender(locSize, brush);
    glm::vec2 newSize = locSize - ori * pItem->GetSize();
    locSize.x = glm::max(newSize.x, 0.0f);
    locSize.y = glm::max(newSize.y, 0.0f);

    mItemsSize += pItem->GetSize();
  }
}

void CUIStack::Render(const glm::vec2 & pos, const CUIBrush & brush) const {
  if(!mVisible)
    return;

  CUIRect::Render(pos, brush);
  glm::vec2 clientPos = GetClientPos(pos);
  glm::vec2 clientSize = GetClientSize();

  glm::vec2 locPos = clientPos;
  glm::vec2 ori(
    mOrientation == UIOrientation::Horizontal ? 1.0f : 0.0f,
    mOrientation == UIOrientation::Vertical ? 1.0f : 0.0f
  );
  glm::vec2 oriRev = 1.0f - ori;
  if(IsHorVAlign(UIHAlign::Center, UIVAlign::Middle)) {
    locPos = glm::mix(locPos, locPos + ((clientSize - mItemsSize) / 2.0f), ori);
  }
  if(IsHorVAlign(UIHAlign::Right, UIVAlign::Bottom)) {
    locPos = glm::mix(locPos, locPos + clientSize - mItemsSize, ori);
  }

  for(UIItemVectorT::const_iterator it = mItems.begin();
      it != mItems.end(); it++) {
    const IUIItem* pItem = (*it);

    if(IsHorVAlignRev(UIHAlign::Center, UIVAlign::Middle)) {
      locPos = glm::mix(locPos, clientPos + (clientSize - pItem->GetSize()) / 2.0f, 1.0f - ori);
    }
    if(IsHorVAlignRev(UIHAlign::Right, UIVAlign::Bottom)) {
      locPos = glm::mix(locPos, clientPos + clientSize - pItem->GetSize(), 1.0f - ori);
    }

    pItem->Render(locPos, brush);

    locPos = glm::mix(locPos, locPos + pItem->GetSize(), ori);
  }
}

void CUIStack::GetAllSubItems(UIItemVectorT & items) {
  for(UIItemVectorT::iterator it = mItems.begin(); it != mItems.end(); it++) {
    items.push_back(*it);
    (*it)->GetAllSubItems(items);
  }
}

const bool CUIStack::IsHorVAlign(const UIHAlign halign, const UIVAlign valign) const {
  if(mOrientation == UIOrientation::Horizontal && mHAlign == halign) {
    return true;
  }
  if(mOrientation == UIOrientation::Vertical && mVAlign == valign) {
    return true;
  }
  return false;
}

const bool CUIStack::IsHorVAlignRev(const UIHAlign halign, const UIVAlign valign) const {
  if(mOrientation == UIOrientation::Vertical && mHAlign == halign) {
    return true;
  }
  if(mOrientation == UIOrientation::Horizontal && mVAlign == valign) {
    return true;
  }
  return false;
}

CUIItemList::CUIItemList(const glm::vec4 & color,
                         const glm::vec2 & size,
                         const glm::vec2 & margin)
  : CUIRect(color, size, margin) {}

CUIItemList::~CUIItemList() {
  for(ItemDataVectorT::iterator it = mItems.begin(); it != mItems.end(); it++) {
    delete it->Item;
  }
}

void CUIItemList::AddItem(IUIItem * pItem, const UIHAlign halign, const UIVAlign valign) {
  CItemData data;
  data.Item = pItem;
  data.HAlign = halign;
  data.VAlign = valign;
  mItems.push_back(data);
}

void CUIItemList::RemoveItem(IUIItem * pItem) {
  ItemDataVectorT::iterator it = mItems.begin();
  while(it != mItems.end()) {
    if(it->Item == pItem) {
      it = mItems.erase(it);
    }
    else {
      it++;
    }
  }
}

void CUIItemList::UpdateRender(const glm::vec2 & size, const CUIBrush & brush) {
  CUIRect::UpdateRender(size, brush);

  for(ItemDataVectorT::iterator it = mItems.begin(); it != mItems.end(); it++) {
    it->Item->UpdateRender(GetClientSize(), brush);
  }
}

void CUIItemList::Render(const glm::vec2 & pos, const CUIBrush & brush) const {
  if(!mVisible)
    return;

  CUIRect::Render(pos, brush);

  glm::vec2 clientPos = GetClientPos(pos);
  glm::vec2 clientSize = GetClientSize();

  for(ItemDataVectorT::const_iterator it = mItems.begin(); it != mItems.end(); it++) {
    glm::vec2 alignVal = GetAlignMulVal(it->HAlign, it->VAlign);
    glm::vec2 itemPos = clientPos + (clientSize - it->Item->GetSize()) * alignVal;

    it->Item->Render(itemPos, brush);
  }
}

void CUIItemList::GetAllSubItems(UIItemVectorT & items) {
  for(ItemDataVectorT::iterator it = mItems.begin(); it != mItems.end(); it++) {
    items.push_back(it->Item);
    it->Item->GetAllSubItems(items);
  }
}

CUIProgressBar::CUIProgressBar(const glm::vec4 & barColor,
                               const glm::vec4 & lineColor,
                               const glm::vec2 & size,
                               const glm::vec2 & margin)
  : CUIRect(lineColor, size, margin)
  , mBar(barColor, size, margin)
  , mMaxValue(1.0f)
  , mMinValue(0.0f)
  , mValue(1.0f) {}

CUIProgressBar::~CUIProgressBar() {}

void CUIProgressBar::SetBarColor(const glm::vec4 & color) {
  mBar.SetColor(color);
}

void CUIProgressBar::SetRange(const float minValue, const float maxValue) {
  mMaxValue = maxValue;
  mMinValue = minValue;
}

void CUIProgressBar::SetValue(const float value) {
  mValue = glm::clamp(value, mMinValue, mMaxValue);
}

const glm::vec4 & CUIProgressBar::GetBarColor() const {
  return mBar.GetColor();
}

const float CUIProgressBar::GetMaxValue() const {
  return mMaxValue;
}

const float CUIProgressBar::GetMinValue() const {
  return mMinValue;
}

const float CUIProgressBar::GetValue() const {
  return mValue;
}

void CUIProgressBar::UpdateRender(const glm::vec2 & size,
                                  const CUIBrush & brush) {
  CUIRect::UpdateRender(size, brush);
  float val = (mValue - mMinValue) / (mMaxValue - mMinValue);
  glm::vec2 valVec(val, 1.0f);
  mBar.SetSize(GetClientSize() * valVec);
}

void CUIProgressBar::Render(const glm::vec2 & pos, const CUIBrush & brush) const {
  if(!mVisible)
    return;

  CUIRect::Render(pos, brush);
  mBar.Render(GetClientPos(pos), brush);
}
