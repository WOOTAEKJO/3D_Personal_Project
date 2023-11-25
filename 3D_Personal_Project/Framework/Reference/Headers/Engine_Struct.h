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

	typedef struct tagInt32
	{
		tagInt32()
			:iX(0), iY(0), iZ(0) {}
		tagInt32(unsigned int ix, unsigned int iy, unsigned int iz)
			: iX(ix), iY(iy), iZ(iz) {}

		unsigned int iX, iY, iZ;

	}_uint3;

	typedef struct tagRay
	{
		XMFLOAT3		vOrigin;
		XMFLOAT3		vDir;
		float			fLength;

	}RAY;

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
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Tanget_Binormal_Normal
	{
		XMFLOAT3	fPosition;
		XMFLOAT3	fTangent;	// 접선 벡터 : 정점 표면의 정보.		탄젠트 공간 x 축
		XMFLOAT3	fBinormal; // 종법선 벡터 : 접선과 법선의 외적된 결과, 노말의 수직인 벡터.	탄젠트 공간 y 축
		XMFLOAT3	fNormal;	// 법선 벡터 : 면에서 수직인 벡터.		탄젠트 공간 z 축
		XMFLOAT2	fTexCoord;

		static const unsigned int	iElementsNum = 5;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
	}VTXTBN;

}