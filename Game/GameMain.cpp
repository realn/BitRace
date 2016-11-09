#include "Game.h"

CGame* CGame::GInst = NULL;

LRESULT CGame::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if( CGame::GInst != NULL )
		if( CGame::GInst->ParseMsg( msg, wParam, lParam ) == 0 )
			return 0;
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

int CGame::MainLoop()
{
	MSG msg;

	while( !m_bShutdown )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if( msg.message == WM_QUIT )
				this->m_bShutdown = true;
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		this->Engine();
		this->Render();
	}

	Free();
	return 0;
}

int CGame::ParseMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	//case WM_ACTIVATE :
	//	if( HIWORD( wParam ) )
	//		WaitMessage();
	//	return 0;
	case WM_DESTROY :
	case WM_CLOSE :
		PostQuitMessage( 0 );
		return 0;
	};

	return -1;
}