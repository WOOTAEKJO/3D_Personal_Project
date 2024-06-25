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
{// 디바이스와 디바이스 컨텍스트를 받아 초기화
}

CMonster::CMonster(const CMonster& rhs)
	:CCharacter(rhs)
{// 복사 생성자
}

HRESULT CMonster::Initialize_Prototype()
{// 원형 객체 초기화
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{// 사본 객체 초기화
	
	if (FAILED(CCharacter::Initialize(pArg)))
		return E_FAIL;

	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_ObjectList(m_pGameInstance->Get_Current_Level(),
		g_strLayerName[LAYER_PLAYER]).front());
	if (m_pPlayer == nullptr)
		return E_FAIL;
	// 플레이어 객체를 받아온다.

	m_pPlayer_Transform = m_pPlayer->Get_Component<CTransform>();
	if (m_pPlayer_Transform == nullptr)
		return E_FAIL;
	// 플레이어의 트랜스폼 컴포넌트를 받아온다.

	m_fDissolveAmount = 0.f;
	m_fDissolveGradiationDistance = 0.f;
	m_vDissolveGradiationStartColor = _float3(0.f,0.f,0.f);
	m_vDissolveGradiationGoalColor = _float3(0.f, 0.f, 0.f);
	// 디졸브 변수 초기화

	return S_OK;
}

void CMonster::Priority_Tick(_float fTimeDelta)
{// 우선순위 틱
	if (!m_bActivate)
		return;

	CCharacter::Priority_Tick(fTimeDelta);
}

void CMonster::Tick(_float fTimeDelta)
{// 일반적인 틱
	

	if (!m_bActivate)
		return;

	CCharacter::Tick(fTimeDelta);
}

void CMonster::Late_Tick(_float fTimeDelta)
{// 늦은 틱
	if (!m_bActivate)
		return;

	CCharacter::Late_Tick(fTimeDelta);

	Update_Light();
}

HRESULT CMonster::Render()
{// 객체 렌더링
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
{// 그림자 렌더링
	if (!m_bActivate)
		return S_OK;

	if (FAILED(CCharacter::Render_Shadow()))
		return E_FAIL;

	return S_OK;
}

void CMonster::TargetLook()
{// Y값을 고려하지 않고 타겟을 바라본다.
	m_pTransformCom->LookAt_OnLand(m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS));
}

void CMonster::TargetLook_Y()
{// Y값을 고려해서 타겟을 바라본다.
	m_pTransformCom->LookAt(m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS));
}

_bool CMonster::Turn(_float fTimeDelta, _bool bCheck)
{// 회전
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vTargetPos = m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS);

	_vector vDir = XMVector3Normalize(vTargetPos - vPos);

	return m_pTransformCom->Turn_Dir_Yaxis(vDir, fTimeDelta * 4.f, bCheck);

	//return m_pTransformCom->Turn_Target_Yaxis(m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS), fTimeDelta * 5.f);
}

_bool CMonster::Is_Target_Range(_float fRange)
{// 범위 안에 타겟 있는지 체크
	_float vDist = XMVectorGetX(XMVector3Length((m_pPlayer_Transform->Get_State(CTransform::STATE::STATE_POS)
		- m_pTransformCom->Get_State(CTransform::STATE::STATE_POS))));

	if (vDist <= fRange)
		return true;

	return false;
}

CCollider* CMonster::Get_WeaponCollider()
{// 무기 콜라이더 반환
	if (m_pWeaponColliderCom == nullptr)
		return nullptr;

	return m_pWeaponColliderCom;
}

_float4x4 CMonster::Get_Col_WorldMat()
{// 콜라이더 월드행렬 반환
	if(m_pColliderCom == nullptr)
		return _float4x4();

	return m_pColliderCom->Get_Collider_WorldMat();
}

void CMonster::Monster_Dead(_float fTimeDelta)
{// 몬스터 죽음 처리
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
{// 디졸브 처리
	if (m_bDeadTime)
	{
		if (m_fDissolveAmount < 1.f)
			m_fDissolveAmount += fTimeDelta * fAmount;

		if (m_fDissolveGradiationDistance < 1.f)
			m_fDissolveGradiationDistance += fTimeDelta * fDistance;
	}
}

HRESULT CMonster::Bind_ShaderResources()
{// 셰이더 리소스 바인딩
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
{// 컴포넌트 준비
	if (FAILED(CCharacter::Ready_Component()))
		return E_FAIL;

	if (FAILED(Add_Component<CTexture>(NOISE_DEFAULT_TAG, &m_pNoiseTextureCom))) return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Ready_State()
{// 일반적인 몬스터 상태 준비
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
{// 메모리 해제
	__super::Free();

	Safe_Release(m_pSocketBone);
	Safe_Release(m_pWeaponColliderCom);
	Safe_Release(m_pNoiseTextureCom);
}
