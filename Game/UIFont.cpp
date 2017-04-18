#include "stdafx.h"
#include "UIFont.h"
#include "FGXFile.h"
#include "Texture.h"

typedef std::vector<glm::vec2> vec2vector;

CUIFont::CUIFont() {
  for(Uint16 i = 0; i < 256; ++i) {
    float cx = float(i % 16) / 16.0f;
    float cy = float(i / 16) / 16.0f;

    CChar fontChar;

    fontChar.Code = i;

    fontChar.TexCoord[0] = glm::vec2(cx, 1 - cy - 0.0625f);
    fontChar.TexCoord[1] = glm::vec2(cx + 0.0625f, 1 - cy - 0.0625f);
    fontChar.TexCoord[2] = glm::vec2(cx + 0.0625f, 1 - cy);
    fontChar.TexCoord[3] = glm::vec2(cx, 1 - cy);

    fontChar.Size = glm::vec2(1.0f, 1.0f);

    fontChar.Adv = glm::vec2(0.6f, 0.0f);

    mCharMap[i] = fontChar;
  }
}

CUIFont::~CUIFont() {}

const CUIFont::CChar& CUIFont::GetChar(const wchar_t code) const {
  //  return mCharMap.find(code)->second;
  Uint16 c = (Uint16)code - 32;

  return mCharMap.at(c);
}

const glm::vec2 CUIFont::GetSize(const cb::string & text) const {
  glm::vec2 result = glm::vec2(0.0f, 0.0f);
  for(cb::string::const_iterator it = text.begin(); it != text.end(); it++) {
    const CChar& fontChar = GetChar(*it);

    result.x += fontChar.Adv.x;
    result.y = glm::max(result.y, fontChar.Size.y);
  }
  return result;
}





CUIText::CUIText(const glm::vec2& screenSize, const glm::vec2& charSize)
  : mTexture(nullptr)
  , mCharSize(charSize)
{
  mProjMatrix = glm::ortho(0.0f, screenSize.x, screenSize.y, 0.0f);

  this->mVertex[0] = glm::vec2(0.0f, 1.0f);
  this->mVertex[1] = glm::vec2(1.0f, 1.0f);
  this->mVertex[2] = glm::vec2(1.0f, 0.0f);
  this->mVertex[3] = glm::vec2(0.0f, 0.0f);
}

CUIText::~CUIText() {
  Free();
}

const bool CUIText::LoadFontTexture(const cb::string& filename) {
  CFGXFile imgFile;
  if(!imgFile.Load(filename)) {
    return false;
  }

  if(!imgFile.IsValid()) {
    return false;
  }

  glm::ivec2 size = imgFile.GetSize();
  Uint32 format = 0;
  switch(imgFile.GetImgDepth()) {
  case 1: format = GL_LUMINANCE8;
  case 2: format = GL_LUMINANCE8_ALPHA8;
  case 3: format = GL_RGB;
  case 4: format = GL_RGBA;
  };
  const cb::bytevector& Data = imgFile.GetData();

  mTexture = new CTexture(GL_TEXTURE_2D, size, GL_RGBA);
  mTexture->Load(format, imgFile.GetData());
  return true;
}

const bool CUIText::Init(const cb::string& fontTextureFilepath) {
  if(!LoadFontTexture(fontTextureFilepath)) {
    mTexture = 0;
    return false;
  }

  return true;
}

void CUIText::Free() {
  delete mTexture;
  mTexture = nullptr;
}

const bool CUIText::IsInited() const {
  return mTexture != nullptr;
}

const glm::vec2 CUIText::GetCharSize() const {
  return mCharSize;
}

void CUIText::Bind() const {
  if(!IsInited()) {
    cb::error(L"Unitialized UI Text, cannot bind.");
    return;
  }
  glPushAttrib(GL_ENABLE_BIT);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  mTexture->Bind();
}

void CUIText::UnBind() const {
  if(!IsInited()) {
    return;
  }
  glPopAttrib();
}

void CUIText::Render(const CUIFont& font, const glm::vec2& pos, const cb::string& text, const CUITextContext& context) const {
  if(!IsInited()) {
    cb::error(L"Unitialized UI Text, cannot render.");
    return;
  }
  if(text.empty()) {
    return;
  }


  std::vector<glm::vec2> vertList;
  std::vector<glm::vec2> texList;

  vertList.resize(4 * text.length());
  texList.resize(4 * text.length());

  glm::vec2 textPos = pos;
  for(size_t i = 0; i < text.length(); i++) {
    const CUIFont::CChar& fontChar = font.GetChar(text[i]);

    for(size_t j = 0; j < 4; j++) {
      vertList[i * 4 + j] = mVertex[j] * mCharSize * fontChar.Size * context.Scale + textPos;
      texList[i * 4 + j] = fontChar.TexCoord[j];
    }

    textPos += fontChar.Adv * mCharSize * context.Scale;
  }

  glLoadMatrixf(glm::value_ptr(mProjMatrix));

  glVertexPointer(2, GL_FLOAT, 0, &vertList[0]);
  glTexCoordPointer(2, GL_FLOAT, 0, &texList[0]);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glColor4fv(glm::value_ptr(context.Color));
  glDrawArrays(GL_QUADS, 0, vertList.size());

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void CUIText::RenderQuad(const glm::vec2 & pos, const glm::vec2 & size, const glm::vec4 & color, const Uint32 texId) {
  glm::vec2 vert[4];
  vert[0] = pos;
  vert[1] = pos + glm::vec2(size.x, 0.0f);
  vert[2] = pos + size;
  vert[3] = pos + glm::vec2(0.0f, size.y);

  glm::vec2 texc[4];
  texc[0] = glm::vec2(0.0f, 0.0f);
  texc[1] = glm::vec2(1.0f, 0.0f);
  texc[2] = glm::vec2(1.0f, 1.0f);
  texc[3] = glm::vec2(0.0f, 1.0f);

  Uint16 ind[6];
  ind[0] = 0;
  ind[1] = 1;
  ind[2] = 2;
  ind[3] = 0;
  ind[4] = 2;
  ind[5] = 3;

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vert);
  glTexCoordPointer(2, GL_FLOAT, 0, texc);

  if(texId) {
    glBindTexture(GL_TEXTURE_2D, texId);
  }
  else
    glDisable(GL_TEXTURE_2D);

  glColor4fv(glm::value_ptr(color));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, ind);

  if(texId) {
    //glBindTexture(GL_TEXTURE_2D, mTexture);
  }
  else
    glEnable(GL_TEXTURE_2D);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void CUIText::RenderQuadLines(const glm::vec2 & pos, const glm::vec2 & size, const glm::vec4 & color) {
  glm::vec2 vert[4];
  vert[0] = pos;
  vert[1] = pos + glm::vec2(size.x, 0.0f);
  vert[2] = pos + size;
  vert[3] = pos + glm::vec2(0.0f, size.y);

  Uint16 ind[8];
  ind[0] = 0;
  ind[1] = 1;
  ind[2] = 1;
  ind[3] = 2;
  ind[4] = 2;
  ind[5] = 3;
  ind[6] = 3;
  ind[7] = 0;

  glDisable(GL_TEXTURE_2D);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vert);

  glColor4fv(glm::value_ptr(color));
  glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, ind);

  glDisableClientState(GL_VERTEX_ARRAY);
  glEnable(GL_TEXTURE_2D);
}

void CUIText::RenderQuadFullScreen(const glm::vec2& size, const glm::vec4& color, const Uint32 texId) {
  this->RenderQuad(glm::vec2(0.0f), size, color, texId);
}

void CUIText::RenderProgressBar(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const float progress) {
  RenderQuadLines(pos, size, color);

  float sizeX = size.x * (progress / 100.0f);

  RenderQuad(pos, glm::vec2(sizeX, size.y), color);
}

CUITextContext::CUITextContext()
  : Color(1.0f, 1.0f, 1.0f, 1.0f)
  , Scale(1.0f, 1.0f)
{}
