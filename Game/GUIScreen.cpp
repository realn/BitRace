#include "GUIScreen.h"
#include "GUI.h"
#include "mathex.h"

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
    mathex::inrange(point.x, pos.x, pos.x + size.x) &&
    mathex::inrange(point.y, pos.y, pos.y + size.y);
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


CGUITextAnimation::CGUITextAnimation(CGUIControllerList* pList, CGUITextControl* pControl, const std::string text, const float animTime, const bool show) :
  IGUIController(pList), m_pControl(pControl), m_Text(text), m_AnimTime(animTime), m_Time(0.0f), m_CharTime(0.0f), m_CharLen(0), m_Visible(show) 
{
  m_CharTime = m_AnimTime / m_Text.length();
}

CGUITextAnimation::CGUITextAnimation(CGUIControllerList* pList, CGUITextControl* pControl, const float animTime, const bool show) :
  IGUIController(pList), m_pControl(pControl), m_Text(pControl->GetText()), m_AnimTime(animTime), m_CharTime(0.0f), m_CharLen(0), m_Visible(show) 
{
  m_CharTime = m_AnimTime / m_Text.length();
}

CGUITextAnimation::~CGUITextAnimation() {}

void CGUITextAnimation::Update(float timeDelta) {
  if(!m_Running)
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
      Done();
      return;
    }
  }
  else {
    m_CharLen--;
    if(m_CharLen == 0) {
      m_pControl->SetVisible(false);
      m_pControl->SetText(m_Text, false);
      Done();
      return;
    }
  }

  m_pControl->SetText(m_Text.substr(0, m_CharLen) + "_", false);
}

void CGUITextAnimation::Start() {
  m_Done = false;
  m_Running = true;
  m_Time = 0.0f;
  if (m_Visible) {
    m_CharLen = 0;
    m_pControl->SetText("_", false);
  }
  else {
    m_CharLen = m_Text.length();
    m_pControl->SetText(m_Text, false);
  }
  m_pControl->SetVisible(true);
}

void CGUITextAnimation::Stop() {
  m_Running = false;
}

void CGUITextAnimation::SetVisible(const bool visible) {
  m_Visible = visible;
}

void CGUITextAnimation::Show() {
  m_Visible = true;
  this->Start();
}

void CGUITextAnimation::Hide() {
  m_Visible = false;
  this->Start();
}



//================================================================================


CGUIFadeAnimation::CGUIFadeAnimation(CGUIControllerList* pList, CGUIControl* pControl, const float animTime, const bool visible) :
  IGUIController(pList), m_pControl(pControl), m_AnimTime(animTime), m_Time(0.0f), m_Visible(visible) 
{}

CGUIFadeAnimation::~CGUIFadeAnimation() {}

void CGUIFadeAnimation::Update(float timeDelta) {
  if(!m_Running)
    return;

  float timeAdd = m_Visible ? timeDelta : -timeDelta;

  m_Time = glm::clamp(m_Time + timeAdd, 0.0f, m_AnimTime);
  glm::vec4 color = m_pControl->GetColor();
  color.a = m_Time / m_AnimTime;
  m_pControl->SetColor(color);

  if(m_Visible && m_Time == m_AnimTime) {
    m_pControl->SetVisible(true);
    Done();
  }
  else if(!m_Visible && m_Time == 0.0f) {
    m_pControl->SetVisible(false);
    Done();
  }
  else {
    m_pControl->SetVisible(true);
  }
}

void CGUIFadeAnimation::Start() {
  m_Done = false;
  m_Running = true;
  glm::vec4 color = m_pControl->GetColor();
  if (m_Visible) {
    color.a = 0.0f;
    m_Time = 0.0f;
  }
  else {
    color.a = 1.0f;
    m_Time = m_AnimTime;
  }
  m_pControl->SetColor(color);
  m_pControl->SetVisible(true);
}

void CGUIFadeAnimation::Stop() {
  m_Running = false;
}

void CGUIFadeAnimation::SetVisible(const bool visible) {
  m_Visible = visible;
}

void CGUIFadeAnimation::Show() {
  m_Visible = true;
  Start();
}

void CGUIFadeAnimation::Hide() {
  m_Visible = false;
  Start();
}

const bool CGUIFadeAnimation::IsVisible() const {
  return m_Visible;
}


//================================================================================


CGUITimer::CGUITimer(CGUIControllerList* pList, const float waitTime, IGUIController* pTarget, const bool started) :
  IGUIController(pList, started), m_pTarget(pTarget), m_WaitTime(waitTime), m_Time(0.0f) {}

CGUITimer::~CGUITimer() {}

void CGUITimer::Update(const float timeDelta) {
  if (!m_Running)
    return;

  m_Time = glm::clamp(m_Time + timeDelta, 0.0f, m_WaitTime);
  if(m_Time >= m_WaitTime) {
    Done();
    if(m_pTarget) {
      m_pTarget->Start();
    }
  }
}

void CGUITimer::Start() {
  m_Time = 0.0f;
  m_Running = true;
  m_Done = false;
}

void CGUITimer::Stop() {
  m_Running = false;;
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

IGUIController* CGUIControllerList::Add(IGUIController * pController) {
  m_List.insert(pController);
  return pController;
}

void CGUIControllerList::Clear() {
  for (std::set<IGUIController*>::iterator it = m_List.begin(); it != m_List.end(); it++) {
    delete *it;
  }
  m_List.clear();
}


//================================================================================


CGUIDoneTrigger::CGUIDoneTrigger(CGUIControllerList* pList, IGUIController * pCondition, IGUIController * pTarget, const bool started) :
  IGUIController(pList, started), m_CondOp(CO_AND)
{
  this->AddCondition(pCondition);
  this->AddTarget(pTarget);
}

CGUIDoneTrigger::~CGUIDoneTrigger() {}

void CGUIDoneTrigger::AddCondition(IGUIController * pCondition) {
  if(pCondition == nullptr)
    return;
  if(std::find(m_Conditions.begin(), m_Conditions.end(), pCondition) != m_Conditions.end())
    return;

  m_Conditions.push_back(pCondition);
}

void CGUIDoneTrigger::AddTarget(IGUIController * pTarget) {
  if(pTarget == nullptr)
    return;
  if(std::find(m_Targets.begin(), m_Targets.end(), pTarget) != m_Targets.end())
    return;

  m_Targets.push_back(pTarget);
}

void CGUIDoneTrigger::SetCondOp(const CondOp condOp) {
  m_CondOp = condOp;
}

void CGUIDoneTrigger::Update(const float timeDelta) {
  if (!m_Running)
    return;

  bool cond = m_CondOp == CO_AND;

  for(std::vector<IGUIController*>::iterator it = m_Conditions.begin(); it != m_Conditions.end(); it++) {
    if(m_CondOp == CO_AND)
      cond &= (*it)->IsDone();
    else
      cond |= (*it)->IsDone();
  }

  if (cond) {
    Done();

    for(std::vector<IGUIController*>::iterator it = m_Targets.begin(); it != m_Targets.end(); it++) {
      (*it)->Start();
    }
  }
}

void CGUIDoneTrigger::Start() {
  m_Done = false;
  m_Running = true;
}

void CGUIDoneTrigger::Stop() {
  m_Running = false;
}


//================================================================================


