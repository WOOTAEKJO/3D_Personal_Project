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

	typedef struct ENGINE_DLL tagVertex_Position_TexCoord
	{
		XMFLOAT3	fPosition;
		XMFLOAT2	fTexCoord;

		static const unsigned int	iElementsNum = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
		// 정점의 정보 구조체의 멤버 변수마다 옵션을 설정하는 구조체 배열
		// 정점 구조체의 멤버변수는 2개이므로 배열의 갯수도 2이다.
	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_TexCoord
	{
		XMFLOAT3	fPosition;
		XMFLOAT3	fNormal;
		XMFLOAT2	fTexCoord;

		static const unsigned int	iElementsNum = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
		// 정점의 정보 구조체의 멤버 변수마다 옵션을 설정하는 구조체 배열
		// 정점 구조체의 멤버변수는 2개이므로 배열의 갯수도 2이다.
	}VTXNORTEX;

}