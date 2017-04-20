#ifndef __BITRACE_UIMENU_H__
#define __BITRACE_UIMENU_H__

#include <SDL_types.h>
#include <CBStr/Defines.h>
#include <vector>

class CUIMenuLayout {
public:
  glm::vec2 ItemsPos;
  glm::vec2 ItemPadding;
  glm::vec2 TitleScale;
  glm::vec2 TitlePos;

  CUIMenuLayout();
};

class CUIFont;
class CUIText;

class CUIMenu {
public:
  enum ITEMFLAG {
    IF_HIDDEN = 0x1,
    IF_HIGHLIGHT = 0x2,
    IF_FOCUS = 0x4
  };

  class CItem {
  public:
    Uint32  Id;
    cb::string Text;
    glm::vec2 Pos;
    glm::vec2 Size;
    Uint32 Flags;
    float HLValue;

    CItem(const Uint32 id, const cb::string& text);
    CItem(const CItem& other);
  };

  typedef std::vector<CItem> ItemVectorT;
  typedef ItemVectorT::iterator iterator;
  typedef ItemVectorT::const_iterator const_iterator;

private:
  cb::string  mTitle;
  ItemVectorT mItems;

public:
  CUIMenu(const cb::string& title);
  ~CUIMenu();

  const cb::string& GetTitle() const;

  void CalcItemsPosSize(const CUIFont& font, const CUIMenuLayout& layout);

  iterator AddItem(const Uint32 id, const cb::string& text);
  iterator FindItem(const Uint32 id);
  const_iterator FindItem(const Uint32 id) const;
  const CItem& GetItem(const Uint32 index) const;
  iterator Erase(const_iterator it);
  void Clear();

  const Uint32 GetSize() const;

  iterator Begin();
  const_iterator Begin() const;
  iterator End();
  const_iterator End() const;

};

class CUIMenuView {
private:
  glm::vec2 mScreenSize;
  glm::vec4 mItemColor;
  glm::vec4 mItemHighLightColor;

public:
  CUIMenuView(const glm::vec2& screenSize);

  void Render(const CUIFont& font, 
              const CUIText& text, 
              const CUIMenuLayout& layout,
              const CUIMenu& menu) const;
};

#endif // !__BITRACE_UIMENU_H__

