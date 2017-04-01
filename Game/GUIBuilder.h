#pragma once

#include <SDL_types.h>
#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "InstructionBuilder.h"

class CGUIScreen;
class CGUIControl;

class CGUIBuilder {
private:
  typedef std::map<std::string, glm::vec4> colormap;
  typedef std::map<std::string, CGUIControl*> controlmap;
  typedef std::map<std::string, Uint32> texturemap;

  CGUIScreen* m_pScreen;
  
  colormap    m_Colors;
  controlmap  m_Controls;
  texturemap  m_Textures;
  
public:
  CGUIBuilder(CGUIScreen* pScreen);
  ~CGUIBuilder();

  const glm::vec4  GetColor(const std::string& name) const;
  CGUIControl*  GetControl(const std::string& name) const;
  template<typename _Type>
  const bool    GetControl(const std::string& name, _Type*& pControl) const {
    pControl = dynamic_cast<_Type*>(GetControl(name));
    return pControl != nullptr;
  }

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
};

class CGUIInstructionBuilder : public CInstructionBuilder {
private:
  CGUIBuilder*  m_pGUIBuilder;
 
public:
  CGUIInstructionBuilder(CGUIBuilder* pGUIBuilder);
  ~CGUIInstructionBuilder();

  CInstruction* TextShow(const std::string& name, const float time, const std::string& text);
  CInstruction* TextShow(const std::string& name, const float time);

  CInstruction* TextHide(const std::string& name, const float time, const std::string& text);
  CInstruction* TextHide(const std::string& name, const float time);

  CInstruction* TextCount(const std::string& name, const Sint32 fromVal, const Sint32 toVal, const float time);
  CInstruction* TextCount(const std::string& name, const Sint32 fromVal, const Sint32 toVal, const float time, const std::string& prefix = "", const std::string& sufix = "");

  CInstruction* Fade(const std::string& name, const float time, const float fromVal = 0.0f, const float toVal = 1.0f);
  CInstruction* Blend(const std::string& name, const float time, const glm::vec4& fromVal, const glm::vec4& toVal);
};
