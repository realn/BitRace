#include "GUIScreen.h"
#include "GUI.h"

#include "../Common/MathHelper.h"

CGUIScreenItem::CGUIScreenItem(CGUIScreen * pScreen, const glm::vec2 & pos, const glm::vec2 & size) :
  m_pScreen(pScreen), m_Pos(pos), m_Size(size), m_Color(1.0f), m_Align(IA_DEFAULT), m_Visible(true) 
{
}

CGUIScreenItem::~CGUIScreenItem() {}

const bool CGUIScreenItem::Contains(const glm::vec2 & point) const {
  glm::vec2 pos = this->GetRenderPos();
  return
    math::inrange(point.x, pos.x, pos.x + m_Size.x) &&
    math::inrange(point.y, pos.y, pos.y + m_Size.y);
}

void CGUIScreenItem::SetPos(const glm::vec2 & pos) {
  this->m_Pos = pos;
}

void CGUIScreenItem::SetSize(const glm::vec2 & size) {
  this->m_Size = size;
}

void CGUIScreenItem::SetColor(const glm::vec4 & color) {
  this->m_Color = color;
}

void CGUIScreenItem::SetAlign(const Uint32 flags) {
  this->m_Align = flags;
}


void CGUIScreenItem::SetVisible(const bool visible) {
  this->m_Visible = visible;
}

const glm::vec2 & CGUIScreenItem::GetPos() const {
  return this->m_Pos;
}

const glm::vec2 & CGUIScreenItem::GetSize() const {
  return this->m_Size;
}

const glm::vec4 & CGUIScreenItem::GetColor() const {
  return this->m_Color;
}

const Uint32 CGUIScreenItem::GetAlign() const {
  return this->m_Align;
}

const bool CGUIScreenItem::IsVisible() const {
  return this->m_Visible;
}

const glm::vec2 CGUIScreenItem::GetRenderPos() const {
  glm::vec2 result;
  glm::vec2 screenSize = this->m_pScreen->GetSize();

  if((this->m_Align & IA_LEFT) > 0 && (this->m_Align & IA_RIGHT) > 0) {
    result.x = ((screenSize.x - this->m_Size.x) / 2.0f) + this->m_Pos.x;
  }
  else if((this->m_Align & IA_RIGHT) > 0) {
    result.x = screenSize.x - this->m_Size.x - this->m_Pos.x;
  }
  else {
    result.x = this->m_Pos.x;
  }

  if((this->m_Align & IA_TOP) > 0 && (this->m_Align & IA_BOTTOM) > 0) {
    result.y = ((screenSize.y - this->m_Size.y) / 2.0f) + this->m_Pos.y;
  }
  else if((this->m_Align & IA_BOTTOM) > 0) {
    result.y = screenSize.y - this->m_Size.y - this->m_Pos.y;
  }
  else {
    result.y = this->m_Pos.y;
  }

  return result;
}


//================================================================================


CGUIScreenTextItem::CGUIScreenTextItem(CGUIScreen * pScreen, const glm::vec2 & pos, const std::string & text) :
  CGUIScreenItem(pScreen, pos, glm::vec2(0.0f)), m_Text(text) 
{
  this->m_Size = pScreen->GetGUI()->GetPrintSize(m_Text);
}

CGUIScreenTextItem::~CGUIScreenTextItem() {}

void CGUIScreenTextItem::Render() {
  if(!this->m_Visible)
    return;

  CGUI* pGUI = this->m_pScreen->GetGUI();
  glm::vec2 pos = this->GetRenderPos();

  pGUI->Print(pos, this->m_Color, this->m_Text);
}

void CGUIScreenTextItem::SetText(const std::string & text, bool changeSize) {
  this->m_Text = text;
  if(changeSize) {
    this->m_Size = this->m_pScreen->GetGUI()->GetPrintSize(this->m_Text);
  }
}

const std::string & CGUIScreenTextItem::GetText() const {
  return this->m_Text;
}


//================================================================================


CGUIScreenRectItem::CGUIScreenRectItem(CGUIScreen * pScreen, const glm::vec2 & pos, const glm::vec2 & size) :
  CGUIScreenItem(pScreen, pos, size) {}

CGUIScreenRectItem::~CGUIScreenRectItem() {}

void CGUIScreenRectItem::Render() {
  if(!this->m_Visible)
    return;

  CGUI* pGUI = this->m_pScreen->GetGUI();
  glm::vec2 pos = this->GetRenderPos();

  pGUI->RenderQuad(pos, this->m_Size, this->m_Color);
}


//================================================================================


CGUIScreen::CGUIScreen(CGUI * pGUI, const glm::vec2 & size) :
  m_pGUI(pGUI), m_Size(size) {}

CGUIScreen::~CGUIScreen() {
  this->ClearItems(true);
}

void CGUIScreen::AddItem(CGUIScreenItem * pItem) {
  if(std::find(this->m_Items.begin(), this->m_Items.end(), pItem) != this->m_Items.end())
    return;

  this->m_Items.push_back(pItem);
}

void CGUIScreen::RemoveItem(CGUIScreenItem * pItem, bool deleteItem) {
  std::vector<CGUIScreenItem*>::iterator it = std::find(m_Items.begin(), m_Items.end(), pItem);
  if(it == m_Items.end())
    return;

  m_Items.erase(it);
  if(deleteItem)
    delete pItem;
}

void CGUIScreen::ClearItems(bool deleteItems) {
  if(deleteItems) {
    for(std::vector<CGUIScreenItem*>::iterator it = m_Items.begin(); it != m_Items.end(); it++) {
      delete *it;
    }
  }
  m_Items.clear();
}

void CGUIScreen::Render() {
  this->m_pGUI->Begin(this->m_Size);

  for(std::vector<CGUIScreenItem*>::iterator it = m_Items.begin(); it != m_Items.end(); it++) {
    (*it)->Render();
  }

  this->m_pGUI->End();
}

CGUI * CGUIScreen::GetGUI() const {
  return this->m_pGUI;
}

const glm::vec2 CGUIScreen::GetSize() const {
  return m_Size;
}
