#include "GUIScreen.h"
#include "GUI.h"

#include "../Common/MathHelper.h"

CGUIControl::CGUIControl(CGUIScreen * pScreen, const glm::vec2 & size, const glm::vec4 & color) :
  m_pScreen(pScreen), m_Size(size), m_Color(color), m_Visible(true) {}

CGUIControl::~CGUIControl() {}

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


CGUITextControl::CGUITextControl(CGUIScreen * pScreen, const std::string & text, const glm::vec4& color) :
  CGUIControl(pScreen, glm::vec2(0.0f), color), m_Text(text) 
{
  this->m_Size = pScreen->GetGUI()->GetPrintSize(m_Text);
}

CGUITextControl::~CGUITextControl() {}

void CGUITextControl::Render(const glm::vec2& pos) {
  if (!this->m_Visible)
    return;

  CGUI* pGUI = this->m_pScreen->GetGUI();

  pGUI->Print(pos, this->m_Color, this->m_Text);
}

void CGUITextControl::SetText(const std::string & text, bool changeSize) {
  this->m_Text = text;
  if (changeSize) {
    this->m_Size = this->m_pScreen->GetGUI()->GetPrintSize(this->m_Text);
  }
}

const std::string & CGUITextControl::GetText() const {
  return this->m_Text;
}


//================================================================================


CGUIRectControl::CGUIRectControl(CGUIScreen * pScreen, const glm::vec2 & size, const glm::vec4& color) :
  CGUIControl(pScreen, size, color), m_TexId(0), m_TexPos(0.0f), m_TexSize(1.0f) {}

CGUIRectControl::~CGUIRectControl() {}

void CGUIRectControl::Render(const glm::vec2& pos) {
  if (!this->m_Visible)
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


CGUIProgressBarControl::CGUIProgressBarControl(CGUIScreen * pScreen, const glm::vec2& size, const float minValue, const float maxValue, const glm::vec4& color) :
  CGUIControl(pScreen, size, color), m_MinValue(minValue), m_MaxValue(maxValue), m_Value(0.0f) {}

CGUIProgressBarControl::~CGUIProgressBarControl() {}

void CGUIProgressBarControl::Render(const glm::vec2& pos) {
  if (!this->m_Visible)
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
    math::inrange(point.x, pos.x, pos.x + size.x) &&
    math::inrange(point.y, pos.y, pos.y + size.y);
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

  if (!deleteControl)
    pItem->SetControl(nullptr);

  m_Items.erase(it);
  delete pItem;
}

void CGUIScreen::ClearControls(const bool deleteControls) {
  for(std::vector<CGUIScreenItem*>::iterator it = m_Items.begin(); it != m_Items.end(); it++) {
    if (!deleteControls)
      (*it)->SetControl(nullptr);
    delete *it;
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


CGUITextAnimation::CGUITextAnimation(CGUITextControl* pControl, const std::string text, const float animTime) :
  m_pControl(pControl), m_Text(text), m_AnimTime(animTime), m_Time(0.0f), m_CharTime(0.0f), m_CharLen(0), m_Animating(false), m_Visible(true) 
{
  m_CharTime = m_AnimTime / m_Text.length();
}

CGUITextAnimation::~CGUITextAnimation() {}

void CGUITextAnimation::Update(float timeDelta) {
  if(!m_Animating)
    return;

  m_Time += timeDelta;
  if(m_Time < m_CharTime)
    return;

  m_Time -= m_CharTime;
  if(m_Visible) {
    m_CharLen++;
    if(m_CharLen >= m_Text.length()) {
      m_pControl->SetVisible(true);
      m_pControl->SetText(m_Text, false);
      m_Animating = false;
      return;
    }
  }
  else {
    m_CharLen--;
    if(m_CharLen == 0) {
      m_pControl->SetVisible(false);
      m_pControl->SetText(m_Text, false);
      m_Animating = false;
      return;
    }
  }

  m_pControl->SetText(m_Text.substr(0, m_CharLen) + "_", false);
}

void CGUITextAnimation::Show() {
  m_Time = 0.0f;
  m_CharLen = 0;
  m_Visible = true;
  m_Animating = true;
  m_pControl->SetText("_", false);
  m_pControl->SetVisible(true);
}

void CGUITextAnimation::Hide() {
  m_Time = 0.0f;
  m_CharLen = m_Text.length();
  m_Visible = false;
  m_Animating = true;
  m_pControl->SetText(m_Text, false);
  m_pControl->SetVisible(true);
}

const bool CGUITextAnimation::IsAnimating() const {
  return m_Animating;
}

const bool CGUITextAnimation::IsVisible() const {
  return m_Visible;
}


//================================================================================


CGUIFadeAnimation::CGUIFadeAnimation(CGUIControl* pControl, const float animTime) :
  m_pControl(pControl), m_AnimTime(animTime), m_Time(0.0f), m_Animating(false), m_Visible(true) 
{}

CGUIFadeAnimation::~CGUIFadeAnimation() {}

void CGUIFadeAnimation::Update(float timeDelta) {
  if(!m_Animating)
    return;

  m_Time = glm::clamp(m_Time + timeDelta, 0.0f, m_AnimTime);
  glm::vec4 color = m_pControl->GetColor();
  if(m_Time >= m_AnimTime) {
    if(m_Visible) {
      color.a = 1.0f;
      m_pControl->SetVisible(true);
    }
    else {
      color.a = 0.0f;
      m_pControl->SetVisible(false);
    }
    m_pControl->SetColor(color);
    m_Animating = false;
    return;
  }
  
  if(m_Visible)
    color.a = m_Time / m_AnimTime;
  else
    color.a = 1.0f - (m_Time / m_AnimTime);

  m_pControl->SetColor(color);
}

void CGUIFadeAnimation::Show() {
  m_Time = 0.0f;
  m_Animating = true;
  m_Visible = true;

  glm::vec4 color = m_pControl->GetColor();
  color.a = 0.0f;
  m_pControl->SetVisible(true);
  m_pControl->SetColor(color);
}

void CGUIFadeAnimation::Hide() {
  m_Time = 0.0f;
  m_Animating = true;
  m_Visible = false;

  glm::vec4 color = m_pControl->GetColor();
  color.a = 1.0f;
  m_pControl->SetVisible(true);
  m_pControl->SetColor(color);
}

const bool CGUIFadeAnimation::IsAnimating() const {
  return m_Animating;
}

const bool CGUIFadeAnimation::IsVisible() const {
  return m_Visible;
}


//================================================================================


CGUITimer::CGUITimer(const float waitTime) :
  m_WaitTime(waitTime), m_Time(0.0f) {}

CGUITimer::~CGUITimer() {}

void CGUITimer::Update(const float timeDelta) {
  if(m_Time >= m_WaitTime)
    return;

  m_Time = glm::clamp(m_Time + timeDelta, 0.0f, m_WaitTime);
}

void CGUITimer::Start() {
  m_Time = 0.0f;
}

void CGUITimer::Stop() {
  m_Time = m_WaitTime;
}

const bool CGUITimer::IsTicking() const {
  return m_Time < m_WaitTime;
}

const bool CGUITimer::IsDone() const {
  return m_Time >= m_WaitTime;
}


//================================================================================


CGUIControllerList::CGUIControllerList() {}

CGUIControllerList::~CGUIControllerList() {
  this->Clear();
}

void CGUIControllerList::Update(const float timeDelta) {
  for (std::set<IGUIController*>::iterator it = m_List.begin(); it != m_List.end(); it++) {
    if ((*it)->IsEnabled()) {
      (*it)->Update(timeDelta);
    }
  }
}

void CGUIControllerList::AddController(IGUIController * pController) {
  m_List.insert(pController);
}

void CGUIControllerList::Clear() {
  for (std::set<IGUIController*>::iterator it = m_List.begin(); it != m_List.end(); it++) {
    delete *it;
  }
  m_List.clear();
}
