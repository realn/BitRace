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
public:
  CGUIScreenRectItem(CGUIScreen* pScreen, const glm::vec2& pos, const glm::vec2& size);
  virtual ~CGUIScreenRectItem();

  virtual void Render() override;
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
};
