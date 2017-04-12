#ifndef __BITRACE_UIFONT_H__
#define __BITRACE_UIFONT_H__

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <map>
#include <CBStr/Defines.h>

class CUIFont {
public:
  class CChar {
  public:
    Uint16 Code;
    glm::vec2 TexCoord[4];
    glm::vec2 Size;
    glm::vec2 Adv;
  };
private:
  typedef std::map<Uint16, CChar> CharMapT;

  CharMapT mCharMap;

public:
  CUIFont();
  ~CUIFont();

  const CChar&  GetChar(const wchar_t code) const;
  const glm::vec2 GetSize(const cb::string& text) const;
};



class CUIText {
private:
  CUIFont& mFont;
  Uint32 mTexture;
  glm::vec2 m_Vertex[4];
  glm::mat4 mProjMatrix;

  bool LoadFontTexture(const cb::string& filename);

public:
  CUIText(CUIFont& font);
  ~CUIText();

  const bool Init(const cb::string& fontTextureFilepath);
  void Free();

  const bool IsInited() const;

  void Bind(const glm::vec2& size);
  void UnBind();

  void Render(const glm::vec2& pos, const cb::string& text) const;

  void RenderQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const Uint32 texId = 0);
  void RenderQuadLines(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
  void RenderQuadFullScreen(const glm::vec2& size, const glm::vec4& color, const Uint32 texId = 0);
  void RenderProgressBar(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const float progress);
};




#endif // !__BITRACE_UIFONT_H__

