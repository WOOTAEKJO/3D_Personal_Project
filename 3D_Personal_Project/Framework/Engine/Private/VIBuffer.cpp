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
	};	// ���ؽ� ���۵� �迭

	_uint			iStrides[] = {
		m_iVertexStride
	};	// ���ؽ� ���۵��� ���� �ϳ��� ũ�� �迭

	_uint			iOffsets[] = {
		0
	}; // ���ؽ� ���۵��� offset

	/* ���ؽ� ���۸� ���ε�*/
	m_pContext->IASetVertexBuffers(0,m_iVertexBuffersNum,pVerTexBuffers, iStrides, iOffsets);

	/* �ε��� ���۸� ���ε�*/
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexForMat,0);

	/* ������ �̿��ؼ� ������� �׸� �������� ����*/
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* InputLayout : ���� �׸������� ����ϴ� ������ �Է�����.  */
	/* dx11������ ������� ���������������ο� ���� ����� �����Ǿ���. */
	/* �츮�� ���� ������ ������������ �����ؾ��Ѵ�.(��������� ����������������) -> Shader */
	/* �׷��� �츮���� �ݵ�� ���̴��� �ʿ��ϴ�. */
	/* �츮�� �� �������� �׸������ؼ��� ���̴��� �ʿ��ϰ�, �� ���̴��� �ݵ�� ���� �׸������ϴ� ������ �޾��� �� �־���Ѵ�. */
	/* ���� �׸������ϴ� ������ ����Ϸ����ϴ� ���̴��� �Է��� ��������?�� ���� üũ�� ������ �̸� ó���ϰ�.
	�����ϴٸ� dx11�� InputLayout�̶� ��ü�� ������ش�. */

	/* ������ ����ü �ȿ� ��� �������� ���̴��� ��� �־������� �����ϴ� ����ü*/
	/* ���� ����ϴ� ���� ����ü�� ��� ������ 2��. �׷��� 2���� �迭�� �־����.*/
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
