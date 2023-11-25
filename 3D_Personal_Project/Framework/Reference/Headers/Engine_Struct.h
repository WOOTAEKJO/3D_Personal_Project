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
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Tanget_Binormal_Normal
	{
		XMFLOAT3	fPosition;
		XMFLOAT3	fTangent;	// ���� ���� : ���� ǥ���� ����.		ź��Ʈ ���� x ��
		XMFLOAT3	fBinormal; // ������ ���� : ������ ������ ������ ���, �븻�� ������ ����.	ź��Ʈ ���� y ��
		XMFLOAT3	fNormal;	// ���� ���� : �鿡�� ������ ����.		ź��Ʈ ���� z ��
		XMFLOAT2	fTexCoord;

		static const unsigned int	iElementsNum = 5;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
	}VTXTBN;

}