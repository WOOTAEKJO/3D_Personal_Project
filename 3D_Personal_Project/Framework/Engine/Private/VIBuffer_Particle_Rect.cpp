#include "..\Public\VIBuffer_Particle_Rect.h"

CVIBuffer_Particle_Rect::CVIBuffer_Particle_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer_Instancing(pDevice, pContext)
{
}

CVIBuffer_Particle_Rect::CVIBuffer_Particle_Rect(const CVIBuffer_Particle_Rect& rhs)
	:CVIBuffer_Instancing(rhs)
{
}

HRESULT CVIBuffer_Particle_Rect::Initialize_ProtoType(_uint iInstanceNum)
{
	m_iVertexBuffersNum = 2;
	m_iVertexNum = 4;
	m_iVertexStride = sizeof(VTXPOSTEX);

	m_iInstanceNum = iInstanceNum;
	m_iInstanceStride = sizeof(VTXINSTANCING);
	m_iIndexCountPerInstance = 6;

	m_iIndexNum = 6 * m_iInstanceNum;
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

	pVerpostex[0].fPosition = _float3(-0.5f, 0.5f, 0.f);
	pVerpostex[0].fTexCoord = _float2(0.f, 0.f);

	pVerpostex[1].fPosition = _float3(0.5f, 0.5f, 0.f);
	pVerpostex[1].fTexCoord = _float2(1.f, 0.f);

	pVerpostex[2].fPosition = _float3(0.5f, -0.5f, 0.f);
	pVerpostex[2].fTexCoord = _float2(1.f, 1.f);

	pVerpostex[3].fPosition = _float3(-0.5f, -0.5f, 0.f);
	pVerpostex[3].fTexCoord = _float2(0.f, 1.f);

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

	_ushort* pIndex = new _ushort[m_iIndexNum];

	_uint iIndex = { 0 };

	for (_uint i = 0; i < m_iInstanceNum; i++)
	{
		pIndex[iIndex++] = 0;
		pIndex[iIndex++] = 1;
		pIndex[iIndex++] = 2;
			   
		pIndex[iIndex++] = 0;
		pIndex[iIndex++] = 2;
		pIndex[iIndex++] = 3;
	}

	m_SubResource_Data.pSysMem = pIndex;

	if (FAILED(Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndex);

	return S_OK;
}

HRESULT CVIBuffer_Particle_Rect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
		
	return S_OK;
}

CVIBuffer_Particle_Rect* CVIBuffer_Particle_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iInstanceNum)
{
	CVIBuffer_Particle_Rect* pInstance = new CVIBuffer_Particle_Rect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(iInstanceNum))) {
		MSG_BOX("Failed to Created : CVIBuffer_Particle_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Particle_Rect::Clone(void* pArg)
{
	CVIBuffer_Particle_Rect* pInstance = new CVIBuffer_Particle_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CVIBuffer_Particle_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Particle_Rect::Free()
{
	__super::Free();
}
