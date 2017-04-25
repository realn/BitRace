#ifndef __BITRACE_UIFONT_H__
#define __BITRACE_UIFONT_H__

#include <SDL_types.h>
#include <glm/fwd.hpp>
#include <map>
#include <CBStr/Defines.h>

class IFileSystem;
class CTexture;

typedef std::vector<glm::vec3> vec3vector;
typedef std::vector<glm::vec2> vec2vector;

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
  void CreateMesh(const cb::string& text, vec3vector& vertices, 
                  vec2vector& texcoords, 
                  const glm::vec2& charSize = glm::vec2(1.0f)) const;

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


#endif // !__BITRACE_UIFONT_H__

