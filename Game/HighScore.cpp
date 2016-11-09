#include "HighScore.h"

struct HSHEADER
{
	char	FILEID[3];
	UINT	FILEVER;
	UINT	SCORECOUNT;
};

#define HSFILEID	"HSF"
#define HSFILEVER	100

//============================================

CHighScore::CScore::CScore() : m_uScore( 0 )
{
	memset( m_acName, 0, 7 );
}

//============================================

CHighScore::CHighScore() : 
m_uTempScore( 0 ),
m_uCurPos( 0 ),
m_uHSS( HSS_STATE1 ),
m_uCharCount( 0 ),
m_strTempName( "" ),
m_strText1( "YOUR SCORE: " ),
m_strText2( "YOU ARE ON POSITION " ),
m_strText3( "ENTER YOUR NAME: " ),
m_fTime( 0.0f ),
m_bCanWrite( false ),
m_bIsEnd( false )
{
	memset( m_bKeyDown, 0, sizeof(bool)*256 );
}

bool CHighScore::LoadScores(std::string strFile)
{
	if( strFile.empty() )
		return false;

	CFile fp;
	if( !fp.Open( strFile ) )
	{
		Log( "Can't find HighScores file %s, NULLing table", strFile.c_str() );
		memset( m_aScore, 0, sizeof(CScore) * 10 );
		return true;
	}
	HSHEADER head;

	fp.Read( &head, sizeof(HSHEADER) );

	if( strncmp( head.FILEID, HSFILEID, 3 ) != 0 || 
		head.FILEVER != HSFILEVER || head.SCORECOUNT != 10 )
	{
		Log( "Invalid file ID - %s isn't a HSF file", strFile.c_str() );
		return false;
	}

	fp.Read( m_aScore, sizeof( CScore ), 10 );

	UINT	uMagicNum = 0;
	UINT	uMagicNumCmp = 0, i, j;
	for( i = 0; i < 10; i++ )
	{
		uMagicNumCmp += m_aScore[i].m_uScore;
		for( j = 0; j < 7; j++ )
			uMagicNumCmp += UINT( m_aScore[i].m_acName[j] );
	}
	fp.Read( &uMagicNum, sizeof(UINT) );

	if( uMagicNum != (UINT)(~uMagicNumCmp) )
	{
		Log( "Invalid Check Sum - file corrupted, NULLing table" );
		memset( m_aScore, 0, sizeof(CScore) * 10 );
		return true;
	}

	return true;
}

bool CHighScore::SaveScores(std::string strFile)
{
	if( strFile.empty() )
		return false;

	CFile fp;
	if( !fp.Open( strFile, "wb" ) )
		return false;

	HSHEADER head;
	strncpy_s( head.FILEID, 4, HSFILEID, 3 );
	head.FILEVER = HSFILEVER;
	head.SCORECOUNT = 10;

	fp.Write( &head, sizeof(HSHEADER) );
	fp.Write( m_aScore, sizeof(CScore), 10 );

	UINT uMagicNum = 0, i, j;
	for( i = 0; i < 10; i++ )
	{
		uMagicNum += m_aScore[i].m_uScore;
		for( j = 0; j < 7; j++ )
			uMagicNum += UINT( m_aScore[i].m_acName[j] );
	}
	uMagicNum = UINT( ~uMagicNum );
	fp.Write( &uMagicNum, sizeof(UINT) );

	return true;
}

void CHighScore::SetTempScore(UINT uScore)
{
	m_uTempScore = uScore;
	m_uCurPos = 0;
	m_uHSS = HSS_STATE1;
	m_uHSPos = 0;
	m_uCharCount = 0;
	m_bCanWrite = false;
	m_bIsEnd = false;
	m_fTime = 0.0f;
	m_strTempName = "";
	m_strText1 = "YOUR SCORE: ";
	m_strText2 = "YOU ARE ON POSITION ";
	m_strText3 = "ENTER YOUR NAME: ";
	memset( m_bKeyDown, 0, sizeof(bool) * 256 );
}

void CHighScore::Engine(BYTE *Key, float fDT)
{
	UINT i;
	for( i = 0; i < 256; i++ )
	{
		if( Key[i] & 0x80 )
		{
			if( m_bKeyDown[i] )
				continue;
			else 
			{
				m_bKeyDown[i] = true;
				ParseKey( i );
				continue;
			}
		}
		else if( m_bKeyDown[i] )
			m_bKeyDown[i] = false;
	}

	std::stringstream ss;
	switch( m_uHSS )
	{
	case HSS_STATE1 :
		m_fTime += 1.0f * fDT;
		if( m_fTime > 0.1f )
		{
			if( m_uCharCount < m_strText1.length() )
				m_uCharCount++;
			else 
			{
				m_uHSS = HSS_STATE2;
				m_uCharCount = 0;
			}
			m_fTime = 0.0f;
		}
		break;

	case HSS_STATE2 :
		m_fTime += float(m_uTempScore) * 0.3f * fDT;
		if( m_fTime >= float(m_uTempScore) )
		{
			m_fTime = 0.0f;
			ss << m_uTempScore;
			m_strText1 += ss.str();
			m_uHSS = HSS_STATE3;
		}
		break;

	case HSS_STATE3 :
		m_fTime += 1.0f * fDT;
		if( m_fTime > 2.0f )
		{
			if( CheckScore() )
			{
				ss << (m_uHSPos+1);
				switch( m_uHSPos )
				{
				case 0 :
					m_strText2 = "OMG! YOU ARE ON FIRST PLACE!"; break; 
				case 1 :
					m_strText2 = "WONDERFUL! YOU ARE ON SECOND PLACE!"; break;
				case 2 :
					m_strText2 = "GOOD. YOU ARE ON THIRD PLACE."; break;
				default:
					m_strText2 = "YOU ARE ON PLACE: " + ss.str(); break;
				};
				m_bCanWrite = true;
			}
			else 
			{
				m_strText2 = "SORRY, YOU AREN'T ON HIGH SCORES...";
				m_bCanWrite = false;
			}
			m_fTime = 0.0f;
			m_uHSS = HSS_STATE4;
		}
		break;

	case HSS_STATE4 :
		m_fTime += 1.0f * fDT;
		if( m_fTime > 0.1f )
		{
			if( m_uCharCount < m_strText2.length() )
				m_uCharCount++;
			else 
			{
				m_uHSS = HSS_STATE5;
				m_uCharCount = 0;
			}
			m_fTime = 0.0f;
		}
		break;

	case HSS_STATE5 :
		m_fTime += 1.0f * fDT;
		if( m_fTime > 2.0f )
		{
			m_fTime = 0.0f;
			if( m_bCanWrite )
				m_uHSS = HSS_STATE6;
			else m_bIsEnd = true;
		}
		break;

	case HSS_STATE6 :
		m_fTime += 1.0f * fDT;
		if( m_fTime > 0.1f )
		{
			if( m_uCharCount < m_strText3.length() )
				m_uCharCount++;
			else 
			{
				m_uHSS = HSS_STATE7;
				m_uCharCount = 0;
			}
			m_fTime = 0.0f;
		}
		break;

	case HSS_STATE7 :
		break;

	case HSS_STATE8 :
		m_bIsEnd = true;
		m_uHSS = HSS_STATE1;
		break;
	};
}

void CHighScore::RenderGUI(CGUI *GUI)
{
	std::stringstream ss;
	switch( m_uHSS )
	{
	case HSS_STATE1 :
		glColor3f( 0.0f, 1.0f, 0.0f );
		GUI->Print( 200.0f, 200.0f, m_strText1.substr( 0, m_uCharCount ) + "_" );
		break;

	case HSS_STATE2 :
		ss << UINT( floor(m_fTime) );
		glColor3f( 0.0f, 1.0f, 0.0f );
		GUI->Print( 200.0f, 200.0f, m_strText1 + ss.str() + "_" );
		break;

	case HSS_STATE3 :
		glColor3f( 0.0f, 1.0f, 0.0f );
		GUI->Print( 200.0f, 200.0f, m_strText1 );
		break;

	case HSS_STATE4 :
		glColor3f( 0.0f, 1.0f, 0.0f );
		GUI->Print( 200.0f, 200.0f, m_strText1 );
		GUI->Print( 200.0f, 220.0f, m_strText2.substr( 0, m_uCharCount ) + "_" );
		break;

	case HSS_STATE5 :
		glColor3f( 0.0f, 1.0f, 0.0f );
		GUI->Print( 200.0f, 200.0f, m_strText1 );
		GUI->Print( 200.0f, 220.0f, m_strText2 );
		break;

	case HSS_STATE6 :
		glColor3f( 0.0f, 1.0f, 0.0f );
		GUI->Print( 200.0f, 200.0f, m_strText1 );
		GUI->Print( 200.0f, 220.0f, m_strText2 );
		GUI->Print( 200.0f, 240.0f, m_strText3.substr( 0, m_uCharCount ) + "_" );
		break;

	case HSS_STATE7 :
		glColor3f( 0.0f, 1.0f, 0.0f );
		GUI->Print( 200.0f, 200.0f, m_strText1 );
		GUI->Print( 200.0f, 220.0f, m_strText2 );
		GUI->Print( 200.0f, 240.0f, m_strText3 + m_strTempName + "_" );
		break;
	}
}

bool CHighScore::IsEnded()
{
	return m_bIsEnd;
}

void CHighScore::ParseKey(UINT Key)
{
	if( !m_bCanWrite || m_uHSS != HSS_STATE7 )
		return;

	if( Key == DIK_RETURN && m_strTempName.length() > 0 )
	{
		strncpy_s( m_aScore[m_uHSPos].m_acName, 7, m_strTempName.c_str(), 6 );
		m_uHSS = HSS_STATE8;
		m_bCanWrite = false;
		return;
	}
	if( Key == DIK_BACK )
	{
		if( m_strTempName.length() > 0 ) 
			m_strTempName = m_strTempName.substr( 0, m_strTempName.length()-1 );
	}

	if( m_strTempName.length() < 6 )
	{
		switch( Key )
		{
		case DIK_0 : m_strTempName += "0"; break;
		case DIK_1 : m_strTempName += "1"; break;
		case DIK_2 : m_strTempName += "2"; break;
		case DIK_3 : m_strTempName += "3"; break;
		case DIK_4 : m_strTempName += "4"; break;
		case DIK_5 : m_strTempName += "5"; break;
		case DIK_6 : m_strTempName += "6"; break;
		case DIK_7 : m_strTempName += "7"; break;
		case DIK_8 : m_strTempName += "8"; break;
		case DIK_9 : m_strTempName += "9"; break;

		case DIK_A : m_strTempName += "A"; break;
		case DIK_B : m_strTempName += "B"; break;
		case DIK_C : m_strTempName += "C"; break;
		case DIK_D : m_strTempName += "D"; break;
		case DIK_E : m_strTempName += "E"; break;
		case DIK_F : m_strTempName += "F"; break;
		case DIK_G : m_strTempName += "G"; break;
		case DIK_H : m_strTempName += "H"; break;
		case DIK_I : m_strTempName += "I"; break;
		case DIK_J : m_strTempName += "J"; break;
		case DIK_K : m_strTempName += "K"; break;
		case DIK_L : m_strTempName += "L"; break;
		case DIK_M : m_strTempName += "M"; break;
		case DIK_N : m_strTempName += "N"; break;
		case DIK_O : m_strTempName += "O"; break;
		case DIK_P : m_strTempName += "P"; break;
		case DIK_Q : m_strTempName += "Q"; break;
		case DIK_R : m_strTempName += "R"; break;
		case DIK_S : m_strTempName += "S"; break;
		case DIK_T : m_strTempName += "T"; break;
		case DIK_U : m_strTempName += "U"; break;
		case DIK_V : m_strTempName += "V"; break;
		case DIK_W : m_strTempName += "W"; break;
		case DIK_X : m_strTempName += "X"; break;
		case DIK_Y : m_strTempName += "Y"; break;
		case DIK_Z : m_strTempName += "Z"; break;

		case DIK_SPACE : m_strTempName += " "; break;
		case DIK_MINUS : m_strTempName += "-"; break;
		}
	}
}

bool CHighScore::CheckScore()
{
	if( m_uTempScore == 0 )
		return false;

	int i;
	bool ok = false;
	for( i = 0; i < 10; i++ )
	{
		if( m_uTempScore > m_aScore[i].m_uScore )
		{
			m_uHSPos = UINT(i);
			ok = true;
			break;
		}
	}
	if( !ok )
		return false;

	for( i = 8; i >= int(m_uHSPos); i-- )
	{
		strncpy_s( m_aScore[i+1].m_acName, 7, m_aScore[i].m_acName, 6 );
		m_aScore[i+1].m_uScore = m_aScore[i].m_uScore;
	}
	memset( m_aScore[m_uHSPos].m_acName, 0, 7 );
	m_aScore[m_uHSPos].m_uScore = m_uTempScore;
	return true;
}

std::string CHighScore::GetName(UINT uIndex)
{
	if( uIndex >= 10 )
		return "";
	return m_aScore[uIndex].m_acName;
}

UINT CHighScore::GetScore(UINT uIndex)
{
	if( uIndex >= 10 )
		return 0;
	return m_aScore[uIndex].m_uScore;
}

void CHighScore::ResetAllScores()
{
	memset( m_aScore, 0, sizeof( CScore ) * 10 );
}