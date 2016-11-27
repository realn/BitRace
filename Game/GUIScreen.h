#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <set>

class CGUI;
class CGUIScreen;

class CGUIControl {
protected:
  CGUIScreen* m_pScreen;
  glm::vec2 m_Size;
  glm::vec4 m_Color;
  bool      m_Visible;

public:
  CGUIControl(CGUIScreen* pScreen, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
  virtual ~CGUIControl();

  virtual void Render(const glm::vec2& pos) = 0;

  void  SetSize(const glm::vec2& size);
  void  SetColor(const glm::vec4& color);
  void  SetVisible(const bool visible);

  const glm::vec2&  GetSize() const;
  const glm::vec4&  GetColor() const;
  const bool  IsVisible() const;
};


class CGUITextControl :
  public CGUIControl {
protected:
  std::string m_Text;

public:
  CGUITextControl(CGUIScreen* pScreen, const std::string& text, const glm::vec4& color = glm::vec4(1.0f));
  virtual ~CGUITextControl();

  virtual void Render(const glm::vec2& pos) override;

  void  SetText(const std::string& text, bool changeSize = true);

  const std::string&  GetText() const;
};

class CGUIRectControl :
  public CGUIControl {
protected:
  Uint32    m_TexId;
  glm::vec2 m_TexPos;
  glm::vec2 m_TexSize;

public:
  CGUIRectControl(CGUIScreen* pScreen, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
  virtual ~CGUIRectControl();

  virtual void Render(const glm::vec2& pos) override;

  void  SetTexture(const Uint32 texId);
  void  SetTextureCoords(const glm::vec2& texPos, const glm::vec2& texSize);
};

class CGUIProgressBarControl :
  public CGUIControl {
protected:
  float m_MinValue;
  float m_MaxValue;
  float m_Value;

public:
  CGUIProgressBarControl(CGUIScreen* pScreen, const glm::vec2& size, const float minValue = 0.0f, const float maxValue = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
  virtual ~CGUIProgressBarControl();

  virtual void Render(const glm::vec2& pos) override;

  void  SetRange(const float minValue, const float maxValue);
  void  SetValue(const float value);

  const float GetValue() const;
};

class CGUIScreenItem {
protected:
  CGUIScreen*  m_pScreen;
  CGUIControl* m_pControl;
  glm::vec2    m_Pos;
  Uint32       m_Align;

public:
  CGUIScreenItem(CGUIScreen* pScreen, CGUIControl* pControl, const glm::vec2& pos, const Uint32 align);
  ~CGUIScreenItem();

  void  Render();

  const bool Contains(const glm::vec2& point) const;

  void  SetControl(CGUIControl* pControl);
  void  SetPos(const glm::vec2& pos);
  void  SetAlign(const Uint32 flags);

  CGUIControl*      GetControl() const;
  const glm::vec2&  GetPos() const;
  const Uint32      GetAlign() const;

protected:
  const glm::vec2 GetRenderPos() const;
};

class CGUIScreen {
public:
  enum ITEMALIGN {
    IA_LEFT = 0x1,
    IA_RIGHT = 0x2,
    IA_TOP = 0x4,
    IA_BOTTOM = 0x8,
    IA_CENTER = (IA_LEFT | IA_RIGHT),
    IA_MIDDLE = (IA_TOP | IA_BOTTOM),
    IA_DEFAULT = (IA_LEFT | IA_TOP)
  };

private:
  std::vector<CGUIScreenItem*> m_Items;
  CGUI* m_pGUI;
  glm::vec2 m_Size;

public:
  CGUIScreen(CGUI* pGUI, const glm::vec2& size);
  ~CGUIScreen();

  CGUIScreenItem* AddControl(CGUIControl* pControl, const glm::vec2& pos = glm::vec2(0.0f), const Uint32 align = IA_DEFAULT);
  CGUIScreenItem* GetItem(CGUIControl* pControl) const;
  void  RemoveControl(CGUIControl* pControl, const bool deleteControl = false);
  void  ClearControls(const bool deleteControls = true);

  void  Render();

  CGUI* GetGUI() const;
  const glm::vec2 GetSize() const;
  const float GetAspectRatio() const;

private:
  std::vector<CGUIScreenItem*>::const_iterator FindControl(CGUIControl* pControl, CGUIScreenItem** ppItem = nullptr) const;
};

class IGUIController;
class CGUIControllerList {
private:
  std::set<IGUIController*> m_List;

public:
  CGUIControllerList();
  ~CGUIControllerList();

  void Update(const float timeDelta);

  IGUIController* Add(IGUIController* pController);
  void Clear();
};

class IGUIController {
protected:
  bool m_Enabled;
  bool m_Running;
  bool m_Done;

public:
  virtual ~IGUIController() {}
  
  virtual void Update(const float timeDelta) = 0;

  virtual void Start() = 0;
  virtual void Stop() = 0;
  void Done() { m_Running = false; m_Done = true; }

  void SetEnabled(const bool enabled) { m_Enabled = enabled; }
  const bool IsEnabled() const { return m_Enabled; }
  const bool IsRunning() const { return m_Running; }
  const bool IsDone() const { return m_Done; }

protected:
  IGUIController(CGUIControllerList* pList, const bool running = false, const bool enabled = true) : 
    m_Enabled(enabled), m_Running(running), m_Done(false) 
  {
    pList->Add(this);
  }
};

class CGUIDoneTrigger :
  public IGUIController 
{
public:
  enum CondOp {
    CO_AND = 0,
    CO_OR = 1
  };
protected:
  std::vector<IGUIController*> m_Conditions;
  std::vector<IGUIController*> m_Targets;
  CondOp  m_CondOp;

public:
  CGUIDoneTrigger(CGUIControllerList* pList, IGUIController* pCondition = nullptr, IGUIController* pTarget = nullptr, bool started = true);
  virtual ~CGUIDoneTrigger();

  void AddCondition(IGUIController* pCondition);
  void AddTarget(IGUIController* pTarget);

  void SetCondOp(const CondOp condOp);

  virtual void Update(const float timeDelta) override;
  virtual void Start() override;
  virtual void Stop() override;
};

class CGUITextAnimation :
  public IGUIController {
private:
  CGUITextControl* m_pControl;
  std::string m_Text;
  float   m_AnimTime;
  float   m_Time;
  float   m_CharTime;
  Uint32  m_CharLen;
  bool    m_Visible;

public:
  CGUITextAnimation(CGUIControllerList* pList, CGUITextControl* pControl, const std::string text, const float animTime, const bool show = true);
  CGUITextAnimation(CGUIControllerList* pList, CGUITextControl* pControl, const float animTime, const bool show = true);
  virtual ~CGUITextAnimation();

  virtual void  Update(const float timeDelta) override;

  virtual void Start() override;
  virtual void Stop() override;

  void SetVisible(const bool visible);
  void Show();
  void Hide();
};

class CGUIFadeAnimation :
  public IGUIController {
private:
  CGUIControl* m_pControl;
  float m_AnimTime;
  float m_Time;
  bool  m_Visible;

public:
  CGUIFadeAnimation(CGUIControllerList* pList, CGUIControl* pControl, const float animTime, const bool visible = true);
  virtual ~CGUIFadeAnimation();

  virtual void Update(const float timeDelta) override;
  virtual void Start() override;
  virtual void Stop() override;

  void SetVisible(const bool visible);
  void Show();
  void Hide();

  const bool IsVisible() const;
};

class CGUITimer :
  public IGUIController {
private:
  IGUIController* m_pTarget;
  float m_WaitTime;
  float m_Time;

public:
  CGUITimer(CGUIControllerList* pList, const float waitTime, IGUIController* pTarget = nullptr, const bool started = false);
  virtual ~CGUITimer();

  virtual void Update(const float timeDelta) override;

  virtual void Start() override;
  virtual void Stop() override;
};