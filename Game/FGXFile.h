#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#define FGX_FILEID	"FGX"
#define FGX_VER10	100
#define FGX_VERSION	100
#define FGX_RLE_COMPRESS	0x00000001

struct FGXHEADER {
  char			FILEID[3];
  unsigned int	FILEVERSION;
  unsigned int	IMAGEWIDTH;
  unsigned int	IMAGEHEIGHT;
  unsigned int	IMAGEDEPTH;
  unsigned int	FLAGS;
};

class CFGXFile {
public:
  typedef unsigned char Byte;
  typedef std::vector<Byte> CData;

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
  const unsigned    GetImgDepth() const;

  const CData&  GetData() const;
};