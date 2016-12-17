#pragma once

#include <SDL_types.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#define FGX_FILEID	"FGX"
#define FGX_VER10	100
#define FGX_VERSION	100
#define FGX_RLE_COMPRESS	0x00000001

struct FGXHEADER {
  char		FILEID[3];
  Uint32	FILEVERSION;
  Uint32	IMAGEWIDTH;
  Uint32	IMAGEHEIGHT;
  Uint32	IMAGEDEPTH;
  Uint32	FLAGS;
};

class CFGXFile {
public:
  typedef std::vector<Uint8> CData;

private:
  FGXHEADER m_Header;
  CData     m_Data;

public:
  CFGXFile();
  ~CFGXFile();

  bool  Load(const std::string filename);
  void  Free();

  bool  IsLoaded() const;
  bool  IsValid() const;

  const glm::ivec2  GetSize() const;
  const Uint32    GetImgDepth() const;

  const CData&  GetData() const;
};
