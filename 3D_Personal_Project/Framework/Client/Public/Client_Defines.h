#pragma once

namespace Client 
{
	static const unsigned int	g_iWinSizeX = 1280;
	static const unsigned int	g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC,LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_TOOL, LEVEL_END };

	enum AXIS {AXIS_X,AXIS_Y,AXIS_Z,AXIS_END};

	enum SHADER_TBN {TBN_TERRAIN,TBN_DTERRAIN,TBN_MODEL,TBN_END};

	enum COLLIDET_LAYER {COL_PLAYER, COL_MONSTER, COL_PLAYER_BULLET, COL_MONSTER_BULLET,COL_END};
}


#include "ImGuizmo/ImGuizmo.h"

#include "Client_Macro.h"
using namespace Client;

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
