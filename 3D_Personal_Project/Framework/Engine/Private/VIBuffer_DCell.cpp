#include "..\Public\VIBuffer_DCell.h"

CVIBuffer_DCell::CVIBuffer_DCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_DCell::CVIBuffer_DCell(const CVIBuffer_DCell& rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_DCell::Initialize_ProtoType(const _float3* pPoints)
{
	m_iVertexBuffersNum = 1;
	m_iVertexNum = 3;
	m_iVertexStride = sizeof(VTXPOS);

	m_iIndexNum = 4;
	m_iIndexStride = 2;
	m_eIndexForMat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
	
	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// �����迭�� ũ��
	m_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC; 					// ������������ ������������ ����/ ������ ����
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// ���� ���� ����
	m_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							// ������ �ش�
	m_Buffer_Desc.MiscFlags = 0;								// ������ �ش�
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// ���� �ϳ��� ũ�⸦ ���� ����

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	VTXPOS* pVerpostex = new VTXPOS[m_iVertexNum];		// ���ؽ� ���� �ȿ� ��� �� ������ ��������

	pVerpostex[0].fPosition = pPoints[0];

	pVerpostex[1].fPosition = pPoints[1];

	pVerpostex[2].fPosition = pPoints[2];

	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pVB)))		// ���� �Ҵ��� ������ ���۰� �Ҵ��� ������ �����Ͽ� ����
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// ���� �Ҵ��� ���� ���۰� �̹� ����߱⿡ �ʿ� ������ ����

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iIndexStride * m_iIndexNum;
	m_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;
	m_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

HRESULT CVIBuffer_DCell::Initialize(void* pArg)
{
	return S_OK;
}

void CVIBuffer_DCell::Update_Buffer(FLOAT3X3 vPositions)
{
	D3D11_MAPPED_SUBRESOURCE SubResource;
	if (FAILED(m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource)))
		return;

	((VTXPOS*)(SubResource.pData))[0].fPosition = vPositions.vVertex0;
	((VTXPOS*)(SubResource.pData))[1].fPosition = vPositions.vVertex1;
	((VTXPOS*)(SubResource.pData))[2].fPosition = vPositions.vVertex2;
	
	m_pContext->Unmap(m_pVB, 0);
}

CVIBuffer_DCell* CVIBuffer_DCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints)
{
	CVIBuffer_DCell* pInstance = new CVIBuffer_DCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(pPoints))) {
		MSG_BOX("Failed to Created : CVIBuffer_DCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_DCell::Clone(void* pArg)
{
	CVIBuffer_DCell* pInstance = new CVIBuffer_DCell(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CVIBuffer_DCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_DCell::Free()
{
	__super::Free();
}
