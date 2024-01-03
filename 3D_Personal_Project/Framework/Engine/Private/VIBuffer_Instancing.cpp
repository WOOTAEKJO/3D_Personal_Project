#include "..\Public\VIBuffer_Instancing.h"

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Instancing::CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs)
	:CVIBuffer(rhs),m_iInstanceNum(rhs.m_iInstanceNum),m_iInstanceStride(rhs.m_iInstanceStride)
	,m_iIndexCountPerInstance(rhs.m_iIndexCountPerInstance)
{
}

HRESULT CVIBuffer_Instancing::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CVIBuffer_Instancing::Initialize(void* pArg)
{
	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iInstanceStride * m_iInstanceNum;	// 동적배열의 크기
	m_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;					// 정적버퍼인지 동적버퍼인지 설정/ 지금은 정적
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 종류 설정
	m_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// 동적만 해당
	m_Buffer_Desc.MiscFlags = 0;								// 동적만 해당
	m_Buffer_Desc.StructureByteStride = m_iInstanceStride;		// 정점 하나의 크기를 집어 넣음

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	VTXINSTANCING* pVerpostex = new VTXINSTANCING[m_iVertexNum];		// 버텍스 버퍼 안에 들어 갈 값들을 설정해줌

	for (_uint i = 0; i < m_iInstanceNum; i++)
	{
		pVerpostex[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		pVerpostex[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		pVerpostex[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		pVerpostex[i].vPos = _float4(rand() % 10, rand() % 10, rand() % 10, 1.f);

	}

	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pInstanceBuffer)))		// 내가 할당한 값들을 버퍼가 할당한 공간에 복사하여 생성
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// 내가 할당한 것은 버퍼가 이미 사용했기에 필요 없으니 해제

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Bind_Buffer()
{
	if (nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;

	ID3D11Buffer* pVerTexBuffers[] = {
		m_pVB,
		m_pInstanceBuffer
	};	// 버텍스 버퍼들 배열

	_uint			iStrides[] = {
		m_iVertexStride,
		m_iInstanceStride
	};	// 버텍스 버퍼들의 정점 하나의 크기 배열

	_uint			iOffsets[] = {
		0,
		0
	}; // 버텍스 버퍼들의 offset

	/* 버텍스 버퍼를 바인딩*/
	m_pContext->IASetVertexBuffers(0, m_iVertexBuffersNum, pVerTexBuffers, iStrides, iOffsets);

	/* 인덱스 버퍼를 바인딩*/
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexForMat, 0);

	/* 정점을 이용해서 어떤식으로 그릴 것인지를 설정*/
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Render()
{
	m_pContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_iInstanceNum, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Instancing::Free()
{
	__super::Free();

	Safe_Release(m_pInstanceBuffer);
}
