#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include <CBIO/Defines.h>

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
private:
  FGXHEADER mHeader;
  cb::bytevector mData;

public:
  CFGXFile();
  ~CFGXFile();

  bool  Load(const cb::string filepath);
  void  Free();

  bool  IsLoaded() const;
  bool  IsValid() const;

  const glm::ivec2  GetSize() const;
  const unsigned    GetImgDepth() const;

  const cb::bytevector&  GetData() const;
};
