#ifndef __BITRACE_UIMENU_H__
#define __BITRACE_UIMENU_H__

#include <SDL_types.h>
#include <CBStr/Defines.h>
#include <vector>

class CUIMenu {
public:
  class CItem {
  public:
    Uint32  Id;
    cb::string Text;
  };

private:
  typedef std::vector<CItem> ItemVectorT;

  cb::string  mTitle;
  ItemVectorT mItems;

public:
  CUIMenu(const cb::string& title);
  ~CUIMenu();

  void AddItem(const Uint32 id, const cb::string& text);
};

#endif // !__BITRACE_UIMENU_H__

