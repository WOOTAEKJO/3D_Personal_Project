#include "..\Public\VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice,pContext)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer_Cube& rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cube::Initialize_ProtoType()
{
	m_iVertexBuffersNum = 1;
	m_iVertexNum = 8;
	m_iVertexStride = sizeof(VTXPOSTEX);

	m_iIndexNum = 36;
	m_iIndexStride = 2;
	m_eIndexForMat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// �����迭�� ũ��
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;					// ������������ ������������ ����/ ������ ����
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// ���� ���� ����
	m_Buffer_Desc.CPUAccessFlags = 0;							// ������ �ش�
	m_Buffer_Desc.MiscFlags = 0;								// ������ �ش�
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// ���� �ϳ��� ũ�⸦ ���� ����

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	VTXPOSTEX* pVerpostex = new VTXPOSTEX[m_iVertexNum];		// ���ؽ� ���� �ȿ� ��� �� ������ ��������

	pVerpostex[0].fPosition = _float3(-0.5f, 0.5f, -0.5f);
	//pVerpostex[0].fTexCoord = _float2(0.f, 0.f);

	pVerpostex[1].fPosition = _float3(0.5f, 0.5f, -0.5f);
	//pVerpostex[1].fTexCoord = _float2(1.f, 0.f);

	pVerpostex[2].fPosition = _float3(0.5f, -0.5f, -0.5f);
	//pVerpostex[2].fTexCoord = _float2(1.f, 1.f);

	pVerpostex[3].fPosition = _float3(-0.5f, -0.5f, -0.5f);
	//pVerpostex[3].fTexCoord = _float2(0.f, 1.f);

	pVerpostex[4].fPosition = _float3(-0.5f, 0.5f, 0.5f);
	//pVerpostex[0].fTexCoord = _float2(0.f, 0.f);

	pVerpostex[5].fPosition = _float3(0.5f, 0.5f, 0.5f);
	//pVerpostex[1].fTexCoord = _float2(1.f, 0.f);

	pVerpostex[6].fPosition = _float3(0.5f, -0.5f, 0.5f);
	//pVerpostex[2].fTexCoord = _float2(1.f, 1.f);

	pVerpostex[7].fPosition = _float3(-0.5f, -0.5f, 0.5f);
	//pVerpostex[3].fTexCoord = _float2(0.f, 1.f);

	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pVB)))		// ���� �Ҵ��� ������ ���۰� �Ҵ��� ������ �����Ͽ� ����
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// ���� �Ҵ��� ���� ���۰� �̹� ����߱⿡ �ʿ� ������ ����

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iIndexStride * m_iIndexNum;
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_Buffer_Desc.CPUAccessFlags = 0;
	m_Buffer_Desc.MiscFlags = 0;
	m_Buffer_Desc.StructureByteStride = 0;

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	_uint iNumIndex = 0;

	_ushort* pIndex = new _ushort[m_iIndexNum];

	pIndex[iNumIndex++] = 1;
	pIndex[iNumIndex++] = 5;
	pIndex[iNumIndex++] = 6;

	pIndex[iNumIndex++] = 1;
	pIndex[iNumIndex++] = 6;
	pIndex[iNumIndex++] = 2;
//---------------------
	pIndex[iNumIndex++] = 4;
	pIndex[iNumIndex++] = 0;
	pIndex[iNumIndex++] = 3;

	pIndex[iNumIndex++] = 4;
	pIndex[iNumIndex++] = 3;
	pIndex[iNumIndex++] = 7;
//---------------------
	pIndex[iNumIndex++] = 4;
	pIndex[iNumIndex++] = 5;
	pIndex[iNumIndex++] = 1;

	pIndex[iNumIndex++] = 4;
	pIndex[iNumIndex++] = 1;
	pIndex[iNumIndex++] = 0;
//---------------------
	pIndex[iNumIndex++] = 3;
	pIndex[iNumIndex++] = 2;
	pIndex[iNumIndex++] = 6;

	pIndex[iNumIndex++] = 3;
	pIndex[iNumIndex++] = 6;
	pIndex[iNumIndex++] = 7;

	m_SubResource_Data.pSysMem = pIndex;

	if (FAILED(Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndex);

	return S_OK;
}

HRESULT CVIBuffer_Cube::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Cube* CVIBuffer_Cube::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CVIBuffer_DTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Cube::Clone(void* pArg)
{
	CVIBuffer_Cube* pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CVIBuffer_Cube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}
