#pragma once

#ifndef FGX_FILEID
#	define FGX_FILEID	"FGX"
#endif
#ifndef FGX_VER10
#	define FGX_VER10	100
#endif
#ifndef FGX_VERSION
#	define FGX_VERSION	100
#endif

struct FGXHEADER
{
	char			FILEID[3];
	unsigned int	FILEVERSION;
	unsigned int	IMAGEWIDTH;
	unsigned int	IMAGEHEIGHT;
	unsigned int	IMAGEDEPTH;
	unsigned int	FLAGS;
};

#define FGX_RLE_COMPRESS	0x00000001