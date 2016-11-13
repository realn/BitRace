#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class CGUI;
class CGUIScreen;

class CGUIScreenItem {
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

protected:
  CGUIScreen*  m_pScreen;
  glm::vec2 m_Pos;
  glm::vec2 m_Size;
  glm::vec4 m_Color;
  Uint32    m_Align;
  bool      m_Visible;

public:
  CGUIScreenItem(CGUIScreen* pScreen, const glm::vec2& pos, const glm::vec2& size);
  virtual ~CGUIScreenItem();

  virtual void  Render() = 0;

  const bool Contains(const glm::vec2& point) const;

  void  SetPos(const glm::vec2& pos);
  void  SetSize(const glm::vec2& size);
  void  SetColor(const glm::vec4& color);
  void  SetAlign(const Uint32 flags);
  void  SetVisible(const bool visible);

  const glm::vec2&  GetPos() const;
  const glm::vec2&  GetSize() const;
  const glm::vec4&  GetColor() const;
  const Uint32      GetAlign() const;
  const bool  IsVisible() const;

protected:
  const glm::vec2 GetRenderPos() const;
};

class CGUIScreenTextItem :
  public CGUIScreenItem {
protected:
  std::string m_Text;

public:
  CGUIScreenTextItem(CGUIScreen* pScreen, const glm::vec2& pos, const std::string& text);
  virtual ~CGUIScreenTextItem();

  virtual void Render() override;

  void  SetText(const std::string& text, bool changeSize = true);

  const std::string&  GetText() const;
};

class CGUIScreenRectItem :
  public CGUIScreenItem {
protected:
  Uint32    m_TexId;
  glm::vec2 m_TexPos;
  glm::vec2 m_TexSize;

public:
  CGUIScreenRectItem(CGUIScreen* pScreen, const glm::vec2& pos, const glm::vec2& size);
  virtual ~CGUIScreenRectItem();

  virtual void Render() override;

  void  SetTexture(const Uint32 texId);
  void  SetTextureCoords(const glm::vec2& texPos, const glm::vec2& texSize);
};

class CGUIScreen {
private:
  std::vector<CGUIScreenItem*> m_Items;
  CGUI* m_pGUI;
  glm::vec2 m_Size;

public:
  CGUIScreen(CGUI* pGUI, const glm::vec2& size);
  ~CGUIScreen();

  void  AddItem(CGUIScreenItem* pItem);
  void  RemoveItem(CGUIScreenItem* pItem, bool deleteItem = false);
  void  ClearItems(bool deleteItems = true);

  void  Render();

  CGUI* GetGUI() const;
  const glm::vec2 GetSize() const;
  const float GetAspectRatio() const;
};


class CGUITextAnimation {
private:
  CGUIScreenTextItem* m_pItem;
  std::string m_Text;
  float   m_AnimTime;
  float   m_Time;
  float   m_CharTime;
  Uint32  m_CharLen;
  bool    m_Visible;
  bool    m_Animating;

public:
  CGUITextAnimation(CGUIScreenTextItem* pItem, const std::string text, const float animTime);

  void  Update(const float timeDelta);

  void  Show();
  void  Hide();

  const bool IsAnimating() const;
  const bool IsVisible() const;
};

class CGUIFadeAnimation {
private:
  CGUIScreenItem* m_pItem;
  float m_AnimTime;
  float m_Time;
  bool  m_Visible;
  bool  m_Animating;

public:
  CGUIFadeAnimation(CGUIScreenItem* pItem, const float animTime);

  void Update(const float timeDelta);

  void Show();
  void Hide();

  const bool IsAnimating() const;
  const bool IsVisible() const;
};

class CGUITimer {
private:
  float m_WaitTime;
  float m_Time;

public:
  CGUITimer(const float waitTime);

  void Update(const float timeDelta);

  void Start();
  void Stop();

  const bool IsTicking() const;
  const bool IsDone() const;
};