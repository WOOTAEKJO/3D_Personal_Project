#pragma once

namespace Engine
{
	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		WINMODE			eWinMode;			// â���
		HWND			hWnd;				// �ڵ�����
		unsigned int	iBackBufferSizeX;	// ����� x ������
		unsigned int	iBackBufferSizeY;	// ����� y ������

	}GRAPHIC_DESC;
}