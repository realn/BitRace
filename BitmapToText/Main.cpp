#include <iostream>
#include <vector>
#include "../Common/Files.h"
#include "../Common/FGXFile.h"

int main( int argc, char* argv[] )
{
	std::string infile = "";
	std::string outfile = "";

	std::cin >> infile;
	std::cin >> outfile;

	unsigned char tga[6];
	unsigned char Head[12];

	CFile fp;
	fp.Open( infile );

	fp.Read( Head, 12 );

	unsigned char TGA_com[]		= { 0,0,10,0,0,0,0,0,0,0,0,0 };
	unsigned char TGA_uncom[]	= { 0,0, 2,0,0,0,0,0,0,0,0,0 };

	if( memcmp( Head, TGA_com, 12 ) == 0 )
	{
		std::cout << "TGA file is compressed - this program can't read files like that.";
		fp.Close();
		return 0;
	}
	if( memcmp( Head, TGA_uncom, 12 ) != 0 )
	{
		std::cout << "Unregonized header - this isn't a TGA file.";
		fp.Close();
		return 0;
	}

	fp.Read( tga, 6 );

	unsigned int W = tga[1] * 256 + tga[0];
	unsigned int H = tga[3] * 256 + tga[2];
	unsigned int B = tga[4] / 8;

	std::vector<unsigned char> Data;
	Data.resize( W * H * B );

	fp.Read( &Data[0], 1, W * H * B );
	fp.Close();

	FGXHEADER head;
	memset( &head, 0, sizeof(FGXHEADER) );
	strncpy( head.FILEID, FGX_FILEID, 3 );
	head.FILEVERSION = FGX_VERSION;
	head.IMAGEWIDTH = W;
	head.IMAGEHEIGHT = H;
	head.IMAGEDEPTH = B;
	head.FLAGS = 0;
	
	size_t i;
	unsigned char a;
	for( i = 0; i < head.IMAGEWIDTH * head.IMAGEHEIGHT; ++i )
	{
		a = Data[i * head.IMAGEDEPTH];
		Data[i * head.IMAGEDEPTH] = Data[i * head.IMAGEDEPTH+2];
		Data[i * head.IMAGEDEPTH+2] = a;
	}

	fp.Open( outfile, "wb" );

	fp.Write( &head, sizeof( FGXHEADER ) );
	fp.Write( &Data[0], 1, Data.size() );

	fp.Close();

	Data.clear();

	return 0;
}