#ifndef __BITRACE_UISCREENXML_H__
#define __BITRACE_UISCREENXML_H__

#include <SDL_types.h>
#include <CBXml/Serialize.h>

enum class UIHAlign;
enum class UIVAlign;
enum class UISizePolicy;
enum class UIOrientation;

class CUIScreen;
class IUIItem;
class CUIText;
template<typename _Type> class CUITextNumber;
class CUIRect;
class CUIPanel;
class CUIStack;
class CUIItemList;
class CUIProgressBar;

namespace cb {
  extern const string toStr(const UIHAlign value);
  extern const string toStr(const UIVAlign value);
  extern const string toStr(const UISizePolicy value);
  extern const string toStr(const UIOrientation value);

  extern const bool fromStr(const string& text, UIHAlign& outVal);
  extern const bool fromStr(const string& text, UIVAlign& outVal);
  extern const bool fromStr(const string& text, UISizePolicy& outVal);
  extern const bool fromStr(const string& text, UIOrientation& outVal);
}

CB_DEFINEXMLREAD(CUIScreen);
CB_DEFINEXMLREAD(CUIText);
CB_DEFINEXMLREAD(CUITextNumber<Sint32>);
CB_DEFINEXMLREAD(CUITextNumber<float>);
CB_DEFINEXMLREAD(CUIRect);
CB_DEFINEXMLREAD(CUIPanel);
CB_DEFINEXMLREAD(CUIStack);
CB_DEFINEXMLREAD(CUIItemList);
CB_DEFINEXMLREAD(CUIProgressBar);

#endif // !__BITRACE_UISCREENXML_H__

