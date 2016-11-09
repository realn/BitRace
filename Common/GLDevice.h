#pragma once

#include "Log.h"

#define _USE_MATH_DEFINES
#define WIN32_LEAN_AND_MEAN
#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GLU.h>
#include <windows.h>

#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "glew32s.lib" )

class CGLDevice
{
protected:
	HWND	Handle;
	HDC		hDC;
	HGLRC	hRC;

public:
	CGLDevice()
	{
		Handle = NULL;
		hDC = NULL;
		hRC = NULL;
	};
	void Free()
	{
		DisableGL();
	};

	bool EnableGL(HWND WinHandle, PIXELFORMATDESCRIPTOR* pf)
	{
		if( Handle != NULL )
			return false;

		Handle = WinHandle;

		Log( "Initializing OpenGL..." );

#ifdef SUPER_DEBUG
		Log( "Geting DC from window handle..." );
#endif
		if( ( hDC = GetDC( Handle ) ) == NULL )
		{
			Log_Error( "ERROR: Can't get DC from window handle" );
			Handle = NULL;
			return false;
		}

#ifdef SUPER_DEBUG
		Log( "Searching pixel format..." );
#endif
		int format = ChoosePixelFormat( hDC, pf );
		if( format == 0 )
		{
			Log_Error( "ERROR: Can't find pixel format!" );
			Free();
			return false;
		}

#ifdef SUPER_DEBUG
		Log( "Seting pixel format..." );
#endif
		if( !SetPixelFormat( hDC, format, pf ) )
		{
			Log_Error( "ERROR: Can't set pixel format. Pixel format nr: %d", format );
			Free();
			return false;
		}

#ifdef SUPER_DEBUG
		Log( "Creating GL Context..." );
#endif
		if( ( hRC = wglCreateContext( hDC ) ) == NULL )
		{
			Log_Error(  "ERROR: Can't create gl context!" );
			ReleaseDC( Handle, hDC );
			hDC = NULL;
			Handle = NULL;
			return false;
		}

		wglMakeCurrent( hDC, hRC );

		//GLenum err = glewInit();
		//if( err != GLEW_OK )
		//{
		//	Log( "Can't initialize GLEW, got error: %s", (char*)glewGetErrorString( err ) );
		//}

		Log( "OpenGL inited!" );

		return true;
	}

	void DisableGL()
	{
		if( hRC != NULL )
		{
			wglMakeCurrent( NULL, NULL );
			wglDeleteContext( hRC );
			hRC = NULL;
		}
		if( Handle != NULL )
		{
			if( hDC != NULL )
			{
				ReleaseDC( Handle, hDC );
				hDC = NULL;
			}
			Handle = NULL;
		}
	}

	void Activate()
	{
		if( hRC == NULL || hDC == NULL )
			return;

		wglMakeCurrent( hDC, hRC );
	}

	void Swap()
	{
		if( hDC == NULL )
			return;

		SwapBuffers( hDC );
	}

	bool IsEnabled()
	{
		if( Handle != NULL && hDC != NULL && hRC != NULL )
			return true;
		return false;
	}
	static char* GetGLError( GLenum err )
	{
		switch(err)
		{
		case GL_NO_ERROR :			return "GL_NO_ERROR";
		case GL_INVALID_ENUM :		return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE :		return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION : return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW :	return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW :	return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY :		return "GL_OUT_OF_MEMORY";
		};
		return "Unknown error";
	}

	static void CheckOpenGLErrors( char* file, int line )
	{
		unsigned int err = GL_NO_ERROR;
		while( (err = glGetError()) != GL_NO_ERROR )
		{
			Log( "OGL ERROR: In file %s on line %u was detected error: %s", file, line, GetGLError( err ) );
			err = GL_NO_ERROR;
		}
	}
};

#define CHECK_GL_ERRORS() CGLDevice::CheckOpenGLErrors( __FILE__, __LINE__ )