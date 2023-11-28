#pragma once

namespace Client 
{
	static const unsigned int	g_iWinSizeX = 1280;
	static const unsigned int	g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC,LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_TOOL, LEVEL_END };
}

using namespace Client;

extern HWND g_hWnd;
extern HINSTANCE g_hInst;

//#ifdef _DEBUG
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//#ifndef DBG_NEW 
//
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 
//
//#endif
//
//#endif // _DEBUG