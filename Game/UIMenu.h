#ifndef __BITRACE_UIMENU_H__
#define __BITRACE_UIMENU_H__

#include <SDL_types.h>
#include <CBStr/Defines.h>
#include <vector>

class CUIMenu {
public:
  enum ITEMFLAT {
    IF_HIDDEN = 0x1,
    IF_HIGHLIGHT = 0x2,
    IF_FOCUS = 0x4
  };

  class CItem {
  public:
    Uint32  Id;
    cb::string Text;
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

  iterator AddItem(const Uint32 id, const cb::string& text);
  iterator GetItem(const Uint32 id);
  const_iterator GetItem(const Uint32 id) const;
  iterator Erase(const_iterator it);
  void Clear();

  iterator Begin();
  const_iterator Begin() const;
  iterator End();
  const_iterator End() const;
};

class CUIFont;
class CUIText;

class CUIMenuView {
private:
  glm::vec2 mSize;
  glm::vec2 mMargin;
  glm::vec4 mItemColor;
  glm::vec4 mItemHighLightColor;

public:
  CUIMenuView(const glm::vec2& size);

  void Render(const CUIFont& font, CUIText& text, const CUIMenu& menu);
};

#endif // !__BITRACE_UIMENU_H__

