#include "..\Public\Collider.h"

#include "GameInstance.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CCollider::CCollider(const CCollider& rhs)
	:CComponent(rhs)
#ifdef _DEBUG

	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect), m_pInputLayout(rhs.m_pInputLayout)

{
	Safe_AddRef(m_pInputLayout);

#endif
}

HRESULT CCollider::Initialize_ProtoType()
{
#ifdef _DEBUG

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = { nullptr };
	size_t iShaderCodeLength = { 0 };

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderCodeLength, &m_pInputLayout))) return E_FAIL;

#endif


	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{

	CBounding::BOUNDING_DESC* pBounding_Desc = (CBounding::BOUNDING_DESC*)pArg;

	m_eType = pBounding_Desc->eType;

	switch (m_eType)
	{
	case CBounding::TYPE_AABB:
		m_pBounding = CBounding_AABB::Create(m_pDevice, m_pContext, pBounding_Desc);
		break;
	case CBounding::TYPE_OBB:
		m_pBounding = CBounding_OBB::Create(m_pDevice, m_pContext, pBounding_Desc);
		break;
	case CBounding::TYPE_SPHERE:
		m_pBounding = CBounding_Sphere::Create(m_pDevice, m_pContext, pBounding_Desc);
		break;
	}
	if (m_pBounding == nullptr)
		return E_FAIL;

	return S_OK;
}

#ifdef _DEBUG

HRESULT CCollider::Render()
{
	if (m_pBounding == nullptr)
		return E_FAIL;

	m_pBatch->Begin();
	
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE::VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE::PROJ));

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->Apply(m_pContext);

	if (FAILED(m_pBounding->Render(m_pBatch)))
		return E_FAIL;

	m_pBatch->End();

	return S_OK;
}

#endif

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCollider* pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Created : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

#ifdef _DEBUG

	/*Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Release(m_pInputLayout);*/

#endif

	Safe_Release(m_pBounding);

}
