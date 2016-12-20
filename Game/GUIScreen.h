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
  CGUIControl(const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
  virtual ~CGUIControl();

  virtual void  SetScreen(CGUIScreen* pScreen);

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
  CGUITextControl(const std::string& text, const glm::vec4& color = glm::vec4(1.0f));
  virtual ~CGUITextControl();

  virtual void Render(const glm::vec2& pos) override;

  virtual void SetScreen(CGUIScreen* pScreen) override;
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
  CGUIRectControl(const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f));
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
  CGUIProgressBarControl(const glm::vec2& size, const float minValue = 0.0f, const float maxValue = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
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
    IA_DEFAULT = (IA_LEFT | IA_TOP),
    IA_SCREEN_CENTER = (IA_CENTER | IA_MIDDLE)
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

