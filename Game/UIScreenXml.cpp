#include "stdafx.h"
#include "UIScreenXml.h"
#include "UIScreen.h"
#include "UIItems.h"
#include "XmlTypes.h"

typedef std::map<cb::string, UIHAlign> UIHAlignNameMapT;
typedef std::map<cb::string, UIVAlign> UIValignNameMapT;
typedef std::map<cb::string, UISizePolicy> UISizePolicyNameMapT;
typedef std::map<cb::string, UIOrientation> UIOrientationNameMapT;

static const UIHAlignNameMapT gHAlignNameMap = {
  {L"Left", UIHAlign::Left},
  {L"Center", UIHAlign::Center},
  {L"Right", UIHAlign::Right}
};

static const UIValignNameMapT gVAlignNameMap = {
  {L"Top", UIVAlign::Top},
  {L"Middle", UIVAlign::Middle},
  {L"Bottom", UIVAlign::Bottom}
};

static const UISizePolicyNameMapT gSizePolicyNameMap = {
  {L"Fixed", UISizePolicy::Fixed},
  {L"FillOwner", UISizePolicy::FillOwner}
};

static const UIOrientationNameMapT gOrientationNameMap = {
  {L"Horizontal", UIOrientation::Horizontal},
  {L"Vertical", UIOrientation::Vertical}
};

template<typename _Type>
const cb::string FindInMap(typename const std::map<cb::string, _Type>& nameMap, const _Type& value) {
  for(typename std::map<cb::string, _Type>::const_iterator it = nameMap.begin();
      it != nameMap.end(); it++) {
    if(it->second == value) {
      return it->first;
    }
  }
  return cb::string();
}

template<typename _Type>
const bool FindInMap(typename const std::map<cb::string, _Type>& nameMap, const cb::string& name, _Type& outVal) {
  typename std::map<cb::string, _Type>::const_iterator it = nameMap.find(name);
  if(it != nameMap.end()) {
    outVal = it->second;
    return true;
  }
  return false;
}


namespace cb {
  const string toStr(const UIHAlign value) {
    return FindInMap(gHAlignNameMap, value);
  }

  const string toStr(const UIVAlign value) {
    return FindInMap(gVAlignNameMap, value);
  }

  const string toStr(const UISizePolicy value) {
    return FindInMap(gSizePolicyNameMap, value);
  }

  const string toStr(const UIOrientation value) {
    return FindInMap(gOrientationNameMap, value);
  }

  const bool fromStr(const string & text, UIHAlign & outVal) {
    return FindInMap(gHAlignNameMap, text, outVal);
  }

  const bool fromStr(const string & text, UIVAlign & outVal) {
    return FindInMap(gVAlignNameMap, text, outVal);
  }

  const bool fromStr(const string & text, UISizePolicy & outVal) {
    return FindInMap(gSizePolicyNameMap, text, outVal);
  }

  const bool fromStr(const string & text, UIOrientation & outVal) {
    return FindInMap(gOrientationNameMap, text, outVal);
  }
}

class IXmlUIItemFactory {
public:
  virtual IUIItem* CreateItem(cb::CXmlNode& node) const = 0;
};
typedef std::map<cb::string, const IXmlUIItemFactory*> XmlUIItemFactoryMapT;

template<typename _Type>
class CXmlUIItemFactory :
  public IXmlUIItemFactory {
public:
  IUIItem* CreateItem(cb::CXmlNode& node) const override {
    _Type* pItem = new _Type();
    if(cb::ReadXmlObject(node, *pItem)) {
      return pItem;
    }
    delete pItem;
    return nullptr;
  }
};

static const cb::string XMLUIITEMTYPE_TEXT = L"Text";
static const cb::string XMLUIITEMTYPE_TEXTINT = L"TextInt";
static const cb::string XMLUIITEMTYPE_TEXTFLOAT = L"TextFloat";
static const cb::string XMLUIITEMTYPE_RECT = L"Rect";
static const cb::string XMLUIITEMTYPE_PANEL = L"Panel";
static const cb::string XMLUIITEMTYPE_STACK = L"Stack";
static const cb::string XMLUIITEMTYPE_ITEMLIST = L"ItemList";
static const cb::string XMLUIITEMTYPE_PROGRESSBAR = L"ProgressBar";

static const CXmlUIItemFactory<CUIText> gXmlUIItemFactoryText;
static const CXmlUIItemFactory<CUITextNumber<Sint32>> gXmlUIItemFactoryTextInt;
static const CXmlUIItemFactory<CUITextNumber<float>> gXmlUIItemFactoryTextFloat;
static const CXmlUIItemFactory<CUIRect> gXmlUIItemFactoryRect;
static const CXmlUIItemFactory<CUIPanel> gXmlUIItemFactoryPanel;
static const CXmlUIItemFactory<CUIStack> gXmlUIItemFactoryStack;
static const CXmlUIItemFactory<CUIItemList> gXmlUIItemFactoryItemList;
static const CXmlUIItemFactory<CUIProgressBar> gXmlUIItemFactoryProgressBar;

IUIItem* CreateItemFromNode(cb::CXmlNode& node) {
  static const XmlUIItemFactoryMapT itemFactoryMap = {
    {XMLUIITEMTYPE_TEXT, &gXmlUIItemFactoryText},
    {XMLUIITEMTYPE_TEXTINT, &gXmlUIItemFactoryTextInt},
    {XMLUIITEMTYPE_TEXTFLOAT, &gXmlUIItemFactoryTextFloat},
    {XMLUIITEMTYPE_RECT, &gXmlUIItemFactoryRect},
    {XMLUIITEMTYPE_PANEL, &gXmlUIItemFactoryPanel},
    {XMLUIITEMTYPE_STACK, &gXmlUIItemFactoryStack},
    {XMLUIITEMTYPE_ITEMLIST, &gXmlUIItemFactoryItemList},
    {XMLUIITEMTYPE_PROGRESSBAR, &gXmlUIItemFactoryProgressBar},
  };

  XmlUIItemFactoryMapT::const_iterator it = itemFactoryMap.find(node.GetName());
  if(it == itemFactoryMap.end()) {
    return nullptr;
  }
  return it->second->CreateItem(node);
}

CB_DEFINEXMLREAD(CUIScreen) {
  if(mNode.Nodes.GetSize() != 1)
    return false;

  IUIItem* pItem = CreateItemFromNode(*mNode.Nodes.Begin());
  if(pItem) {
    mObject.SetItem(pItem);
  }
  return pItem != nullptr;
}


static const cb::string XML_UIITEMBASE_ID = L"Id";
static const cb::string XML_UIITEMBASE_SIZE = L"Size";
static const cb::string XML_UIITEMBASE_MARGIN = L"Margin";
static const cb::string XML_UIITEMBASE_VISIBLE = L"Visible";

CB_DEFINEXMLREAD(CUIItemBase) {
  cb::string id;
  glm::vec2 val2;
  bool visible;
  
  if(GetAttribute(XML_UIITEMBASE_ID, id)) {
    mObject.SetId(id);
  }
  if(GetAttribute(XML_UIITEMBASE_SIZE, val2)) {
    mObject.SetSize(val2);
  }
  if(GetAttribute(XML_UIITEMBASE_MARGIN, val2)) {
    mObject.SetMargin(val2);
  }
  if(GetAttribute(XML_UIITEMBASE_VISIBLE, visible)) {
    mObject.SetVisible(visible);
  }
  return true;
}

static const cb::string XML_UITEXT_TEXT = L"Text";
static const cb::string XML_UITEXT_COLOR = L"Color";
static const cb::string XML_UITEXT_AUTOSIZE = L"AutoSize";

CB_DEFINEXMLREAD(CUIText) {
  if(!cb::ReadXmlObject(mNode, (CUIItemBase&)mObject)) {
    return false;
  }
  cb::string text;
  glm::vec4 color;
  bool autosize;
  if(GetAttribute(XML_UITEXT_TEXT, text)) {
    mObject.SetText(text);
  }
  if(GetAttribute(XML_UITEXT_COLOR, color)) {
    mObject.SetColor(color);
  }
  if(GetAttribute(XML_UITEXT_AUTOSIZE, autosize)) {
    mObject.SetAutoSize(autosize);
  }
  return true;
}

static const cb::string XML_UITEXTNUMBER_FORMAT = L"Format";
static const cb::string XML_UITEXTNUMBER_VALUE = L"Value";

CB_DEFINEXMLREAD(CUITextNumber<Sint32>) {
  if(!cb::ReadXmlObject(mNode, (CUIText&)mObject)) {
    return false;
  }
  cb::string format;
  Sint32 value;
  if(GetAttribute(XML_UITEXTNUMBER_FORMAT, format)) {
    mObject.SetFormat(format);
  }
  if(GetAttribute(XML_UITEXTNUMBER_VALUE, value)) {
    mObject.SetValue(value);
  }
  return true;
}

CB_DEFINEXMLREAD(CUITextNumber<float>) {
  if(!cb::ReadXmlObject(mNode, (CUIText&)mObject)) {
    return false;
  }
  cb::string format;
  float value;
  if(GetAttribute(XML_UITEXTNUMBER_FORMAT, format)) {
    mObject.SetFormat(format);
  }
  if(GetAttribute(XML_UITEXTNUMBER_VALUE, value)) {
    mObject.SetValue(value);
  }
  return true;
}

static const cb::string XML_UIRECT_COLOR = L"Color";
static const cb::string XML_UIRECT_FILLRECT = L"FillRect";

CB_DEFINEXMLREAD(CUIRect) {
  if(!cb::ReadXmlObject(mNode, (CUIItemBase&)mObject)) {
    return false;
  }
  bool fillRect;
  glm::vec4 color;
  if(GetAttribute(XML_UIRECT_COLOR, color)) {
    mObject.SetColor(color);
  }
  if(GetAttribute(XML_UIRECT_FILLRECT, fillRect)) {
    mObject.SetFillRect(fillRect);
  }
  return true;
}

static const cb::string XML_UIPANEL_HALIGN = L"ContentHorAlign";
static const cb::string XML_UIPANEL_VALIGN = L"ContentVerAlign";

CB_DEFINEXMLREAD(CUIPanel) {
  if(!cb::ReadXmlObject(mNode, (CUIRect&)mObject)) {
    return false;
  }
  UIHAlign halign;
  UIVAlign valign;
  if(GetAttribute(XML_UIPANEL_HALIGN, halign)) {
    mObject.SetItemAlign(halign, mObject.GetVAlign());
  }
  if(GetAttribute(XML_UIPANEL_VALIGN, valign)) {
    mObject.SetItemAlign(mObject.GetHAlign(), valign);
  }

  if(!mNode.Nodes.IsEmpty()) {
    IUIItem* pItem = CreateItemFromNode(*mNode.Nodes.Begin());
    mObject.SetItem(pItem);
  }
  return true;
}

static const cb::string XML_UISTACK_ORIENTATION = L"Orientation";
static const cb::string XML_UISTACK_HALIGN = L"ContentHorAlign";
static const cb::string XML_UISTACK_VALIGN = L"ContentVerAlign";


CB_DEFINEXMLREAD(CUIStack) {
  if(!cb::ReadXmlObject(mNode, (CUIRect&)mObject)) {
    return false;
  }
  UIOrientation orient;
  UIHAlign halign;
  UIVAlign valign;
  if(GetAttribute(XML_UISTACK_ORIENTATION, orient)) {
    mObject.SetOrientation(orient);
  }
  if(GetAttribute(XML_UISTACK_HALIGN, halign)) {
    mObject.SetItemsAlign(halign, mObject.GetVAlign());
  }
  if(GetAttribute(XML_UISTACK_VALIGN, valign)) {
    mObject.SetItemsAlign(mObject.GetHAlign(), valign);
  }

  for(cb::CXmlNodeList::iterator it = mNode.Nodes.Begin();
      it != mNode.Nodes.End(); it++) {
    IUIItem* pItem = CreateItemFromNode(*it);
    if(pItem) {
      mObject.AddItem(pItem);
    }
  }
  return true;
}

static const cb::string XML_UIITEMLIST_ITEMHALIGN = L"HorAlign";
static const cb::string XML_UIITEMLIST_ITEMVALIGN = L"VerAlign";

CB_DEFINEXMLREAD(CUIItemList) {
  if(!cb::ReadXmlObject(mNode, (CUIRect&)mObject)) {
    return false;
  }
  for(cb::CXmlNodeList::iterator it = mNode.Nodes.Begin();
      it != mNode.Nodes.End(); it++) {
    UIHAlign halign = UIHAlign::Center;
    UIVAlign valign = UIVAlign::Middle;

    IUIItem* pItem = CreateItemFromNode(*it);
    if(pItem == nullptr) {
      continue;
    }

    cb::CXmlSerialize<IUIItem> tmp(*it, *pItem);
    tmp.GetAttribute(XML_UIITEMLIST_ITEMHALIGN, halign);
    tmp.GetAttribute(XML_UIITEMLIST_ITEMVALIGN, valign);

    mObject.AddItem(pItem, halign, valign);
  }
  return true;
}

static const cb::string XML_UIPROGRESSBAR_BARCOLOR = L"BarColor";
static const cb::string XML_UIPROGRESSBAR_MINVALUE = L"MinValue";
static const cb::string XML_UIPROGRESSBAR_MAXVALUE = L"MaxValue";
static const cb::string XML_UIPROGRESSBAR_VALUE = L"Value";

CB_DEFINEXMLREAD(CUIProgressBar) {
  if(!cb::ReadXmlObject(mNode, (CUIRect&)mObject)) {
    return false;
  }
  glm::vec4 barColor;
  float min, max, val;
  if(GetAttribute(XML_UIPROGRESSBAR_BARCOLOR, barColor)) {
    mObject.SetBarColor(barColor);
  }
  if(GetAttribute(XML_UIPROGRESSBAR_MINVALUE, min)) {
    mObject.SetRange(min, mObject.GetMaxValue());
  }
  if(GetAttribute(XML_UIPROGRESSBAR_MAXVALUE, max)) {
    mObject.SetRange(mObject.GetMinValue(), max);
  }
  if(GetAttribute(XML_UIPROGRESSBAR_VALUE, val)) {
    mObject.SetValue(val);
  }
  return true;
}

