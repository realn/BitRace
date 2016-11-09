#pragma once

#include <stdio.h>
#include <io.h>
#include <string>

class CFile
{
protected:
	FILE* fp;

public:
	CFile() : fp( NULL ){}
	CFile(std::string filename, char *flags = "rb" ) : fp(NULL)
	{
		Open( filename, flags );
	}
	~CFile()
	{
		Close();
	}

	bool Open(std::string filename, char *flags = "rb" )
	{
		fopen_s( &fp, filename.c_str(), flags );
		if( fp != NULL )
			return true;
		return false;
	}

	bool Close()
	{
		if( fp == NULL )
			return false;
		if( fclose( fp ) == 0 )
		{
			fp = NULL;
			return true;
		}
		return false;
	}

	unsigned int CFile::Write(void *Data, unsigned int Size, unsigned int Count = 1)
	{
		return (unsigned int)fwrite( Data, Size, Count, fp ) * Size;
	}

	unsigned int CFile::Read(void *Data, unsigned int Size, unsigned int Count = 1)
	{
		return (unsigned int)fread( Data, Size, Count, fp ) * Size;
	}

	unsigned int CFile::WriteStr(std::string str)
	{
		int a = fputs( str.c_str(), fp );
		if( a >= 0 )
			return unsigned int(a);
		return 0;
	}

	unsigned int CFile::WriteStr(std::wstring str)
	{
		int a = fputws( str.c_str(), fp );
		if( a >= 0 )
			return unsigned int(a);
		return 0;
	}

	std::string CFile::ReadStr(unsigned int BufSize)
	{
		if( BufSize == 0 )
			return "";

		char* a;
		std::string t;

		a = new char[BufSize];
		memset( a, 0, BufSize );
		fgets( a, BufSize, fp );
		t = a;
		delete[] a;

		return t;
	}

	std::wstring CFile::ReadWStr(unsigned int BufSize)
	{
		if( BufSize == 0 )
			return L"";

		wchar_t* a;
		std::wstring t;

		a = new wchar_t[BufSize];
		memset( a, 0, BufSize * sizeof(wchar_t) );
		fgetws( a, BufSize, fp );
		t = a;
		delete[] a;

		return t;
	}

	__int64 CFile::Length()
	{
		if( !Opened() )
			return 0;

		return _filelengthi64( _fileno( fp ) );
	}

	bool CFile::Seek(__int64 offset, int origin )
	{
		if( _fseeki64( fp, offset, origin ) == 0 )
			return true;
		return false;
	}

	FILE* CFile::GetPointer()
	{
		return fp;
	}

	bool CFile::Opened()
	{
		if( fp == NULL )
			return false;
		return true;
	}

	bool CFile::EndOfFile()
	{
		if( feof( fp ) )
			return true;
		return false;
	}

	bool CFile::Flush()
	{
		if( fflush( fp ) == 0 )
			return true;
		return false;
	}

	static bool FileExists( std::string filename )
	{
		if( filename.empty() )
			return false;

		CFile fp;
		if( fp.Open( filename ) )
			return true;
		return false;
	}

	static std::string ReadTextFile( std::string filename )
	{
		CFile fp;
		if( !fp.Open( filename, "rt" ) )
			return "";

		__int64 len = fp.Length();

		if( len <= 0 )
			return "";

		char* buf = new char[(size_t)len];
		fp.Read( buf, 1, (unsigned int)len );
		fp.Close();

		std::string ret = buf;
		delete[] buf;

		return ret;
	}
};