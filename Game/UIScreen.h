#ifndef __BITRACE_UISCREEN_H__
#define __BITRACE_UISCREEN_H__

#include <glm/fwd.hpp>
#include <SDL_types.h>
#include <CBStr/StringEx.h>

class CUIFont;
class CUIBrush;
class IUIItem;
class IFileSystem;

typedef std::vector<IUIItem*> UIItemVectorT;
class IUIItem {
protected:
  IUIItem();

public:
  virtual ~IUIItem();

  virtual void SetId(const cb::string& id) = 0;
  virtual void SetMargin(const glm::vec2& margin) = 0;
  virtual void SetSize(const glm::vec2& size) = 0;
  virtual void SetVisible(const bool value) = 0;

  virtual const cb::string& GetId() const = 0;
  virtual const glm::vec2& GetMargin() const = 0;
  virtual const glm::vec2& GetSize() const = 0;
  virtual const glm::vec2 GetClientSize() const = 0;
  virtual const bool IsVisible() const = 0;

  virtual void UpdateRender(const glm::vec2& size,
                            const CUIBrush& brush) = 0;
  virtual void Render(const glm::vec2& pos,
                      const CUIBrush& brush) const = 0;

  virtual void GetAllSubItems(UIItemVectorT& items) = 0;
};


class CUIScreen {
private:
  typedef std::map<cb::string, IUIItem*> UIItemIdMapT;

  glm::vec2 mSize;
  glm::vec2 mMargin;
  IUIItem* mItem;
  UIItemVectorT mAllItems;
  UIItemIdMapT mItemIdMap;

public:
  CUIScreen(const glm::vec2& size, const glm::vec2& margin = glm::vec2(0.0f));
  ~CUIScreen();

  void UpdateRender(const CUIFont& font);
  void Render(const CUIFont& font) const;

  const bool Load(IFileSystem& fs, const cb::string& filepath);
  const bool Save(IFileSystem& fs, const cb::string& filepath) const;

  void UpdateItemLists();

  void SetItem(IUIItem* pItem);
  IUIItem* GetItemById(const cb::string& id) const;
  template<typename _Type>
  _Type* GetItem(const cb::string& id) const {
    IUIItem* pItem = GetItemById(id);
    return dynamic_cast<_Type*>(pItem);
  }
};

#endif // !__BITRACE_UISCREEN_H__

