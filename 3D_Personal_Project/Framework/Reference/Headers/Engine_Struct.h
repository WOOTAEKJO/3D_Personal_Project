#pragma once

namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		WINMODE			eWinMode;			// 창모드
		HWND			hWnd;				// 핸들정보
		unsigned int	iBackBufferSizeX;	// 백버퍼 x 사이즈
		unsigned int	iBackBufferSizeY;	// 백버퍼 y 사이즈

	}GRAPHIC_DESC;
}