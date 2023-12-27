#include "stdafx.h"
#include "..\Public\Monster.h"
#include "Player.h"

#include "Player_Body.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter(pDevice, pContext)
{
}

CMonster::CMonster(const CMonster& rhs)
	:CCharacter(rhs)
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	
	if (FAILED(CCharacter::Initialize(&pArg)))
		return E_FAIL;

	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_ObjectList(LEVEL_GAMEPLAY, TEXT("Player")).front());
	if (m_pPlayer == nullptr)
		return E_FAIL;

	m_pPlayer_Transform = m_pPlayer->Get_Component<CTransform>();
	if (m_pPlayer_Transform == nullptr)
		return E_FAIL;

	return S_OK;
}

void CMonster::Priority_Tick(_float fTimeDelta)
{
	CCharacter::Priority_Tick(fTimeDelta);
}

void CMonster::Tick(_float fTimeDelta)
{
	CCharacter::Tick(fTimeDelta);
}

void CMonster::Late_Tick(_float fTimeDelta)
{
	CCharacter::Late_Tick(fTimeDelta);
}

HRESULT CMonster::Render()
{
	if (FAILED(CCharacter::Render()))
		return E_FAIL;

	return S_OK;
}

void CMonster::TargetLook()
{
	m_pTransformCom->LookAt_OnLand(m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS));
}

void CMonster::Turn(_float fTimeDelta)
{
	_vector Dir = XMVector3Normalize(m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS) -
		m_pTransformCom->Get_State(CTransform::STATE::STATE_POS));

	_float fAngle = XMVectorGetX(XMVector3Dot(Dir,XMVector3Normalize( m_pTransformCom->Get_State(CTransform::STATE::STATE_RIGHT))));

	m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fAngle * 4.f);
}

_bool CMonster::Is_Target_Range(_float fRange)
{
	_float vDist = XMVectorGetX(XMVector3Length((m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS)
		- m_pTransformCom->Get_State(CTransform::STATE::STATE_POS))));

	if (vDist <= fRange)
		return true;

	return false;
}

void CMonster::Hited()
{
	
}

CCollider* CMonster::Get_WeaponCollider()
{
	if (m_pWeaponColliderCom == nullptr)
		return nullptr;

	return m_pWeaponColliderCom;
}

HRESULT CMonster::Bind_ShaderResources()
{
	if (FAILED(CCharacter::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Ready_Component()
{
	if (FAILED(CCharacter::Ready_Component()))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CMonster::Ready_State()
{
	return S_OK;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pSocketBone);
	Safe_Release(m_pWeaponColliderCom);
}
