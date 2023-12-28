#include "..\Public\Collider.h"

#include "GameInstance.h"
#include "GameObject.h"

_uint CCollider::m_iNextID = 0;

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

	m_pOwner = pBounding_Desc->pOnwer;
	if (FAILED(m_pOwner == nullptr))
		return E_FAIL;

	m_bUseCol = pBounding_Desc->bUseCol;

	CBounding* pBounding = nullptr;

	switch (pBounding_Desc->eType)
	{
	case CBounding::TYPE_AABB:
		pBounding = CBounding_AABB::Create(m_pDevice, m_pContext, pBounding_Desc);
		break;
	case CBounding::TYPE_OBB:
		pBounding = CBounding_OBB::Create(m_pDevice, m_pContext, pBounding_Desc);
		break;
	case CBounding::TYPE_SPHERE:
		pBounding = CBounding_Sphere::Create(m_pDevice, m_pContext, pBounding_Desc);
		break;
	}
	if (pBounding == nullptr)
		return E_FAIL;

	m_vecBounding.push_back(pBounding);

	m_iColID = m_iNextID++;

	return S_OK;
}

void CCollider::Update(_fmatrix matWorld)
{
	/*if (m_pBounding == nullptr)
		return;*/
	if (m_vecBounding.empty())
		return;

	//m_pBounding->Update(matWorld);
	for (auto& iter : m_vecBounding)
		iter->Update(matWorld);
}

#ifdef _DEBUG

HRESULT CCollider::Render()
{
	/*if (m_pBounding == nullptr)
		return E_FAIL;*/
	if (m_vecBounding.empty())
		return E_FAIL;

	m_pBatch->Begin();
	
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE::VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE::PROJ));

	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pEffect->Apply(m_pContext);

	for(auto& iter : m_vecBounding)
		if (FAILED(iter->Render(m_pBatch, m_bCollision == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f))))
			return E_FAIL;
	
	m_pBatch->End();

	return S_OK;
}

_bool CCollider::Collision(CCollider* pTargetCollider)
{
	/*if (m_pBounding == nullptr || m_pOwner == nullptr)
		return false;*/
	if (m_vecBounding.empty() || m_pOwner == nullptr)
		return false;

	for (auto& iter : m_vecBounding)
	{
		if (iter->Collision(pTargetCollider))
			return true;
	}

	return  false;
}

HRESULT CCollider::Add_Bounding(void* pArg)
{
	CBounding::BOUNDING_DESC* pBounding_Desc = (CBounding::BOUNDING_DESC*)pArg;

	CBounding* pBounding = nullptr;

	switch (pBounding_Desc->eType)
	{
	case CBounding::TYPE_AABB:
		pBounding = CBounding_AABB::Create(m_pDevice, m_pContext, pBounding_Desc);
		break;
	case CBounding::TYPE_OBB:
		pBounding = CBounding_OBB::Create(m_pDevice, m_pContext, pBounding_Desc);
		break;
	case CBounding::TYPE_SPHERE:
		pBounding = CBounding_Sphere::Create(m_pDevice, m_pContext, pBounding_Desc);
		break;
	}
	if (pBounding == nullptr)
		return E_FAIL;

	m_vecBounding.push_back(pBounding);

	return S_OK;
}

void CCollider::OnCollisionEnter(CCollider* pOtherCollider)
{
	if (m_pOwner == nullptr || pOtherCollider == nullptr)
		return;

	m_bCollision = true;

	m_pOwner->OnCollisionEnter(pOtherCollider, m_iColID);
}

void CCollider::OnCollisionStay(CCollider* pOtherCollider)
{
	if (m_pOwner == nullptr || pOtherCollider == nullptr)
		return;

	m_bCollision = true;

	m_pOwner->OnCollisionStay(pOtherCollider, m_iColID);
}

void CCollider::OnCollisionExit(CCollider* pOtherCollider)
{
	if (m_pOwner == nullptr || pOtherCollider == nullptr)
		return;

	m_bCollision = false;

	m_pOwner->OnCollisionExit(pOtherCollider,m_iColID);
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

	if (!m_bClone) {
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}

	Safe_Release(m_pInputLayout);

#endif

	//Safe_Release(m_pBounding);
	for (auto& iter : m_vecBounding)
		Safe_Release(iter);
	m_vecBounding.clear();

}
