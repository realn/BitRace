#include "stdafx.h"
#include "GUIScreen.h"
#include "GUI.h"
#include "helper.h"

CGUIControl::CGUIControl(const glm::vec2 & size, const glm::vec4 & color) :
  m_pScreen(nullptr), m_Size(size), m_Color(color), m_Visible(true) 
{
}

CGUIControl::~CGUIControl() 
{
  if(m_pScreen)
    m_pScreen->RemoveControl(this, false);
}

void CGUIControl::SetScreen(CGUIScreen * pScreen) {
  if(m_pScreen)
    m_pScreen->RemoveControl(this, false);
  m_pScreen = pScreen;
}

void CGUIControl::SetSize(const glm::vec2 & size) {
  this->m_Size = size;
}

void CGUIControl::SetColor(const glm::vec4 & color) {
  this->m_Color = color;
}

void CGUIControl::SetVisible(const bool visible) {
  m_Visible = visible;
}

const glm::vec2 & CGUIControl::GetSize() const {
  return m_Size;
}

const glm::vec4 & CGUIControl::GetColor() const {
  return m_Color;
}

const bool CGUIControl::IsVisible() const {
  return m_Visible;
}


//================================================================================


CGUITextControl::CGUITextControl(const std::string & text, const glm::vec4& color) :
  CGUIControl(glm::vec2(0.0f), color), m_Text(text) 
{
}

CGUITextControl::~CGUITextControl() {}

void CGUITextControl::Render(const glm::vec2& pos) {
  if (!this->m_Visible || m_pScreen == nullptr)
    return;

  CGUI* pGUI = this->m_pScreen->GetGUI();

  pGUI->Print(pos, this->m_Color, this->m_Text);
}

void CGUITextControl::SetScreen(CGUIScreen * pScreen) {
  CGUIControl::SetScreen(pScreen);
  if(m_pScreen)
    this->m_Size = m_pScreen->GetGUI()->GetPrintSize(m_Text);
}

void CGUITextControl::SetText(const std::string & text, bool changeSize) {
  this->m_Text = text;
  if (changeSize && m_pScreen) {
    this->m_Size = this->m_pScreen->GetGUI()->GetPrintSize(this->m_Text);
  }
}

const std::string & CGUITextControl::GetText() const {
  return this->m_Text;
}


//================================================================================


CGUIRectControl::CGUIRectControl(const glm::vec2 & size, const glm::vec4& color) :
  CGUIControl(size, color), m_TexId(0), m_TexPos(0.0f), m_TexSize(1.0f) {}

CGUIRectControl::~CGUIRectControl() {}

void CGUIRectControl::Render(const glm::vec2& pos) {
  if (!this->m_Visible || m_pScreen == nullptr)
    return;

  CGUI* pGUI = this->m_pScreen->GetGUI();

  pGUI->RenderQuad(pos, this->m_Size, this->m_Color, this->m_TexId, this->m_TexPos, this->m_TexSize);
}

void CGUIRectControl::SetTexture(const Uint32 texId) {
  this->m_TexId = texId;
}

void CGUIRectControl::SetTextureCoords(const glm::vec2 & texPos, const glm::vec2 & texSize) {
  this->m_TexPos = texPos;
  this->m_TexSize = texSize;
}


//================================================================================


CGUIProgressBarControl::CGUIProgressBarControl(const glm::vec2& size, const float minValue, const float maxValue, const glm::vec4& color) :
  CGUIControl(size, color), m_MinValue(minValue), m_MaxValue(maxValue), m_Value(0.0f) {}

CGUIProgressBarControl::~CGUIProgressBarControl() {}

void CGUIProgressBarControl::Render(const glm::vec2& pos) {
  if (!this->m_Visible || m_pScreen == nullptr)
    return;

  CGUI* pGUI = this->m_pScreen->GetGUI();

  pGUI->RenderQuadLines(pos, m_Size, m_Color);

  float progress = (m_Value - m_MinValue) / (m_MaxValue - m_MinValue);
  glm::vec2 progressSize = glm::vec2(m_Size.x * progress, m_Size.y);

  pGUI->RenderQuad(pos, progressSize, m_Color);
}

void CGUIProgressBarControl::SetRange(const float minValue, const float maxValue) {
  m_MinValue = minValue;
  m_MaxValue = maxValue;
}

void CGUIProgressBarControl::SetValue(const float value) {
  m_Value = value;
}

const float CGUIProgressBarControl::GetValue() const {
  return m_Value;
}


//================================================================================


CGUIScreenItem::CGUIScreenItem(CGUIScreen * pScreen, CGUIControl* pControl , const glm::vec2 & pos, const Uint32 align) :
  m_pScreen(pScreen), m_pControl(pControl), m_Pos(pos), m_Align(align) {}

CGUIScreenItem::~CGUIScreenItem() {
  delete m_pControl;
}

void CGUIScreenItem::Render() {
  glm::vec2 pos = this->GetRenderPos();
  this->m_pControl->Render(pos);
}

const bool CGUIScreenItem::Contains(const glm::vec2 & point) const {
  glm::vec2 pos = this->GetRenderPos();
  glm::vec2 size = this->m_pControl->GetSize();
  return
    helper::inrange(point.x, pos.x, pos.x + size.x) &&
    helper::inrange(point.y, pos.y, pos.y + size.y);
}

void CGUIScreenItem::SetControl(CGUIControl * pControl) {
  this->m_pControl = pControl;
}

void CGUIScreenItem::SetPos(const glm::vec2 & pos) {
  this->m_Pos = pos;
}

void CGUIScreenItem::SetAlign(const Uint32 flags) {
  this->m_Align = flags;
}

CGUIControl * CGUIScreenItem::GetControl() const {
  return m_pControl;
}

const glm::vec2 & CGUIScreenItem::GetPos() const {
  return this->m_Pos;
}

const Uint32 CGUIScreenItem::GetAlign() const {
  return this->m_Align;
}

const glm::vec2 CGUIScreenItem::GetRenderPos() const {
  glm::vec2 result;
  glm::vec2 size = this->m_pControl->GetSize();
  glm::vec2 screenSize = this->m_pScreen->GetSize();

  if((this->m_Align & CGUIScreen::IA_LEFT) > 0 && (this->m_Align & CGUIScreen::IA_RIGHT) > 0) {
    result.x = ((screenSize.x - size.x) / 2.0f) + this->m_Pos.x;
  }
  else if((this->m_Align & CGUIScreen::IA_RIGHT) > 0) {
    result.x = screenSize.x - size.x - this->m_Pos.x;
  }
  else {
    result.x = this->m_Pos.x;
  }

  if((this->m_Align & CGUIScreen::IA_TOP) > 0 && (this->m_Align & CGUIScreen::IA_BOTTOM) > 0) {
    result.y = ((screenSize.y - size.y) / 2.0f) + this->m_Pos.y;
  }
  else if((this->m_Align & CGUIScreen::IA_BOTTOM) > 0) {
    result.y = screenSize.y - size.y - this->m_Pos.y;
  }
  else {
    result.y = this->m_Pos.y;
  }

  return result;
}


//================================================================================


CGUIScreen::CGUIScreen(CGUI * pGUI, const glm::vec2 & size) :
  m_pGUI(pGUI), m_Size(size) {}

CGUIScreen::~CGUIScreen() {
  this->ClearControls(true);
}

CGUIScreenItem* CGUIScreen::AddControl(CGUIControl* pControl, const glm::vec2& pos, const Uint32 align) {
  if(FindControl(pControl) != m_Items.end())
    return nullptr;

  pControl->SetScreen(this);
  CGUIScreenItem* pItem = new CGUIScreenItem(this, pControl, pos, align);
  this->m_Items.push_back(pItem);
  return pItem;
}

CGUIScreenItem* CGUIScreen::GetItem(CGUIControl * pControl) const {
  CGUIScreenItem* pItem = nullptr;
  if (FindControl(pControl, &pItem) != m_Items.end())
    return pItem;
  return nullptr;
}

void CGUIScreen::RemoveControl(CGUIControl* pControl, const bool deleteControl) {
  CGUIScreenItem* pItem = nullptr;
  std::vector<CGUIScreenItem*>::const_iterator it = FindControl(pControl, &pItem);
  if (it == m_Items.end())
    return;

  m_Items.erase(it);

  if(!deleteControl) {
    pItem->GetControl()->SetScreen(nullptr);
    pItem->SetControl(nullptr);
  }
  delete pItem;
}

void CGUIScreen::ClearControls(const bool deleteControls) {
  std::vector<CGUIScreenItem*> items = m_Items;
  m_Items.clear();

  for(std::vector<CGUIScreenItem*>::iterator it = items.begin(); it != items.end(); it++) {
    if(!deleteControls) {
      (*it)->GetControl()->SetScreen(nullptr);
      (*it)->SetControl(nullptr);
    }
    delete *it;
  }
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

const float CGUIScreen::GetAspectRatio() const {
  return m_Size.x / m_Size.y;
}

std::vector<CGUIScreenItem*>::const_iterator CGUIScreen::FindControl(CGUIControl * pControl, CGUIScreenItem ** ppItem) const {
  for (std::vector<CGUIScreenItem*>::const_iterator it = m_Items.begin(); it != m_Items.end(); it++) {
    if ((*it)->GetControl() == pControl) {
      if (ppItem != nullptr) {
        (*ppItem) = (*it);
      }
      return it;
    }
  }
  return m_Items.end();
}


//================================================================================


