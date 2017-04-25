#ifndef __BITRACE_UISCREEN_H__
#define __BITRACE_UISCREEN_H__

#include <glm/fwd.hpp>
#include <SDL_types.h>
#include <CBStr/StringEx.h>

class CUIFont;
class CUIBrush;

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

class IUIItem {
protected:
  IUIItem();

public:
  virtual ~IUIItem();

  virtual void SetMargin(const glm::vec2& margin) = 0;
  virtual void SetSize(const glm::vec2& size) = 0;

  virtual const glm::vec2& GetMargin() const = 0;
  virtual const glm::vec2& GetSize() const = 0;
  virtual const glm::vec2 GetClientSize() const = 0;

  virtual void UpdateRender(const glm::vec2& size,
                            const CUIBrush& brush) = 0;
  virtual void Render(const glm::vec2& pos,
                      const CUIBrush& brush) const = 0;
};
typedef std::vector<IUIItem*> UIItemVectorT;

class CUIItemBase
  : public IUIItem {
protected:
  glm::vec2 mSize;
  glm::vec2 mMargin;

public:
  CUIItemBase(const glm::vec2& size, const glm::vec2& margin);
  virtual ~CUIItemBase();

  virtual void SetMargin(const glm::vec2& margin) override;
  virtual void SetSize(const glm::vec2& size) override;

  const glm::vec2& GetMargin() const override;
  const glm::vec2& GetSize() const override;
  const glm::vec2 GetClientSize() const override;

  const glm::vec2 GetClientPos(const glm::vec2& pos) const;
};

class CUIScreen {
private:
  glm::vec2 mSize;
  glm::vec2 mMargin;
  IUIItem* mItem;

public:
  CUIScreen(const glm::vec2& size, const glm::vec2& margin = glm::vec2(0.0f));
  ~CUIScreen();

  void SetItem(IUIItem* pItem);

  void UpdateRender(const CUIFont& font);
  void Render(const CUIFont& font) const;
};

class CUIText :
  public CUIItemBase {
private:
  cb::string mText;
  glm::vec4 mColor;

public:
  CUIText(const cb::string& text,
          const glm::vec4& color = glm::vec4(1.0f),
          const glm::vec2& margin = glm::vec2(0.0f));
  virtual ~CUIText();

  void SetText(const cb::string& text);

  const cb::string& GetText() const;

  // Inherited via IUIItem
  virtual void UpdateRender(const glm::vec2& size,
                            const CUIBrush& brush) override;
  virtual void Render(const glm::vec2 & pos,
                      const CUIBrush & brush) const override;

};

class CUIRect
  : public CUIItemBase {
private:
  glm::vec4 mColor;
  UISizePolicy mSizeHPolicy;
  UISizePolicy mSizeVPolicy;

public:
  CUIRect(const glm::vec4& color,
          const glm::vec2& size = glm::vec2(0.0f),
          const glm::vec2& margin = glm::vec2(0.0f));
  virtual ~CUIRect();

  virtual void SetColor(const glm::vec4& color);
  virtual void SetSize(const glm::vec2& size) override;

  const glm::vec4& GetColor() const;

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

  void AddItem(IUIItem* pItem);
  void RemoveItem(IUIItem* pItem);

  // Inherited via CUIItemBase
  virtual void UpdateRender(const glm::vec2 & size, const CUIBrush & brush) override;
  virtual void Render(const glm::vec2 & pos, const CUIBrush & brush) const override;

private:
  const bool IsHorVAlign(const UIHAlign halign, const UIVAlign valign) const;
  const bool IsHorVAlignRev(const UIHAlign halign, const UIVAlign valign) const;
};



#endif // !__BITRACE_UISCREEN_H__

