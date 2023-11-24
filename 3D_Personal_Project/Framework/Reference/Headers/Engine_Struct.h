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

	typedef struct ENGINE_DLL tagVertex_Position_TexCoord
	{
		XMFLOAT3	fPosition;
		XMFLOAT2	fTexCoord;

		static const unsigned int	iElementsNum = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
		// ������ ���� ����ü�� ��� �������� �ɼ��� �����ϴ� ����ü �迭
		// ���� ����ü�� ��������� 2���̹Ƿ� �迭�� ������ 2�̴�.
	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertex_Position_Normal_TexCoord
	{
		XMFLOAT3	fPosition;
		XMFLOAT3	fNormal;
		XMFLOAT2	fTexCoord;

		static const unsigned int	iElementsNum = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
		// ������ ���� ����ü�� ��� �������� �ɼ��� �����ϴ� ����ü �迭
		// ���� ����ü�� ��������� 2���̹Ƿ� �迭�� ������ 2�̴�.
	}VTXNORTEX;

}