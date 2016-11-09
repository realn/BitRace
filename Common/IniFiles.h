#pragma once

#include "Files.h"
#include <string>
#include <sstream>
#include <vector>

class CIniFile
{
protected:
	std::vector<std::string> Lines;
	std::string FileName;

	std::string CIniFile::GetVar( std::string str )
	{
		size_t pos = str.find( '=' );
		if( pos == std::string::npos )
			return str;
		return str.substr( 0, pos );
	};
	std::string CIniFile::GetValue( std::string str )
	{
		size_t pos = str.find( '=' );
		if( pos == std::string::npos )
			return str;
		return str.substr( pos + 1 );
	};
	int CIniFile::FindSection( std::string str )
	{
		int i;
		for( i = 0; i < int(Lines.size()); i++ )
			if( Lines[i] == "[" + str + "]" )
				return i;
		return -1;
	};
	int CIniFile::FindVar( std::string Section, std::string Var )
	{
		int i, a;
		a = FindSection( Section );
		if( a == Lines.size()-1 )
			return -1;
		for( i = a + 1; i < int(Lines.size()); i++ )
		{
			if( !Lines[i].empty() )
			{
				if( Lines[i][0] == '[' )
					return -1;
				if( GetVar( Lines[i] ) == Var )
					return i;
			}
		}
		return -1;
	};
public:
	CIniFile(){};
	CIniFile(std::string filename)
	{
		Open( filename );
	};
	~CIniFile()
	{
		Close();
	};
	bool CIniFile::Open(std::string filename)
	{
		FileName = filename;

		CFile fp;
		if( !fp.Open( filename, "rt" ) )
			return false;

		std::string str;
		while( !fp.EndOfFile() )
		{
			str = fp.ReadStr( 1000 );
			if( str.length() == 1 && str[0] == '\n' )
				str = "";
			else if( str.length() > 0 && str[str.length()-1] == '\n' )
				str = str.substr( 0, str.length()-1 );
			Lines.push_back( str );
		}
		return true;
	};
	bool CIniFile::Close()
	{
		if( FileName.empty() )
		{
			Lines.clear();
			return false;
		}
		if( Lines.size() == 0 )
		{
			FileName = "";
			return true;
		}
		CFile fp;
		if( !fp.Open( FileName, "wt" ) )
		{
			FileName = "";
			Lines.clear();
			return false;
		}
		size_t i;
		for( i = 0; i < Lines.size(); i++ )
		{
			fp.WriteStr( Lines[i] + "\n" );
		}

		FileName = "";
		Lines.clear();

		return true;
	};
	void CIniFile::WriteBool(std::string Section, std::string Var, bool value)
	{
		if( Lines.size() == 0 || FindSection( Section ) == -1 )
		{
			Lines.push_back( "[" + Section + "]" );
			Lines.push_back( Var + "=" + ( value ? "1" : "0" ) );
			return;
		}
		int a;
		if( ( a = FindVar( Section, Var ) ) != -1 )
		{
			Lines[a] = Var + "=" + ( value ? "1" : "0" );
			return;
		}
		a = FindSection( Section );
		if( a == Lines.size()-1 )
		{
			Lines.push_back( Var + "=" + ( value ? "1" : "0" ) );
			return;
		}
		a++;
		Lines.insert( Lines.begin() + a, Var + "=" + ( value ? "1" : "0" ) );
	};
	void CIniFile::WriteFloat(std::string Section, std::string Var, float value)
	{
		std::stringstream ss;
		ss << value;
		if( Lines.size() == 0 || FindSection( Section ) == -1 )
		{
			Lines.push_back( "[" + Section + "]" );
			Lines.push_back( Var + "=" + ss.str() );
			return;
		}
		int a;
		if( ( a = FindVar( Section, Var ) ) != -1 )
		{
			Lines[a] = Var + "=" + ss.str();
			return;
		}
		a = FindSection( Section );
		if( a == Lines.size()-1 )
		{
			Lines.push_back( Var + "=" + ss.str() );
			return;
		}
		a++;
		Lines.insert( Lines.begin() + a, Var + "=" + ss.str() );
	};
	void CIniFile::WriteInt(std::string Section, std::string Var, int value)
	{
		std::stringstream ss;
		ss << value;
		if( Lines.size() == 0 || FindSection( Section ) == -1 )
		{
			Lines.push_back( "[" + Section + "]" );
			Lines.push_back( Var + "=" + ss.str() );
			return;
		}
		int a;
		if( ( a = FindVar( Section, Var ) ) != -1 )
		{
			Lines[a] = Var + "=" + ss.str();
			return;
		}
		a = FindSection( Section );
		if( a == Lines.size()-1 )
		{
			Lines.push_back( Var + "=" + ss.str() );
			return;
		}
		a++;
		Lines.insert( Lines.begin() + a, Var + "=" + ss.str() );
	};
	void CIniFile::WriteString(std::string Section, std::string Var, std::string value)
	{
		if( Lines.size() == 0 || FindSection( Section ) == -1 )
		{
			Lines.push_back( "[" + Section + "]" );
			Lines.push_back( Var + "=" + value );
			return;
		}
		int a;
		if( ( a = FindVar( Section, Var ) ) != -1 )
		{
			Lines[a] = Var + "=" + value;
			return;
		}
		a = FindSection( Section );
		if( a == Lines.size()-1 )
		{
			Lines.push_back( Var + "=" + value );
			return;
		}
		a++;
		Lines.insert( Lines.begin() + a, Var + "=" + value );
	};
	bool CIniFile::ReadBool(std::string Section, std::string Var, bool DefValue)
	{
		if( Lines.size() == 0 )
			return DefValue;
		int a = FindVar( Section, Var );
		if( a == -1 )
			return DefValue;
		return ( GetValue( Lines[a] ) == "1" ) ? true : false;
	};
	float CIniFile::ReadFloat(std::string Section, std::string Var, float DefValue)
	{
		if( Lines.size() == 0 )
			return DefValue;
		int a = FindVar( Section, Var );
		if( a == -1 )
			return DefValue;
		std::stringstream ss;
		ss << GetValue( Lines[a] );
		ss >> DefValue;
		return DefValue;
	};
	int CIniFile::ReadInt(std::string Section, std::string Var, int DefValue)
	{
		if( Lines.size() == 0 )
			return DefValue;
		int a = FindVar( Section, Var );
		if( a == -1 )
			return DefValue;
		std::stringstream ss;
		ss << GetValue( Lines[a] );
		ss >> DefValue;
		return DefValue;
	};
	std::string CIniFile::ReadString(std::string Section, std::string Var, std::string DefValue)
	{
		if( Lines.size() == 0 )
			return DefValue;
		int a = FindVar( Section, Var );
		if( a == -1 )
			return DefValue;
		return GetValue( Lines[a] );
	};
	bool CIniFile::Opened()
	{
		if( FileName.empty() )
			return false;
		return true;
	};
};