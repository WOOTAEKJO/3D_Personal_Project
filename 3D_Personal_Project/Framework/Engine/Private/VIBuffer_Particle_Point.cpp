#include "..\Public\VIBuffer_Particle_Point.h"

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

	INSTANCING_PARTICLEPOINT_DESC* pPoint_Desc = (INSTANCING_PARTICLEPOINT_DESC*)pArg;

	m_Instancing_Desc.vCenter = pPoint_Desc->vCenter;
	m_Instancing_Desc.fLifeTime = pPoint_Desc->fLifeTime;
	m_Instancing_Desc.fRange = pPoint_Desc->fRange;
	m_Instancing_Desc.fSpeed = pPoint_Desc->fSpeed;
	m_Instancing_Desc.fScale = pPoint_Desc->fScale;
	m_Instancing_Desc.vColor = pPoint_Desc->vColor;
	m_Instancing_Desc.vDir = pPoint_Desc->vDir;
	m_Instancing_Desc.vRotation = pPoint_Desc->vRotation;

	m_Instancing_Desc.vRunDir = pPoint_Desc->vRunDir;
	m_Instancing_Desc.vRunRotation = pPoint_Desc->vRunRotation;

	m_ePointDesc.iInstanceNum = pPoint_Desc->iInstanceNum;
	
		
	if (FAILED(Init_Buffer())) return E_FAIL;
	if (FAILED(__super::Init_InstanceBuffer())) return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Init_Buffer()
{
	m_eInstanceType = INSTANCING_TYPE::TYPE_PARTICLE;

	m_iVertexBuffersNum = 2;
	m_iVertexNum = 1;
	m_iVertexStride = sizeof(VTXPOINT);

	//m_iInstanceNum = iInstanceNum;
	m_iInstanceNum =  m_ePointDesc.iInstanceNum;
	m_iInstanceStride = sizeof(VTXINSTANCING);
	m_iIndexCountPerInstance = 1;

	m_pSpeeds = new _float[m_iInstanceNum];
	m_pLifeTime = new _float[m_iInstanceNum];
	m_pRunRotation = new _float[m_iInstanceNum];
	m_pScale = new _float[m_iInstanceNum];
	m_pTimeAcc = new _float[m_iInstanceNum];
	m_pPos = new _float4[m_iInstanceNum];
	m_pRotation = new _float[m_iInstanceNum];

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
