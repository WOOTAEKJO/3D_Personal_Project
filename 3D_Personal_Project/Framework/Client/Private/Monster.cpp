#include "stdafx.h"
#include "..\Public\Monster.h"
#include "Player.h"

#include "Player_Body.h"

#include "NorMonster_IDLE.h"
#include "NorMonster_Move.h"
#include "NorMonster_Attack.h"
#include "NorMonster_Delay.h"
#include "NorMonster_Dead.h"
#include "NorMonster_Hited.h"
#include "NorMonster_Appear.h"

#include "Light.h"

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
	
	if (FAILED(CCharacter::Initialize(pArg)))
		return E_FAIL;

	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_ObjectList(m_pGameInstance->Get_Current_Level(),
		g_strLayerName[LAYER_PLAYER]).front());
	if (m_pPlayer == nullptr)
		return E_FAIL;

	m_pPlayer_Transform = m_pPlayer->Get_Component<CTransform>();
	if (m_pPlayer_Transform == nullptr)
		return E_FAIL;

	m_fDissolveAmount = 0.f;
	m_fDissolveGradiationDistance = 0.f;
	m_vDissolveGradiationStartColor = _float3(0.f,0.f,0.f);
	m_vDissolveGradiationGoalColor = _float3(0.f, 0.f, 0.f);

	return S_OK;
}

void CMonster::Priority_Tick(_float fTimeDelta)
{
	if (!m_bActivate)
		return;

	CCharacter::Priority_Tick(fTimeDelta);
}

void CMonster::Tick(_float fTimeDelta)
{
	

	if (!m_bActivate)
		return;

	CCharacter::Tick(fTimeDelta);
}

void CMonster::Late_Tick(_float fTimeDelta)
{
	if (!m_bActivate)
		return;

	CCharacter::Late_Tick(fTimeDelta);

	Update_Light();
}

HRESULT CMonster::Render()
{
	if (!m_bActivate && !m_bDeadTime)
		return S_OK;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (size_t i = 0; i < iNumMeshs; i++)
	{
		if (FAILED(m_pModelCom->Bind_Blend(m_pShaderCom, "g_BlendMatrix", i)))
			return E_FAIL;

		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(4);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CMonster::Render_Shadow()
{
	if (!m_bActivate)
		return S_OK;

	if (FAILED(CCharacter::Render_Shadow()))
		return E_FAIL;

	return S_OK;
}

void CMonster::TargetLook()
{
	m_pTransformCom->LookAt_OnLand(m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS));
}

void CMonster::TargetLook_Y()
{
	m_pTransformCom->LookAt(m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS));
}

_bool CMonster::Turn(_float fTimeDelta, _bool bCheck)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vTargetPos = m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS);

	_vector vDir = XMVector3Normalize(vTargetPos - vPos);

	return m_pTransformCom->Turn_Dir_Yaxis(vDir, fTimeDelta * 4.f, bCheck);

	//return m_pTransformCom->Turn_Target_Yaxis(m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS), fTimeDelta * 5.f);
}

_bool CMonster::Is_Target_Range(_float fRange)
{
	_float vDist = XMVectorGetX(XMVector3Length((m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS)
		- m_pTransformCom->Get_State(CTransform::STATE::STATE_POS))));

	if (vDist <= fRange)
		return true;

	return false;
}

CCollider* CMonster::Get_WeaponCollider()
{
	if (m_pWeaponColliderCom == nullptr)
		return nullptr;

	return m_pWeaponColliderCom;
}

_float4x4 CMonster::Get_Col_WorldMat()
{
	if(m_pColliderCom == nullptr)
		return _float4x4();

	return m_pColliderCom->Get_Collider_WorldMat();
}

void CMonster::Monster_Dead(_float fTimeDelta)
{
	if (m_Status_Desc.iCurHP <= 0)
	{
		//Set_Dead();
		if (!m_bDeadTime)
		{
			Create_Soul_Effect(1.f);

			if (m_pLight != nullptr)
				m_pLight->Set_Active(false);

			m_pGameInstance->Play_Sound(L"Spooketon", L"DeadVoice.ogg", CHANNELID::SOUND_MONSTER_VOICE, 0.5f, false);
			m_pGameInstance->Play_Sound(L"Spooketon", L"Dead.ogg", CHANNELID::SOUND_MONSTER_HIT, 0.3f, false);

			m_pStateMachineCom->Set_State(CMonster::STATE::DEAD);

			m_bDeadTime = true;
			m_bActivate = false;

			m_pColliderCom->Set_UseCol(false);

			if (m_pLightEffect != nullptr)
				m_pLightEffect->Set_Dead();
		}
	}
}

void CMonster::Dissolve(_float fAmount, _float fDistance, _float fTimeDelta)
{
	if (m_bDeadTime)
	{
		if (m_fDissolveAmount < 1.f)
			m_fDissolveAmount += fTimeDelta * fAmount;

		if (m_fDissolveGradiationDistance < 1.f)
			m_fDissolveGradiationDistance += fTimeDelta * fDistance;
	}
}

HRESULT CMonster::Bind_ShaderResources()
{
	if (FAILED(CCharacter::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pNoiseTextureCom->Bind_ShaderResources(m_pShaderCom, "g_DissolveTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &m_fDissolveAmount, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveGradiationDistance", &m_fDissolveGradiationDistance, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDissolveGradiationStartColor", &m_vDissolveGradiationStartColor, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDissolveGradiationGoalColor", &m_vDissolveGradiationGoalColor, sizeof(_float3))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Ready_Component()
{
	if (FAILED(CCharacter::Ready_Component()))
		return E_FAIL;

	if (FAILED(Add_Component<CTexture>(NOISE_DEFAULT_TAG, &m_pNoiseTextureCom))) return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Ready_State()
{
	if (FAILED(m_pStateMachineCom->Add_State(STATE::IDLE, CNorMonster_IDLE::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::MOVE, CNorMonster_Move::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::ATTACK, CNorMonster_Attack::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::DEAD, CNorMonster_Dead::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::DELAY, CNorMonster_Delay::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::HITED, CNorMonster_Hited::Create(this)))) return E_FAIL;
	if (FAILED(m_pStateMachineCom->Add_State(STATE::APPEAR, CNorMonster_Appear::Create(this)))) return E_FAIL;

	if (FAILED(m_pStateMachineCom->Init_State(STATE::APPEAR)))
		return E_FAIL;

	return S_OK;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pSocketBone);
	Safe_Release(m_pWeaponColliderCom);
	Safe_Release(m_pNoiseTextureCom);
}
