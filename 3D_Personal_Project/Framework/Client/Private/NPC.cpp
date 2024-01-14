#include "stdafx.h"
#include "..\Public\NPC.h"
#include "Player.h"

#include "NPC_IDLE.h"
#include "NPC_Follow.h"
#include "NPC_Attack.h"

CNPC::CNPC(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter(pDevice, pContext)
{
}

CNPC::CNPC(const CNPC& rhs)
	:CCharacter(rhs)
{
}

HRESULT CNPC::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNPC::Initialize(void* pArg)
{
	
	if (FAILED(CCharacter::Initialize(pArg)))
		return E_FAIL;

	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_ObjectList(m_pGameInstance->Get_Current_Level(),
		g_strLayerName[LAYER_PLAYER]).front());
	if (m_pPlayer == nullptr)
		return E_FAIL;

	m_pPlayer_Transform = m_pPlayer->Get_Component<CTransform>();
	if (m_pPlayer_Transform == nullptr)
		return E_FAIL;

	return S_OK;
}

void CNPC::Priority_Tick(_float fTimeDelta)
{
	CCharacter::Priority_Tick(fTimeDelta);
}

void CNPC::Tick(_float fTimeDelta)
{
	CCharacter::Tick(fTimeDelta);
}

void CNPC::Late_Tick(_float fTimeDelta)
{
	CCharacter::Late_Tick(fTimeDelta);
}

HRESULT CNPC::Render()
{
	if (FAILED(CCharacter::Render()))
		return E_FAIL;

	return S_OK;
}

void CNPC::PlayerLook()
{
	_vector vDir = m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_LOOK);

	m_pTransformCom->LookAt_Dir(vDir);
}

void CNPC::Target_Follow_Look(_bool bGround)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vTargetPos = XMLoadFloat4(&m_vTargetPos);

	_vector vDir = XMVector3Normalize(vTargetPos - vPos);

	m_pTransformCom->LookAt_Dir(vDir, bGround);
}

_bool CNPC::Turn(_float fTimeDelta)
{
	//return m_pTransformCom->Turn_Target_Yaxis(XMLoadFloat4(&m_vTargetPos), fTimeDelta * 4.f);

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vTargetPos = XMLoadFloat4(&m_vTargetPos);

	_vector vDir = XMVector3Normalize(vTargetPos - vPos);

	return m_pTransformCom->Turn_Dir_Yaxis(vDir, fTimeDelta * 4.f);
}

_bool CNPC::Is_Target_Range( _float fRange)
{
	_float vDist = XMVectorGetX(XMVector3Length((XMLoadFloat4(&m_vTargetPos)
		- m_pTransformCom->Get_State(CTransform::STATE::STATE_POS))));

	if (vDist <= fRange)
		return true;

	return false;
}

void CNPC::Target_Follow( _float fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vTargetPos = XMLoadFloat4(&m_vTargetPos);
	_vector vDir = vTargetPos - vPos;
	_vector vTranslation = XMVector3Normalize(vDir) * fTimeDelta;
	m_pTransformCom->Translate(vTranslation, m_pNavigationCom, fTimeDelta);
}

_float CNPC::Distance_Proportional(_float fMin, _float fMax, _float fRange)
{
	_float fSpeed = 0.f;

	_float fUnit = fRange / (fMax - fMin);
	
	_float vDist = XMVectorGetX(XMVector3Length((XMLoadFloat4(&m_vTargetPos)
		- m_pTransformCom->Get_State(CTransform::STATE::STATE_POS))));

	if (vDist > fRange)
		fSpeed = fMax;
	else if (vDist < 0.f)
		fSpeed = 0.f;
	else
	{
		
		fSpeed = fMax - ((fRange - vDist) / fUnit);
	}

	return fSpeed;
}

_bool CNPC::Is_Target_State(_uint iTargetState)
{
	if (m_pPlayer->Get_CurrentState() == iTargetState)
		return true;

	return false;
}

void CNPC::Trans_Attack(_bool bCheck)
{
	m_pColliderCom->Set_UseCol(bCheck);
}

HRESULT CNPC::Bind_ShaderResources()
{
	if (FAILED(CCharacter::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNPC::Ready_Component()
{
	if (FAILED(CCharacter::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNPC::Ready_State()
{
	if (FAILED(m_pStateMachineCom->Add_State(STATE::IDLE, CNPC_IDLE::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::FOLLOW, CNPC_Follow::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::ATTACK, CNPC_Attack::Create(this)))) return E_FAIL;

	if (FAILED(m_pStateMachineCom->Init_State(STATE::IDLE)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNPC::Ready_Animation()
{

	return S_OK;
}

void CNPC::Free()
{
	__super::Free();

	//Safe_Release(m_pSocketBone);
}
