#include "..\Public\VIBuffer_Particle_Point.h"
#include "GameInstance.h"

CVIBuffer_Particle_Point::CVIBuffer_Particle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer_Instancing(pDevice, pContext)
{
}

CVIBuffer_Particle_Point::CVIBuffer_Particle_Point(const CVIBuffer_Particle_Point& rhs)
	:CVIBuffer_Instancing(rhs)
{
}

HRESULT CVIBuffer_Particle_Point::Initialize_ProtoType()
{

	/*if (FAILED(Init_Buffer()))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	INSTANCING_DESC* pPoint_Desc = (INSTANCING_DESC*)pArg;

	m_Instancing_Desc.vCenter = pPoint_Desc->vCenter;
	m_Instancing_Desc.fLifeTime = pPoint_Desc->fLifeTime;
	m_Instancing_Desc.fRange = pPoint_Desc->fRange;

	m_Instancing_Desc.fSpeed[0] = pPoint_Desc->fSpeed[0];
	m_Instancing_Desc.fSpeed[1] = pPoint_Desc->fSpeed[1];
	m_Instancing_Desc.fSpeed[2] = pPoint_Desc->fSpeed[2];
	m_Instancing_Desc.fPowerSpeed = pPoint_Desc->fPowerSpeed;
	// 스피드와 스피드 가속도 파워

	m_Instancing_Desc.fScale = pPoint_Desc->fScale;
	m_Instancing_Desc.fScaleControl = pPoint_Desc->fScaleControl;
	// 크기 값과 크기 속도 컨트롤 값

	m_Instancing_Desc.vColor = pPoint_Desc->vColor;
	// 색깔

	m_Instancing_Desc.vDir = pPoint_Desc->vDir; // 생성할 때 사용하는 방향 벡터
	m_Instancing_Desc.vRunDir = pPoint_Desc->vRunDir; // 실시간으로 움직일 때 사용하는 방향 벡터

	m_Instancing_Desc.fRotation[0] = pPoint_Desc->fRotation[0];
	m_Instancing_Desc.fRotation[1] = pPoint_Desc->fRotation[1];
	m_Instancing_Desc.fRotation[2] = pPoint_Desc->fRotation[2];
	// 생성 회전 xyz 랜덤 값

	m_Instancing_Desc.fRunRotation[0] = pPoint_Desc->fRunRotation[0];
	m_Instancing_Desc.fRunRotation[1] = pPoint_Desc->fRunRotation[1];
	m_Instancing_Desc.fRunRotation[2] = pPoint_Desc->fRunRotation[2];
	// 실시간 회전 xyz 랜덤 값

	m_Instancing_Desc.bLoop = pPoint_Desc->bLoop;
	// 무한 루프를 돌것인지

	m_Instancing_Desc.iInstanceNum = pPoint_Desc->iInstanceNum;
	// 인스턴스 갯수

	m_Instancing_Desc.eColorType = pPoint_Desc->eColorType;
	
	if (FAILED(Init_Buffer())) return E_FAIL;
	if (FAILED(__super::Init_InstanceBuffer())) return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Save_Particle(const _char* strFilePath)
{
	ofstream fout;

	fout.open(strFilePath, std::ofstream::binary);

	if (fout.is_open())
	{
		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.iInstanceNum), sizeof(_int));
		// 인스턴스 갯수
		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.vCenter), sizeof(_float3));
		// 센터 값
		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fRange), sizeof(_float));
		// 범위 값

		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
		{
			fout.write(reinterpret_cast<const char*>(&m_pSpeeds[i]), sizeof(_float3));
		}
		// 각각 스피드 값

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fPowerSpeed), sizeof(_float3));
		// 스피드 파워 값

		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
		{
			fout.write(reinterpret_cast<const char*>(&m_pScale[i]), sizeof(_float));
		}
		// 각각 크기 값

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fScaleControl), sizeof(_float));
		// 크기 컨트롤 값

		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
		{
			fout.write(reinterpret_cast<const char*>(&m_pLifeTime[i]), sizeof(_float));
		}
		// 각각 라이프타임 값

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.vColor), sizeof(_float4));
		// 색상

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.vRunDir), sizeof(_float3));
		// 실시간 방향 값

		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
		{
			fout.write(reinterpret_cast<const char*>(&m_pRunRotation[i]), sizeof(_float3));
		}
		// 실시간 회전 값

		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
		{
			fout.write(reinterpret_cast<const char*>(&m_pPos[i]), sizeof(_float4));
		}
		// 처음 위치 값

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.bLoop), sizeof(_bool));
		// 무한루프 판단

		string strTag = CUtility_String::WString_To_string(m_Instancing_Desc.strTextureTag);
		size_t istrSize = strTag.size();
		fout.write(reinterpret_cast<const char*>(&istrSize), sizeof(size_t));
		fout.write(strTag.c_str(), istrSize);
		// 텍스쳐 테그

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.eColorType), sizeof(m_Instancing_Desc.eColorType));
		// 텍스쳐 컬러 타입

	}
	else
		return E_FAIL;

	fout.close();

	return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Load_Particle(const _char* strFilePath)
{
	ifstream fIn;

	fIn.open(strFilePath, std::ios::binary);

	if (fIn.is_open())
	{
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.iInstanceNum), sizeof(_int));
		// 인스턴스 갯수
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.vCenter), sizeof(_float3));
		//센터
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fRange), sizeof(_float));
		//범위
		for(_uint i = 0;i< m_Instancing_Desc.iInstanceNum;i++)
			fIn.read(reinterpret_cast<char*>(&m_pSpeeds[i]), sizeof(_float3));
		// 각각의 스피드
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fPowerSpeed), sizeof(_float3));
		// 스피드 파워
		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
			fIn.read(reinterpret_cast<char*>(&m_pScale[i]), sizeof(_float));
		// 각각의 크기
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fScaleControl), sizeof(_float));
		// 크기 컨트롤 값
		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
			fIn.read(reinterpret_cast<char*>(&m_pLifeTime[i]), sizeof(_float));
		// 각각의 라이프 타임
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.vColor), sizeof(_float4));
		// 색상
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.vRunDir), sizeof(_float3));
		// 실시간 방향
		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
			fIn.read(reinterpret_cast<char*>(&m_pRunRotation[i]), sizeof(_float3));
		// 각각의 실시간 회전 각도
		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
			fIn.read(reinterpret_cast<char*>(&m_pPos[i]), sizeof(_float4));
		// 각각의 처음 위치 값
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.bLoop), sizeof(_bool));
		// 무한루프 판단
		size_t TagSize;
		fIn.read(reinterpret_cast<char*>(&TagSize), sizeof(size_t));
		string strTag;
		strTag.resize(TagSize);
		fIn.read(&strTag[0], TagSize);
		m_Instancing_Desc.strTextureTag = CUtility_String::string_To_Wstring(strTag);
		// 텍스쳐 테그 값
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.eColorType), sizeof(m_Instancing_Desc.eColorType));
		// 텍스쳐 컬러 타입
	}
	else
		return E_FAIL;

	fIn.close();

	return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Init_Buffer()
{
	m_eInstanceType = INSTANCING_TYPE::TYPE_PARTICLE;

	m_iVertexBuffersNum = 2;
	m_iVertexNum = 1;
	m_iVertexStride = sizeof(VTXPOINT);

	//m_iInstanceNum = iInstanceNum;
	m_iInstanceNum = m_Instancing_Desc.iInstanceNum;
	m_iInstanceStride = sizeof(VTXINSTANCING);
	m_iIndexCountPerInstance = 1;

	m_pSpeeds = new _float3[m_iInstanceNum];
	m_pSpeedAcc = new _float3[m_iInstanceNum];
	ZeroMemory(m_pSpeedAcc, sizeof(_float3) * m_iInstanceNum);

	m_pLifeTime = new _float[m_iInstanceNum];
	m_pTimeAcc = new _float[m_iInstanceNum];
	ZeroMemory(m_pTimeAcc, sizeof(_float) * m_iInstanceNum);
	
	m_pScale = new _float[m_iInstanceNum];
	m_pScaleTimeAcc = new _float[m_iInstanceNum];
	ZeroMemory(m_pScaleTimeAcc, sizeof(_float) * m_iInstanceNum);

	m_pPos = new _float4[m_iInstanceNum];

	m_pRotation = new _float3[m_iInstanceNum];
	m_pRunRotation = new _float3[m_iInstanceNum];

	m_iIndexNum = m_iInstanceNum;
	m_iIndexStride = 2;
	m_eIndexForMat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// 동적배열의 크기
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;					// 정적버퍼인지 동적버퍼인지 설정/ 지금은 정적
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 종류 설정
	m_Buffer_Desc.CPUAccessFlags = 0;							// 동적만 해당
	m_Buffer_Desc.MiscFlags = 0;								// 동적만 해당
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// 정점 하나의 크기를 집어 넣음

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	VTXPOINT* pVerpostex = new VTXPOINT;		// 버텍스 버퍼 안에 들어 갈 값들을 설정해줌

	pVerpostex->fPosition = _float3(0.f, 0.f, 0.f);
	pVerpostex->fPSize = _float2(1.f, 1.f);

	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pVB)))		// 내가 할당한 값들을 버퍼가 할당한 공간에 복사하여 생성
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// 내가 할당한 것은 버퍼가 이미 사용했기에 필요 없으니 해제

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iIndexStride * m_iIndexNum;
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_Buffer_Desc.CPUAccessFlags = 0;
	m_Buffer_Desc.MiscFlags = 0;
	m_Buffer_Desc.StructureByteStride = 0;

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	_ushort* pIndex = new _ushort[m_iIndexNum];

	ZeroMemory(pIndex, sizeof(_ushort) * m_iIndexNum);

	m_SubResource_Data.pSysMem = pIndex;

	if (FAILED(Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndex);

	return S_OK;
}

CVIBuffer_Particle_Point* CVIBuffer_Particle_Point::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Particle_Point* pInstance = new CVIBuffer_Particle_Point(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CVIBuffer_Particle_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Particle_Point::Clone(void* pArg)
{
	CVIBuffer_Particle_Point* pInstance = new CVIBuffer_Particle_Point(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CVIBuffer_Particle_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Particle_Point::Free()
{
	__super::Free();
}
