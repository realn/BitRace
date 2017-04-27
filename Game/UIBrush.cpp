#include "stdafx.h"
#include "UIBrush.h"
#include "UIFont.h"
#include "Texture.h"
#include "MeshFunctions.h"

CUIBrush::CUIBrush(const glm::vec2& screenSize,
                   const CUIFont* pFont,
                   const glm::vec2& charSize)
  : mFont(pFont)
  , mCharSize(charSize) 
  , mColor(1.0f)
  , mScale(1.0f)
  , mTexCoordPos(0.0f)
  , mTexCoordSize(1.0f)
{
  mProjMatrix = glm::ortho(0.0f, screenSize.x, screenSize.y, 0.0f);
}

CUIBrush::CUIBrush(const CUIBrush & other) 
  : mFont(other.mFont)
  , mCharSize(other.mCharSize)
  , mColor(other.mColor)
  , mScale(other.mScale)
  , mTexCoordPos(other.mTexCoordPos)
  , mTexCoordSize(other.mTexCoordSize)
  , mProjMatrix(other.mProjMatrix)
{}

CUIBrush::~CUIBrush() {}

void CUIBrush::SetColor(const glm::vec4 & color) {
  mColor = color;
}

void CUIBrush::SetScale(const glm::vec2 & scale) {
  mScale = scale;
}

void CUIBrush::SetTexCoord(const glm::vec2 & pos, const glm::vec2 & size) {
  mTexCoordPos = pos;
  mTexCoordSize = size;
}

const glm::vec2 CUIBrush::GetCharSize() const {
  return mCharSize;
}

const glm::vec2 CUIBrush::GetTextSize(const cb::string & text) const {
  if(mFont) {
    return mFont->GetSize(text) * mCharSize * mScale;
  }
  return glm::vec2();
}

void CUIBrush::Bind() const {
  glPushAttrib(GL_ENABLE_BIT);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if(mFont) {
    mFont->GetTexture()->Bind();
  }
}

void CUIBrush::UnBind() const {
  if(mFont) {
    mFont->GetTexture()->UnBind();
  }
  glPopAttrib();
}

void CUIBrush::RebindTexture() const {
  if(mFont) {
    mFont->GetTexture()->Bind();
  }
}

void CUIBrush::RenderText(const glm::vec2& pos, const cb::string& text) const {
  if(text.empty() || mFont == nullptr) {
    return;
  }

  vec3vector vertList;
  vec2vector texList;
  mFont->CreateMesh(text, vertList, texList, mCharSize * mScale);

  glm::mat4 mat = mProjMatrix * glm::translate(glm::vec3(pos, 0.0f));

  glLoadMatrixf(glm::value_ptr(mat));
  glColor4fv(glm::value_ptr(mColor));
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  RenderTexVectorList(GL_QUADS, vertList, texList);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CUIBrush::RenderQuad(const glm::vec2 & pos, const glm::vec2 & size) const {
  glDisable(GL_TEXTURE_2D);
  RenderQuadPriv(pos, size);
  glEnable(GL_TEXTURE_2D);
}

void CUIBrush::RenderQuad(const glm::vec2 & pos, const glm::vec2 & size, 
                          const CTexture & texture) const {
  texture.Bind();
  RenderQuadPriv(pos, size);
  if(mFont) {
    mFont->GetTexture()->Bind();
  }
  else {
    texture.UnBind();
  }
}

void CUIBrush::RenderQuadLines(const glm::vec2 & pos, const glm::vec2 & size) const {
  glm::mat4 mat = mProjMatrix * glm::translate(glm::vec3(pos, 0.0f));

  vec3vector vertices = {
    glm::vec3(0.0f),
    glm::vec3(size.x, 0.0f, 0.0f),
    glm::vec3(size, 0.0f),
    glm::vec3(0.0f, size.y, 0.0f)
  };

  ind16vector indices = {
    0, 1, 1, 2, 2, 3, 3, 0
  };

  glDisable(GL_TEXTURE_2D);
  glColor4fv(glm::value_ptr(mColor));
  glLoadMatrixf(glm::value_ptr(mat));
  RenderVectorList(GL_LINES, vertices, indices);
  glEnable(GL_TEXTURE_2D);
}

void CUIBrush::RenderQuadFullScreen() {
  glm::mat4 proj = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f);
  glLoadMatrixf(glm::value_ptr(proj));

  glColor4fv(glm::value_ptr(mColor));
  glDisable(GL_TEXTURE_2D);
  RenderQuadBase(glm::vec2(1.0f));
  glEnable(GL_TEXTURE_2D);
}

void CUIBrush::RenderQuadFullScreen(const CTexture & texture) {
  glm::mat4 proj = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f);
  glLoadMatrixf(glm::value_ptr(proj));

  glColor4fv(glm::value_ptr(mColor));
  texture.Bind();
  RenderQuadBase(glm::vec2(1.0f));
  glEnable(GL_TEXTURE_2D);
  if(mFont) {
    mFont->GetTexture()->Bind();
  }
}

void CUIBrush::RenderQuadPriv(const glm::vec2 & pos, const glm::vec2 & size) const {
  glm::mat4 mat = mProjMatrix * glm::translate(glm::vec3(pos, 0.0f));

  glColor4fv(glm::value_ptr(mColor));
  glLoadMatrixf(glm::value_ptr(mat));
  RenderQuadBase(size);
}

void CUIBrush::RenderQuadBase(const glm::vec2 & size) const {
  glm::vec2 t0 = mTexCoordPos;
  glm::vec2 t1 = mTexCoordPos + mTexCoordSize;
  vec3vector vertices = {
    glm::vec3(0.0f),
    glm::vec3(size.x, 0.0f, 0.0f),
    glm::vec3(size, 0.0f),
    glm::vec3(0.0f, size.y, 0.0f)
  };

  vec2vector texcoords = {
    t0,
    glm::vec2(t1.x, t0.y),
    t1,
    glm::vec2(t0.x, t1.y)
  };

  ind16vector indices = {
    0, 1, 2, 0, 2, 3
  };

  RenderTexVectorList(GL_TRIANGLES, vertices, texcoords, indices);
}
