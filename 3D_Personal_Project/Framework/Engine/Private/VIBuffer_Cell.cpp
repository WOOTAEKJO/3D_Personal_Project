#include "..\Public\VIBuffer_Cell.h"

CVIBuffer_Cell::CVIBuffer_Cell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Cell::CVIBuffer_Cell(const CVIBuffer_Cell& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cell::Initialize_ProtoType(const _float3* pPoints)
{
	m_iVertexBuffersNum = 1;
	m_iVertexNum = 3;
	m_iVertexStride = sizeof(VTXPOS);

	m_iIndexNum = 4;
	m_iIndexStride = 2;
	m_eIndexForMat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
	
	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// 동적배열의 크기
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;					// 정적버퍼인지 동적버퍼인지 설정/ 지금은 정적
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 종류 설정
	m_Buffer_Desc.CPUAccessFlags = 0;							// 동적만 해당
	m_Buffer_Desc.MiscFlags = 0;								// 동적만 해당
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// 정점 하나의 크기를 집어 넣음

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	VTXPOS* pVerpostex = new VTXPOS[m_iVertexNum];		// 버텍스 버퍼 안에 들어 갈 값들을 설정해줌

	pVerpostex[0].fPosition = pPoints[0];

	pVerpostex[1].fPosition = pPoints[1];

	pVerpostex[2].fPosition = pPoints[2];

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

	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 0;

	m_SubResource_Data.pSysMem = pIndex;

	if (FAILED(Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndex);

	return S_OK;
}

HRESULT CVIBuffer_Cell::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Cell* CVIBuffer_Cell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints)
{
	CVIBuffer_Cell* pInstance = new CVIBuffer_Cell(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(pPoints))) {
		MSG_BOX("Failed to Created : CVIBuffer_Cell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Cell::Clone(void* pArg)
{
	CVIBuffer_Cell* pInstance = new CVIBuffer_Cell(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CVIBuffer_Cell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cell::Free()
{
	__super::Free();
}
