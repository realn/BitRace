#pragma once

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>

class CGame;

class CGUI {
public:
  struct CFontChar {
    Uint8 m_Code;
    glm::vec2   m_TCoord[4];
    glm::vec2   m_Size;
    float m_Adv;
  };

private:
  std::map<Uint8, CFontChar> m_CharList;
  Uint32 m_FontTexture;
  glm::vec2 m_Size;
  glm::vec2 m_Vertex[4];
  glm::mat4 m_Transform;

  bool LoadFontTexture(std::string filename);
  bool InitFont();

public:
  CGUI();
  ~CGUI();

  bool Init();
  void Free();

  void Begin(const glm::vec2& size);
  void End();

  void Print(const glm::vec2& pos, const glm::vec4& color, std::string format, ...) const;

  const glm::vec2 GetPrintSize(const std::string format, ...) const;

  void RenderQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const Uint32 texId = 0, const glm::vec2& texPos = glm::vec2(0.0f), const glm::vec2& texSize = glm::vec2(1.0f));
  void RenderQuadLines(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
  void RenderQuadFullScreen(const glm::vec2& size, const glm::vec4& color, const Uint32 texId = 0, const glm::vec2& texPos = glm::vec2(0.0f), const glm::vec2& texSize = glm::vec2(1.0f));
  void RenderProgressBar(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const float progress);

  static std::string Format(const std::string format, ...);

private:
  const CFontChar&  GetChar(const char charCode) const;
  static const std::string FormatText(const std::string& format, va_list va);
};


