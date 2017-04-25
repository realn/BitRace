#ifndef __BITRACE_UIBRUSH_H__
#define __BITRACE_UIBRUSH_H__

#include <SDL_types.h>
#include <glm/glm.hpp>
#include <CBStr/Defines.h>

class CUIFont;
class CTexture;

class CUIBrush {
private:
  const CUIFont* mFont;
  glm::vec2 mCharSize;
  glm::vec4 mColor;
  glm::vec2 mScale;
  glm::vec2 mTexCoordPos;
  glm::vec2 mTexCoordSize;
  glm::mat4 mProjMatrix;

public:
  CUIBrush(const glm::vec2& screenSize, const CUIFont* pFont = nullptr, const glm::vec2& charSize = glm::vec2(16.0f, 16.0f));
  CUIBrush(const CUIBrush& other);
  ~CUIBrush();

  void SetColor(const glm::vec4& color);
  void SetScale(const glm::vec2& scale);
  void SetTexCoord(const glm::vec2& pos, const glm::vec2& size);

  const glm::vec2 GetCharSize() const;
  const glm::vec2 GetTextSize(const cb::string& text) const;

  void Bind() const;
  void UnBind() const;
  void RebindTexture() const;

  void RenderText(const glm::vec2& pos, const cb::string& text) const;

  void RenderQuad(const glm::vec2& pos, const glm::vec2& size) const;
  void RenderQuad(const glm::vec2& pos, const glm::vec2& size,
                  const CTexture& texture) const;
  void RenderQuadLines(const glm::vec2& pos, const glm::vec2& size) const;
  void RenderQuadFullScreen();
  void RenderQuadFullScreen(const CTexture& texture);

private:
  void RenderQuadPriv(const glm::vec2& pos, const glm::vec2& size) const;
  void RenderQuadBase(const glm::vec2& size) const;
};


#endif // !__BITRACE_UIBRUSH_H__
