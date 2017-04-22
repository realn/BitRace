#include "stdafx.h"
#include "UIScreen.h"
#include "UIFont.h"

CUIScreen::CUIScreen() {}

void CUIScreen::Render(const CUIFont & font, const CUIText & text) const {
  text.Bind();



  text.UnBind();
}
