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
  : mTexture(nullptr) {}

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

void CUIFont::CreateMesh(const cb::string & text, vec3vector & vertices,
                         vec2vector & texcoords, const glm::vec2& charSize) const {
  vertices.resize(4 * text.length());
  texcoords.resize(4 * text.length());

  glm::vec2 verts[] = {
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 0.0f)
  };

  glm::vec2 textPos = glm::vec3(0.0f);
  for(size_t i = 0; i < text.length(); i++) {
    const CUIFont::CChar& fontChar = GetChar(text[i]);
    glm::vec3* vp = &vertices[i * 4];
    glm::vec2* vt = &texcoords[i * 4];

    for(size_t j = 0; j < 4; j++) {
      vp[j] = glm::vec3(verts[j] * fontChar.Size * charSize + textPos, 0.0f);
    }

    glm::vec2 t0 = fontChar.TexPos;
    glm::vec2 t1 = fontChar.TexPos + fontChar.TexSize;

    vt[0] = glm::vec2(t0.x, t1.y);
    vt[1] = t1;
    vt[2] = glm::vec2(t1.x, t0.y);
    vt[3] = t0;

    textPos += fontChar.Adv * charSize;
  }
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

