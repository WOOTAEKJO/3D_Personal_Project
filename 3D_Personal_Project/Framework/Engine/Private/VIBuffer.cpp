#include "..\Public\VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	:CComponent(rhs),m_pVB(rhs.m_pVB),m_pIB(rhs.m_pIB)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	m_pContext->DrawIndexed(m_iIndexNum, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::Bind_Buffer()
{
	if (nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;

	ID3D11Buffer*	pVerTexBuffers[] = {
		m_pVB,
	};	// 버텍스 버퍼들 배열

	_uint			iStrides[] = {
		m_iVertexStride
	};	// 버텍스 버퍼들의 정점 하나의 크기 배열

	_uint			iOffsets[] = {
		0
	}; // 버텍스 버퍼들의 offset

	/* 버텍스 버퍼를 바인딩*/
	m_pContext->IASetVertexBuffers(0,m_iVertexBuffersNum,pVerTexBuffers, iStrides, iOffsets);

	/* 인덱스 버퍼를 바인딩*/
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexForMat,0);

	/* 정점을 이용해서 어떤식으로 그릴 것인지를 설정*/
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* InputLayout : 내가 그리기위해 사용하는 정점의 입력정보.  */
	/* dx11에서는 고정기능 렌더링파이프라인에 대한 기능이 삭제되었다. */
	/* 우리가 직접 렌더링 파이프라인을 수행해야한다.(사용자정의 렌더링파이프라인) -> Shader */
	/* 그래서 우리에겐 반드시 셰이더가 필요하다. */
	/* 우리가 이 정점들을 그리기위해서는 셰이더가 필요하고, 이 셰이더는 반드시 내가 그릴려고하는 정점을 받아줄 수 있어야한다. */
	/* 내가 그리려고하는 정점이 사용하려고하는 셰이더에 입력이 가능한지?에 대한 체크를 사전에 미리 처리하고.
	가능하다면 dx11이 InputLayout이란 객체를 만들어준다. */

	/* 정점의 구조체 안에 멤버 변수마다 쉐이더에 어떻게 넣어줄지를 설정하는 구조체*/
	/* 지금 사용하는 정점 구조체는 멤버 변수가 2개. 그래서 2개를 배열에 넣어줬다.*/
	D3D10_INPUT_ELEMENT_DESC	Elements[] = {
		{},
		{}
	};

	/*m_pDevice->CreateInputLayout(Elements);
	m_pContext->IASetInputLayout();*/
	
	return S_OK;
}

HRESULT CVIBuffer::Create_Buffer(_Inout_ ID3D11Buffer** pBuffer)
{
	if (FAILED(m_pDevice->CreateBuffer(&m_Buffer_Desc, &m_SubResource_Data, pBuffer)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
