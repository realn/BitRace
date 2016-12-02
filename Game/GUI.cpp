#include "GUI.h"
#include "Game.h"
#include "../Common/FGXFile.h"
#include "../Common/MathHelper.h"
#include <stdarg.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

CGUI::CGUI() {
  this->m_Vertex[0] = glm::vec2(0.0f, 16.0f);
  this->m_Vertex[1] = glm::vec2(16.0f, 16.0f);
  this->m_Vertex[2] = glm::vec2(16.0f, 0.0f);
  this->m_Vertex[3] = glm::vec2(0.0f, 0.0f);
}

CGUI::~CGUI() {
  Free();
}

bool CGUI::LoadFontTexture(std::string filename) {
  CFGXFile imgFile;
  if (!imgFile.Load(filename)) {
    return false;
  }

  if (!imgFile.IsValid()) {
    return false;
  }

  glGenTextures(1, &this->m_FontTexture);
  glBindTexture(GL_TEXTURE_2D, this->m_FontTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glm::ivec2 size = imgFile.GetSize();
  Uint32 format = 0;
  switch (imgFile.GetImgDepth()) {
  case 1: format = GL_LUMINANCE8;
  case 2: format = GL_LUMINANCE8_ALPHA8;
  case 3: format = GL_RGB;
  case 4: format = GL_RGBA;
  };
  const CFGXFile::CData& Data = imgFile.GetData();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, &Data[0]);
  //glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
  return true;
}

bool CGUI::InitFont() {
  Uint32 i;
  float cx;
  float cy;
  for (i = 0; i < 256; ++i) {
    cx = float(i % 16) / 16.0f;
    cy = float(i / 16) / 16.0f;

    CFontChar fontChar;
    
    fontChar.m_Code = (Uint8)i;

    fontChar.m_TCoord[0] = glm::vec2(cx, 1 - cy - 0.0625f);
    fontChar.m_TCoord[1] = glm::vec2(cx + 0.0625f, 1 - cy - 0.0625f);
    fontChar.m_TCoord[2] = glm::vec2(cx + 0.0625f, 1 - cy);
    fontChar.m_TCoord[3] = glm::vec2(cx, 1 - cy);

    fontChar.m_Size = glm::vec2(16.0f, 16.0f);

    fontChar.m_Adv = 10.0f;

    this->m_CharList[(Uint8)i] = fontChar;
  }

  return true;
}

bool CGUI::Init() {
  if (!LoadFontTexture("font.fgx"))
    return false;
  if (!InitFont())
    return false;

  return true;
}

void CGUI::Free() {
  if (glIsTexture(this->m_FontTexture))
    glDeleteTextures(1, &this->m_FontTexture);
}

void CGUI::Begin(const glm::vec2& size) {
  this->m_Size = size;

  m_Transform = glm::ortho(0.0f, size.x, size.y, 0.0f);
  glPushAttrib(GL_ENABLE_BIT);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindTexture(GL_TEXTURE_2D, this->m_FontTexture);
}

void CGUI::End() {
  glPopAttrib();
}

void CGUI::Print(const glm::vec2& pos, const glm::vec4& color, std::string format, ...) const {
  if (format.empty())
    return;

  std::string text;
  va_list ap;

  va_start(ap, format);
  text = FormatText(format, ap);
  va_end(ap);

  std::vector<glm::vec2> vertList;
  std::vector<glm::vec2> texList;

  vertList.resize(4 * text.length());
  texList.resize(4 * text.length());

  glm::vec2 textPos = pos;
  for (size_t i = 0; i < text.length(); i++) {
    const CFontChar& fontChar = this->GetChar(text[i]);

    for (size_t j = 0; j < 4; j++) {
      vertList[i * 4 + j] = this->m_Vertex[j] + textPos;
      texList[i * 4 + j] = fontChar.m_TCoord[j];
    }

    textPos += glm::vec2(fontChar.m_Adv, 0.0f);
  }

  glLoadMatrixf(glm::value_ptr(m_Transform));
  glColor4fv(glm::value_ptr(color));

  glVertexPointer(2, GL_FLOAT, 0, &vertList[0]);
  glTexCoordPointer(2, GL_FLOAT, 0, &texList[0]);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glBlendFunc(GL_ONE, GL_ONE);
  glDrawArrays(GL_QUADS, 0, vertList.size());
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

const glm::vec2 CGUI::GetPrintSize(const std::string format, ...) const {
  std::string text;

  va_list va;
  va_start(va, format);
  text = FormatText(format, va);
  va_end(va);

  glm::vec2 result;
  for(unsigned i = 0; i < text.length(); i++) {
    const CFontChar& fontChar = this->GetChar(text[i]);

    result.y = glm::max(fontChar.m_Size.y, result.y);
    result.x += fontChar.m_Adv;
  }
  result.x += 4.0f;

  return result;
}

void CGUI::RenderQuad(const glm::vec2 & pos, const glm::vec2 & size, 
                      const glm::vec4 & color, const Uint32 texId,
                      const glm::vec2 & texPos, const glm::vec2& texSize) {
  glm::vec2 vert[4];
  vert[0] = pos;
  vert[1] = pos + glm::vec2(size.x, 0.0f);
  vert[2] = pos + size;
  vert[3] = pos + glm::vec2(0.0f, size.y);

  glm::vec2 texc[4];
  texc[0] = texPos + glm::vec2(0.0f, texSize.y);
  texc[1] = texPos + texSize;
  texc[2] = texPos + glm::vec2(texSize.x, 0.0f);
  texc[3] = texPos;

  Uint16 ind[6];
  ind[0] = 0;
  ind[1] = 1;
  ind[2] = 2;
  ind[3] = 0;
  ind[4] = 2;
  ind[5] = 3;

  glLoadMatrixf(glm::value_ptr(m_Transform));
  glColor4fv(glm::value_ptr(color));

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vert);
  glTexCoordPointer(2, GL_FLOAT, 0, texc);

  if(texId != 0) {
    glBindTexture(GL_TEXTURE_2D, texId);
  }
  else
    glDisable(GL_TEXTURE_2D);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, ind);

  if(texId != 0) {
    glBindTexture(GL_TEXTURE_2D, m_FontTexture);
  }
  else
    glEnable(GL_TEXTURE_2D);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void CGUI::RenderQuadLines(const glm::vec2 & pos, const glm::vec2 & size, const glm::vec4 & color) {
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

  glLoadMatrixf(glm::value_ptr(m_Transform));
  glColor4fv(glm::value_ptr(color));

  glDisable(GL_TEXTURE_2D);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(2, GL_FLOAT, 0, vert);

  glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, ind);

  glDisableClientState(GL_VERTEX_ARRAY);
  glEnable(GL_TEXTURE_2D);
}

void CGUI::RenderQuadFullScreen(const glm::vec2& size, const glm::vec4& color, 
                                const Uint32 texId, const glm::vec2& texPos, const glm::vec2& texSize) {
  RenderQuad(glm::vec2(0.0f), size, color, texId, texPos, texSize);
}

void CGUI::RenderProgressBar(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color, const float progress) {
  RenderQuadLines(pos, size, color);

  float sizeX = size.x * (progress / 100.0f);

  RenderQuad(pos, glm::vec2(sizeX, size.y), color);
}

std::string CGUI::Format(const std::string format, ...) {
  va_list ap;
  va_start(ap, format);
  std::string text = FormatText(format, ap);
  va_end(ap);
  return text;
}

const CGUI::CFontChar & CGUI::GetChar(const char charCode) const {
  Uint8 code = (Uint8)charCode - 32;

  return this->m_CharList.at(code);
}

const std::string CGUI::FormatText(const std::string & format, va_list va) {
  std::string text;
  int len = _vscprintf(format.c_str(), va);
  text.resize(len);
  vsprintf_s(&text[0], len + 1, format.c_str(), va);
  return text;
}

//==========================================================

