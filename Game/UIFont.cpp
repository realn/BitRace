#include "stdafx.h"
#include "UIFont.h"
#include "FileSystem.h"
#include "FGXFile.h"
#include "MeshFunctions.h"
#include "Texture.h"
#include "XmlTypes.h"

class CFontData {
public:
  typedef std::map<Uint16, CUIFont::CChar> CharMapT;

  cb::string Name;
  cb::string TextureFilePath;
  CharMapT CharMap;
};

CUIFont::CChar::CChar() {}

CUIFont::CUIFont() 
  : mTexture(nullptr)
{}

CUIFont::~CUIFont() {
  if(mTexture) {
    delete mTexture;
    mTexture = nullptr;
  }
}

const bool CUIFont::Load(IFileSystem & fs, const cb::string & fontFilePath) {
  CFontData data;
  if(!fs.ReadXml(fontFilePath, L"Font", data)) {
    return false;
  }

  if(!LoadTexture(fs, data.TextureFilePath)) {
    return false;
  }

  mCharMap = data.CharMap;
  return true;
}

const CTexture * CUIFont::GetTexture() const {
  return mTexture;
}

const CUIFont::CChar& CUIFont::GetChar(const wchar_t code) const {
  return mCharMap.at((Uint16)code);
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

const bool CUIFont::LoadTexture(IFileSystem & fs, const cb::string & filepath) {
  CFGXFile imgFile;
  if(!imgFile.Load(filepath)) {
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

  if(mTexture) {
    delete mTexture;
  }
  mTexture = new CTexture(GL_TEXTURE_2D, size, GL_RGBA);
  mTexture->Load(format, imgFile.GetData());
  return true;
}





CUIText::CUIText(const CUIFont& font, const glm::vec2& screenSize, 
                 const glm::vec2& charSize)
  : mFont(font)
  , mCharSize(charSize)
{
  mProjMatrix = glm::ortho(0.0f, screenSize.x, screenSize.y, 0.0f);

  this->mVertex[0] = glm::vec2(0.0f, 1.0f);
  this->mVertex[1] = glm::vec2(1.0f, 1.0f);
  this->mVertex[2] = glm::vec2(1.0f, 0.0f);
  this->mVertex[3] = glm::vec2(0.0f, 0.0f);
}

CUIText::~CUIText() {}

const glm::vec2 CUIText::GetCharSize() const {
  return mCharSize;
}

void CUIText::Bind() const {
  glPushAttrib(GL_ENABLE_BIT);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  mFont.GetTexture()->Bind();
}

void CUIText::UnBind() const {
  mFont.GetTexture()->UnBind();
  glPopAttrib();
}

void CUIText::Render(const glm::vec2& pos, const cb::string& text, const CUITextContext& context) const {
  if(text.empty()) {
    return;
  }

  vec3vector vertList;
  vec2vector texList;

  vertList.resize(4 * text.length());
  texList.resize(4 * text.length());

  glm::vec2 textPos = pos;
  for(size_t i = 0; i < text.length(); i++) {
    const CUIFont::CChar& fontChar = mFont.GetChar(text[i]);
    glm::vec3* vp = &vertList[i * 4];
    glm::vec2* vt = &texList[i * 4];

    for(size_t j = 0; j < 4; j++) {
      vp[j] = glm::vec3(mVertex[j] * mCharSize * fontChar.Size * context.Scale + textPos, 0.0f);
    }

    glm::vec2 t0 = fontChar.TexPos;
    glm::vec2 t1 = fontChar.TexPos + fontChar.TexSize;

    vt[0] = glm::vec2(t0.x, t1.y);
    vt[1] = t1;
    vt[2] = glm::vec2(t1.x, t0.y);
    vt[3] = t0;

    textPos += fontChar.Adv * mCharSize * context.Scale;
  }

  glLoadMatrixf(glm::value_ptr(mProjMatrix));
  glColor4fv(glm::value_ptr(context.Color));
  RenderTexVectorList(GL_QUADS, vertList, texList);
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



static const cb::string XML_FONTCHAR_CODE = L"Code";
static const cb::string XML_FONTCHAR_TEXPOS = L"TexPos";
static const cb::string XML_FONTCHAR_TEXSIZE = L"TexSize";
static const cb::string XML_FONTCHAR_POS = L"Pos";
static const cb::string XML_FONTCHAR_SIZE = L"Size";
static const cb::string XML_FONTCHAR_ADV = L"Adv";

CB_DEFINEXMLREAD(CUIFont::CChar) {
  return
    GetAttribute(XML_FONTCHAR_CODE, mObject.Code) &&
    GetAttribute(XML_FONTCHAR_POS, mObject.Pos) &&
    GetAttribute(XML_FONTCHAR_SIZE, mObject.Size) &&
    GetAttribute(XML_FONTCHAR_TEXPOS, mObject.TexPos) &&
    GetAttribute(XML_FONTCHAR_TEXSIZE, mObject.TexSize) &&
    GetAttribute(XML_FONTCHAR_ADV, mObject.Adv);
}

CB_DEFINEXMLWRITE(CUIFont::CChar) {
  return
    SetAttribute(XML_FONTCHAR_CODE, mObject.Code) &&
    SetAttribute(XML_FONTCHAR_POS, mObject.Pos) &&
    SetAttribute(XML_FONTCHAR_SIZE, mObject.Size) &&
    SetAttribute(XML_FONTCHAR_TEXPOS, mObject.TexPos) &&
    SetAttribute(XML_FONTCHAR_TEXSIZE, mObject.TexSize) &&
    SetAttribute(XML_FONTCHAR_ADV, mObject.Adv);
}

static const cb::string XML_FONT_NAME = L"Name";
static const cb::string XML_FONT_TEXFILENAME = L"Texture";
static const cb::string XML_FONT_CHAR = L"Char";

CB_DEFINEXMLREAD(CFontData) {
  return
    GetAttribute(XML_FONT_NAME, mObject.Name) &&
    GetAttribute(XML_FONT_TEXFILENAME, mObject.TextureFilePath) &&
    GetNodeMap(mObject.CharMap, XML_FONT_CHAR, XML_FONTCHAR_CODE);
}

CB_DEFINEXMLWRITE(CFontData) {
  return
    SetAttribute(XML_FONT_NAME, mObject.Name) &&
    SetAttribute(XML_FONT_TEXFILENAME, mObject.TextureFilePath) &&
    SetNodeMap(mObject.CharMap, XML_FONT_CHAR, XML_FONTCHAR_CODE);
}

