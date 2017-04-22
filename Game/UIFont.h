#ifndef __BITRACE_UIFONT_H__
#define __BITRACE_UIFONT_H__

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <map>
#include <CBStr/Defines.h>

class IFileSystem;
class CTexture;

class CUIFont {
public:
  class CChar {
  public:
    Uint16 Code;
    glm::vec2 Pos;
    glm::vec2 Size;
    glm::vec2 TexPos;
    glm::vec2 TexSize;
    glm::vec2 Adv;

    CChar();
  };
private:
  typedef std::map<Uint16, CChar> CharMapT;

  CharMapT mCharMap;
  CTexture* mTexture;

public:
  CUIFont();
  ~CUIFont();

  const bool Load(IFileSystem& fs, const cb::string& fontFilePath);

  const CTexture* GetTexture() const;
  const CChar&  GetChar(const wchar_t code) const;
  const glm::vec2 GetSize(const cb::string& text) const;

private:
  const bool LoadTexture(IFileSystem& fs, const cb::string& filepath);
};

class CUITextContext {
public:
  glm::vec4 Color;
  glm::vec2 Scale;

  CUITextContext();
};

class CUIText {
private:
  const CUIFont& mFont;
  glm::vec2 mCharSize;
  glm::vec2 mVertex[4];
  glm::mat4 mProjMatrix;

public:
  CUIText(const CUIFont& font, const glm::vec2& screenSize, 
          const glm::vec2& charSize = glm::vec2(16.0f, 16.0f));
  ~CUIText();

  const glm::vec2 GetCharSize() const;

  void Bind() const;
  void UnBind() const;

  void Render(const glm::vec2& pos, const cb::string& text, const CUITextContext& context = CUITextContext()) const;

  void RenderQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const Uint32 texId = 0);
  void RenderQuadLines(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
  void RenderQuadFullScreen(const glm::vec2& size, const glm::vec4& color, const Uint32 texId = 0);
  void RenderProgressBar(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const float progress);
};


#endif // !__BITRACE_UIFONT_H__

