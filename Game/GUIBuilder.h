#pragma once

#include <SDL_types.h>
#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>

class CGUIScreen;
class CGUIControl;

class CGUIController;
class CGUIControllerList;
class CGUIDoneTrigger;

class CGUIBuilder {
private:
  class CContext {
  protected:
    CGUIControllerList* m_pList;
    CGUIController* m_pBegin;
    CGUIController* m_pEnd;

  public:
    CContext(CGUIControllerList* pList);
    virtual ~CContext();

    virtual void AddController(CGUIController* pBegin, CGUIController* pEnd) = 0;
    CGUIController* GetBegin() const;
    CGUIController* GetEnd() const;
  };

  class CSequenceContext :
    public CContext
  {
  public:
    CSequenceContext(CGUIControllerList* pList);
    virtual ~CSequenceContext();

    // Inherited via CContext
    virtual void AddController(CGUIController * pBegin, CGUIController * pEnd) override;
  };

  class CParallelContext :
    public CContext 
  {
  private:
    CGUIDoneTrigger* m_pTriggerBegin;
    CGUIDoneTrigger* m_pTriggerEnd;

  public:
    CParallelContext(CGUIControllerList* pList);
    virtual ~CParallelContext();

    // Inherited via CContext
    virtual void AddController(CGUIController * pBegin, CGUIController * pEnd) override;
  };

  typedef std::map<std::string, glm::vec4> colormap;
  typedef std::map<std::string, CGUIControl*> controlmap;
  typedef std::map<std::string, Uint32> texturemap;
  typedef std::vector<CContext*>  contextstack;

  CGUIScreen* m_pScreen;
  CGUIControllerList* m_pList;
  
  contextstack  m_ContextStack;
  CContext* m_pCurrentContext;

  colormap    m_Colors;
  controlmap  m_Controls;
  texturemap  m_Textures;
  
public:
  CGUIBuilder(CGUIScreen* pScreen, CGUIControllerList* pControllerList);
  ~CGUIBuilder();

  CGUIController* GetFirst() const;
  CGUIController* GetLast() const;

  void  BeginSequence();
  void  EndSequence();

  void  BeginParallel();
  void  EndParallel();

  void  Color(const std::string& name, const glm::vec4& color);
  void  Color(const std::string& name, const float r, const float g, const float b, const float a);

  void  Texture(const std::string& name, const Uint32 texId);

  void  Label(const std::string& name, const std::string& text);
  void  Label(const std::string& name, const std::string& text, const glm::vec4& color);
  void  Label(const std::string& name, const std::string& text, const std::string& color);

  void  Rect(const std::string& name, const glm::vec2& size);
  void  Rect(const std::string& name, const glm::vec2& size, const glm::vec4& color);
  void  Rect(const std::string& name, const glm::vec2& size, const std::string& color);

  void  Put(const std::string& name, const glm::vec2& pos, const Uint32 align);

  void  SetVisible(const std::string& name, const bool value);
  void  SetRectTexture(const std::string& name, const std::string& texName, const glm::vec2& pos, const glm::vec2& size);

  void  TextShow(const std::string& name, const float time, const std::string& text);
  void  TextShow(const std::string& name, const float time);

  void  TextHide(const std::string& name, const float time, const std::string& text);
  void  TextHide(const std::string& name, const float time);

  void  FadeIn(const std::string& name, const float time, const float min = 0.0f, const float max = 1.0f);

  void  FadeOut(const std::string& name, const float time, const float min = 0.0f, const float max = 1.0f);

  void  BlendIn(const std::string& name, const float time, const glm::vec4& min = glm::vec4(0.0f), const glm::vec4& max = glm::vec4(1.0f));

  void  BlendOut(const std::string& name, const float time, const glm::vec4& min = glm::vec4(0.0f), const glm::vec4& max = glm::vec4(1.0f));

  void  Wait(const float time);

private:
  void  BeginBlock(CContext* pNewContext);
  void  EndBlock();
  void  AddController(CGUIController* pController);

  template<typename _Type>
  _Type* GetControl(const std::string& name) {
    return dynamic_cast<_Type*>(m_Controls[name]);
  }
};