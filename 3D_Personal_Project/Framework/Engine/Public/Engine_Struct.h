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

	typedef struct tagUnsigned_Int_2
	{
		tagUnsigned_Int_2()
			:iX(0), iY(0) {}
		tagUnsigned_Int_2(unsigned int ix, unsigned int iy)
			: iX(ix), iY(iy) {}

		unsigned int iX, iY;

	}_uint2;

	typedef struct tagInt32
	{
		tagInt32()
			:iX(0), iY(0), iZ(0) {}
		tagInt32(unsigned int ix, unsigned int iy, unsigned int iz)
			: iX(ix), iY(iy), iZ(iz) {}

		unsigned int iX, iY, iZ;

	}_uint3;

	typedef struct tagfloat3x3
	{
		_float3 vVertex0;
		_float3 vVertex1;
		_float3 vVertex2;

	}FLOAT3X3;

	typedef struct tagRay
	{
		XMFLOAT3		vOrigin;
		XMFLOAT3		vDir;
		float			fLength;

	}RAY;

	typedef	struct tagKeyFrame
	{
		_float3 vScale;
		_float3 vPosition;
		_float4 vRotation;

		_float fTrackPosition;

	}KEYFRAME;

	typedef	struct tagMaterial_Desc
	{
		class CTexture* pMtrlTexture[TEXTURETYPE::TYPE_UNKNOWN];
	}MATERIAL_DESC;

	typedef struct ENGINE_DLL tagVertex_Position
	{
		XMFLOAT3	fPosition;

		static const unsigned int	iElementsNum = 1;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_TexCoord
	{
		XMFLOAT3	fPosition;
		XMFLOAT2	fTexCoord;

		static const unsigned int	iElementsNum = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
		// ������ ���� ����ü�� ��� �������� �ɼ��� �����ϴ� ����ü �迭
		// ���� ����ü�� ��������� 2���̹Ƿ� �迭�� ������ 2�̴�.
	}VTXPOSTEX;

	typedef struct ENGINE_DLL tagVertex_Point
	{
		XMFLOAT3	fPosition;
		XMFLOAT2	fPSize;

		static const unsigned int	iElementsNum = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
	}VTXPOINT;

	typedef struct ENGINE_DLL tagVertex_Cube
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vTexCoord;

		static const unsigned int	iElementsNum = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
		// ������ ���� ����ü�� ��� �������� �ɼ��� �����ϴ� ����ü �迭
		// ���� ����ü�� ��������� 2���̹Ƿ� �迭�� ������ 2�̴�.
	}VTXCUBE;

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

	typedef struct ENGINE_DLL tagVertex_Mesh
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vTangent;	// ���� ���� : ���� ǥ���� ����.		ź��Ʈ ���� x ��
		XMFLOAT3	vNormal;	// ���� ���� : �鿡�� ������ ����.		ź��Ʈ ���� z ��
		XMFLOAT2	vTexCoord;

		static const unsigned int	iElementsNum = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
	}VTXMESH;

	typedef struct ENGINE_DLL tagVertex_Anim_Mesh
	{
		XMFLOAT3	vPosition;
		XMFLOAT3	vTangent;	// ���� ���� : ���� ǥ���� ����.		ź��Ʈ ���� x ��
		XMFLOAT3	vNormal;	// ���� ���� : �鿡�� ������ ����.		ź��Ʈ ���� z ��
		XMFLOAT2	vTexCoord;
		XMINT4		vBlendIndices;	// ���� �ε����� ����, �ִ� 4������ ���� �����ϴ�.
		XMFLOAT4	vBlendWeights;	// ������ ����ġ�� �����Ѵ�.

		static const unsigned int	iElementsNum = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
	}VTXANIMMESH;

	typedef struct tagInstancing
	{
		_float4 vRight, vUp, vLook, vPos;
		_float4 vColor;

	}VTXINSTANCING;

	typedef struct ENGINE_DLL tagParticle_Rect
	{
		static const unsigned int	iElementsNum = 7;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
	}PARTICLE_RECT;

	typedef struct ENGINE_DLL tagParticle_Point
	{
		static const unsigned int	iElementsNum = 7;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iElementsNum];
	}PARTICLE_POINT;

	typedef struct tagMeshDesc
	{
		string				szName;

		_uint				iMaterialIndex;

		vector<_uint>		vecMeshBoneIndices;

		vector<VTXANIMMESH> vecVerticesAnim;
		vector<VTXMESH>		vecVerticesNonAim;

		vector<_uint3>		vecIndices;

	}MESH;

	typedef struct tagMaterialDesc
	{
		string	vecMaterialPath[TEXTURETYPE::TYPE_UNKNOWN];

	}MATERIAL;

	typedef struct tagBoneDesc
	{
		string			szName;

		_int			iParentIndex;

		_float4x4		matTransformation;
		_float4x4		matOffsetMatrix;

	}BONE;

	typedef struct tagChannelDesc
	{
		string				szName;

		vector<KEYFRAME>	vecKeyFrame;

	}CHANNEL;

	typedef struct tagAnimationDesc
	{
		string		szName;

		_float		fDuration;
		_float		fTicksPerSecond;

		vector<CHANNEL>		vecChannel;

	}ANIMATION;

	typedef struct tagCellDesc
	{
		FLOAT3X3	vPoints;
		_uint		iCellType;

		//_uint3		iNeighborIndexs;

	}CELL;

	typedef struct tagCell2Desc
	{
		FLOAT3X3	vPoints;
		_uint		iCellType;

		_int3		iNeighborIndexs;

	}CELL2;

	typedef struct tagLevelObjectDesc
	{
		string		strLayer;
		string		strTagName;
		string		strModelTag;
		_int		iNaviIndex;
		_float4x4	matWorld;
		
	}LEVELOBJECT_DESC;
}