#pragma once


namespace Converter
{
	static const unsigned int	g_iWinSizeX = 1280;
	static const unsigned int	g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVET_CONVERTER, LEVEL_LOADING, LEVEL_END };

	enum AXIS { AXIS_X, AXIS_Y, AXIS_Z, AXIS_END };

	enum SHADER_TBN { TBN_TERRAIN, TBN_DTERRAIN, TBN_MODEL, TBN_END };
}

#include "Converter_Macro.h"
using namespace Converter;

extern HWND g_hWnd;
extern HINSTANCE g_hInst;