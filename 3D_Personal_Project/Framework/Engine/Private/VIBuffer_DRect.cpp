#include "..\Public\VIBuffer_DRect.h"

CVIBuffer_DRect::CVIBuffer_DRect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_DRect::CVIBuffer_DRect(const CVIBuffer_DRect& rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_DRect::Initialize_ProtoType()
{
	
	return S_OK;
}

HRESULT CVIBuffer_DRect::Initialize(void* pArg)
{
	m_iVertexBuffersNum = 1;
	m_iVertexNum = 4;
	m_iVertexStride = sizeof(VTXPOSTEX);

	m_iIndexNum = 6;
	m_iIndexStride = 2;
	m_eIndexForMat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// 동적배열의 크기
	m_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;					// 정적버퍼인지 동적버퍼인지 설정/ 지금은 정적
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 종류 설정
	m_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							// 동적만 해당
	m_Buffer_Desc.MiscFlags = 0;								// 동적만 해당
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// 정점 하나의 크기를 집어 넣음

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	VTXPOSTEX* pVerpostex = new VTXPOSTEX[m_iVertexNum];		// 버텍스 버퍼 안에 들어 갈 값들을 설정해줌

	pVerpostex[0].fPosition = _float3(-0.5f, 0.5f, 0.f);
	pVerpostex[0].fTexCoord = _float2(0.f, 0.f);

	pVerpostex[1].fPosition = _float3(0.5f, 0.5f, 0.f);
	pVerpostex[1].fTexCoord = _float2(1.f, 0.f);

	pVerpostex[2].fPosition = _float3(0.5f, -0.5f, 0.f);
	pVerpostex[2].fTexCoord = _float2(1.f, 1.f);

	pVerpostex[3].fPosition = _float3(-0.5f, -0.5f, 0.f);
	pVerpostex[3].fTexCoord = _float2(0.f, 1.f);

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
	pIndex[4] = 2;
	pIndex[5] = 3;

	m_SubResource_Data.pSysMem = pIndex;

	if (FAILED(Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndex);


	return S_OK;
}

void CVIBuffer_DRect::Update_Vertex(DIRECTION eDirection)
{

	D3D11_MAPPED_SUBRESOURCE SubResource;
	if (FAILED(m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource)))
		return;

	_float3 vPos[4] = {
	((VTXPOSTEX*)(SubResource.pData))[0].fPosition,
	((VTXPOSTEX*)(SubResource.pData))[1].fPosition,
	((VTXPOSTEX*)(SubResource.pData))[2].fPosition,
	((VTXPOSTEX*)(SubResource.pData))[3].fPosition
	};

	switch (eDirection)
	{
	case Engine::CVIBuffer_DRect::POSTIVE_X:
		for (_uint i = 0; i < 4; i++)
			vPos[i].x += 0.5f;
		break;
	case Engine::CVIBuffer_DRect::NEGATIVE_X:
		for (_uint i = 0; i < 4; i++)
			vPos[i].x -= 0.5f;
		break;
	case Engine::CVIBuffer_DRect::POSTIVE_Y:
		for (_uint i = 0; i < 4; i++)
			vPos[i].y += 0.5f;
		break;
	case Engine::CVIBuffer_DRect::NEGATIVE_Y:
		for (_uint i = 0; i < 4; i++)
			vPos[i].y -= 0.5f;
		break;
	case Engine::CVIBuffer_DRect::CENTER:
		vPos[0] = _float3(-0.5f, 0.5f, 0.f);
		vPos[1] = _float3(0.5f, 0.5f, 0.f);
		vPos[2] = _float3(0.5f, -0.5f, 0.f);
		vPos[3] = _float3(-0.5f, -0.5f, 0.f);
		break;
	}

	((VTXPOSTEX*)(SubResource.pData))[0].fPosition = vPos[0];
	((VTXPOSTEX*)(SubResource.pData))[1].fPosition = vPos[1];
	((VTXPOSTEX*)(SubResource.pData))[2].fPosition = vPos[2];
	((VTXPOSTEX*)(SubResource.pData))[3].fPosition = vPos[3];


	m_pContext->Unmap(m_pVB, 0);
}

CVIBuffer_DRect* CVIBuffer_DRect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_DRect* pInstance = new CVIBuffer_DRect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CVIBuffer_DRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_DRect::Clone(void* pArg)
{
	CVIBuffer_DRect* pInstance = new CVIBuffer_DRect(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CVIBuffer_DRect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_DRect::Free()
{
	__super::Free();
}
