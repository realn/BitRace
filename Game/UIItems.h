#ifndef __BITRACE_UIITEMS_H__
#define __BITRACE_UIITEMS_H__

#include <SDL_types.h>
#include <glm/fwd.hpp>

#include "UIScreen.h"

enum class UIOrientation {
  Horizontal = 0,
  Vertical = 1,
};

enum class UIHAlign {
  Left = 0,
  Center = 1,
  Right = 2,
};

enum class UIVAlign {
  Top = 0,
  Middle = 1,
  Bottom = 2,
};

enum class UISizePolicy {
  Fixed = 0,
  FillOwner = 1,
};

class CUIItemBase
  : public IUIItem {
protected:
  cb::string mId;
  glm::vec2 mSize;
  glm::vec2 mMargin;
  bool mVisible;

public:
  CUIItemBase(const glm::vec2& size, const glm::vec2& margin);
  virtual ~CUIItemBase();

  virtual void SetId(const cb::string & id) override;
  virtual void SetMargin(const glm::vec2& margin) override;
  virtual void SetSize(const glm::vec2& size) override;
  virtual void SetVisible(const bool value) override;

  virtual const cb::string & GetId() const override;
  const glm::vec2& GetMargin() const override;
  const glm::vec2& GetSize() const override;
  virtual const bool IsVisible() const override;

  const glm::vec2 GetClientSize() const override;
  const glm::vec2 GetClientPos(const glm::vec2& pos) const;

  virtual void GetAllSubItems(UIItemVectorT & items) override;
};

class CUIText :
  public CUIItemBase {
private:
  cb::string mText;
  glm::vec4 mColor;
  glm::vec2 mScale;
  bool mAutoSize;

public:
  CUIText(const cb::string& text = cb::string(),
          const glm::vec4& color = glm::vec4(1.0f),
          const glm::vec2& margin = glm::vec2(0.0f));
  virtual ~CUIText();

  void SetText(const cb::string& text);
  void SetColor(const glm::vec4& color);
  void SetScale(const glm::vec2& scale);
  void SetAutoSize(const bool value);

  const cb::string& GetText() const;
  const glm::vec4& GetColor() const;
  const glm::vec2& GetScale() const;
  const bool IsAutoSize() const;

  // Inherited via IUIItem
  virtual void UpdateRender(const glm::vec2& size,
                            const CUIBrush& brush) override;
  virtual void Render(const glm::vec2 & pos,
                      const CUIBrush & brush) const override;

};

template<typename _Type>
class CUITextNumber
  : public CUIText
{
private:
  cb::string mFormat;
  _Type mValue;

public:
  CUITextNumber(const cb::string& format = L"{0}",
                const glm::vec4& color = glm::vec4(1.0f),
                const glm::vec2& margin = glm::vec2(0.0f))
    : CUIText(cb::string(), color, margin)
    , mFormat(format)
    , mValue(_Type(0)) {}
  virtual ~CUITextNumber() {}

  void SetFormat(const cb::string& format) { mFormat = format; }
  void SetValue(const _Type& value) { mValue = value; }

  const cb::string& GetFormat() const { return mFormat; }
  const _Type GetValue() const { return mValue; }

  virtual void UpdateRender(const glm::vec2& size,
                            const CUIBrush& brush) override {
    SetText(cb::format(mFormat, mValue));
    CUIText::UpdateRender(size, brush);
  }
};
typedef CUITextNumber<Sint32> CUITextInt;
typedef CUITextNumber<float> CUITextFloat;

class CUIRect
  : public CUIItemBase {
private:
  glm::vec4 mColor;
  UISizePolicy mSizeHPolicy;
  UISizePolicy mSizeVPolicy;
  bool mFillRect;

public:
  CUIRect(const glm::vec4& color = glm::vec4(1.0f),
          const glm::vec2& size = glm::vec2(0.0f),
          const glm::vec2& margin = glm::vec2(0.0f));
  virtual ~CUIRect();

  virtual void SetColor(const glm::vec4& color);
  virtual void SetSize(const glm::vec2& size) override;
  void SetFillRect(const bool value);

  const glm::vec4& GetColor() const;
  const bool GetFillRect() const;

  // Inherited via IUIItem
  virtual void UpdateRender(const glm::vec2& size,
                            const CUIBrush& brush) override;
  virtual void Render(const glm::vec2 & pos,
                      const CUIBrush & brush) const override;
};

class CUIPanel :
  public CUIRect {
private:
  IUIItem* mItem;
  UIHAlign mHAlign;
  UIVAlign mVAlign;

public:
  CUIPanel(IUIItem* pItem = nullptr,
           const glm::vec4& color = glm::vec4(0.0f),
           const glm::vec2& size = glm::vec2(0.0f),
           const glm::vec2& margin = glm::vec2(0.0f));
  virtual ~CUIPanel();

  void SetItemAlign(const UIHAlign halign, const UIVAlign valign);
  void SetItem(IUIItem* pItem);

  IUIItem* GetItem() const;
  const UIHAlign GetHAlign() const;
  const UIVAlign GetVAlign() const;

  // Inherited via CUIRect 
  virtual void UpdateRender(const glm::vec2& size,
                            const CUIBrush& brush) override;
  virtual void Render(const glm::vec2 & pos,
                      const CUIBrush & brush) const override;

  virtual void GetAllSubItems(UIItemVectorT & items) override;
};

class CUIStack :
  public CUIRect {
private:
  UIOrientation mOrientation;
  UIHAlign mHAlign;
  UIVAlign mVAlign;
  UIItemVectorT mItems;
  glm::vec2 mItemsSize;

public:
  CUIStack(const UIOrientation orientation = UIOrientation::Horizontal,
           const UIHAlign halign = UIHAlign::Left,
           const UIVAlign valign = UIVAlign::Top,
           const glm::vec4& color = glm::vec4(0.0f),
           const glm::vec2& size = glm::vec2(0.0f),
           const glm::vec2& margin = glm::vec2(0.0f));
  virtual ~CUIStack();

  void SetOrientation(const UIOrientation value);
  void SetItemsAlign(const UIHAlign halign, const UIVAlign valign);

  const UIOrientation GetOrientation() const;
  const UIHAlign GetHAlign() const;
  const UIVAlign GetVAlign() const;

  void AddItem(IUIItem* pItem);
  void RemoveItem(IUIItem* pItem);

  // Inherited via CUIItemBase
  virtual void UpdateRender(const glm::vec2 & size, const CUIBrush & brush) override;
  virtual void Render(const glm::vec2 & pos, const CUIBrush & brush) const override;

  virtual void GetAllSubItems(UIItemVectorT & items) override;

private:
  const bool IsHorVAlign(const UIHAlign halign, const UIVAlign valign) const;
  const bool IsHorVAlignRev(const UIHAlign halign, const UIVAlign valign) const;
};

class CUIItemList
  : public CUIRect {
private:
  class CItemData {
  public:
    IUIItem* Item;
    UIHAlign HAlign;
    UIVAlign VAlign;
  };
  typedef std::vector<CItemData> ItemDataVectorT;

  ItemDataVectorT mItems;

public:
  CUIItemList(const glm::vec4& color = glm::vec4(0.0f),
              const glm::vec2& size = glm::vec2(0.0f),
              const glm::vec2& margin = glm::vec2(0.0f));
  virtual ~CUIItemList();

  void AddItem(IUIItem* pItem,
               const UIHAlign halign = UIHAlign::Center,
               const UIVAlign valign = UIVAlign::Middle);
  void RemoveItem(IUIItem* pItem);

  virtual void UpdateRender(const glm::vec2& size,
                            const CUIBrush& brush) override;
  virtual void Render(const glm::vec2& pos,
                      const CUIBrush& brush) const override;

  virtual void GetAllSubItems(UIItemVectorT & items) override;
};

class CUIProgressBar
  : public CUIRect {
private:
  CUIRect mBar;
  float mMinValue;
  float mMaxValue;
  float mValue;

public:
  CUIProgressBar(const glm::vec4& barColor = glm::vec4(1.0f),
                 const glm::vec4& lineColro = glm::vec4(0.0f),
                 const glm::vec2& size = glm::vec2(0.0f),
                 const glm::vec2& margin = glm::vec2(0.0f));
  virtual ~CUIProgressBar();

  void SetBarColor(const glm::vec4& color);
  void SetRange(const float minValue, const float maxValue);
  void SetValue(const float value);

  const glm::vec4& GetBarColor() const;
  const float GetMaxValue() const;
  const float GetMinValue() const;
  const float GetValue() const;

  virtual void UpdateRender(const glm::vec2& size,
                            const CUIBrush& brush) override;
  virtual void Render(const glm::vec2& pos,
                      const CUIBrush& brush) const override;


};


#endif // !__BITRACE_UIITEMS_H__

