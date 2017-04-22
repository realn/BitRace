#ifndef __BITRACE_UISCREEN_H__
#define __BITRACE_UISCREEN_H__

#include <glm/fwd.hpp>

class CUIText;
class CUIFont;

class CUIScreen {
public:
  CUIScreen();

  void Render(const CUIFont& font, const CUIText& text) const;
};

#endif // !__BITRACE_UISCREEN_H__

