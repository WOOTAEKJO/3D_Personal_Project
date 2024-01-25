#include "stdafx.h"
#include "..\Public\Character_State.h"
#include "StateMachine.h"

#include "Particle.h"

CCharacter_State::CCharacter_State()
{
}

HRESULT CCharacter_State::Initialize(CGameObject* pGameObject)
{

	return S_OK;
}

void CCharacter_State::State_Enter()
{
}

_uint CCharacter_State::State_Priority_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CCharacter_State::State_Tick(_float fTimeDelta)
{
	return m_iStateID;
}

_uint CCharacter_State::State_Late_Tick(_float fTimeDelta)
{

	return m_iStateID;
}

void CCharacter_State::State_Exit()
{
}

void CCharacter_State::Translate(CTransform::STATE eType,_float fSpeed, _float fTimeDelta, _bool bTurn)
{
	_vector vDir = m_pOnwerTransform->Get_State(eType);
	_vector vPos = XMVector3Normalize(vDir) * fSpeed * fTimeDelta
		* (bTurn == false ? 1.f : -1.f);

	if (std::isnan(vPos.m128_f32[0]))
		return;

	m_pOnwerTransform->Translate(vPos, m_pOnwerNavigation, fTimeDelta);
}

void CCharacter_State::Translate_Simple(CTransform::STATE eType, _float fSpeed, _float fTimeDelta, _bool bTurn)
{
	_vector vDir = m_pOnwerTransform->Get_State(eType);
	_vector vPos = XMVector3Normalize(vDir) * fSpeed * fTimeDelta
		* (bTurn == false ? 1.f : -1.f);

	if (std::isnan(vPos.m128_f32[0]))
		return;

	m_pOnwerTransform->Translate_Simple(vPos);
}

_bool CCharacter_State::Is_Attack_Time(_float fTimeDelta, _float fTime, CCollider* pOwnerCol)
{
	if (m_bAttack)
		m_fTime += fTimeDelta;

	if (m_fTime > fTime && m_bAttack)
	{
		if(pOwnerCol != nullptr)
			pOwnerCol->Set_UseCol(true);

		m_bAttack = false;
		m_fTime = 0.f;
		return true;
	}

	return false;
}

void CCharacter_State::Reset_Attack_Time(CCollider* pOwnerCol)
{
	pOwnerCol->Set_UseCol(false);
	m_bAttack = true;
}

void CCharacter_State::Create_Particle(const wstring& strParticleTag,const wstring& strObjTag, CGameObject* pOwner,
	CGameObject** pOut, _float fLifeTime, vector<CBone*>* vecBone)
{
	CParticle::PARTICLEINFO Info = {};
	Info.pOwner = pOwner;
	Info.strParticleTag = strParticleTag;
	Info.fLifeTime = fLifeTime;
	if (vecBone == nullptr)
		Info.pBones = Info.pOwner->Get_Component<CModel>()->Get_Bones();
	else
		Info.pBones = *vecBone;

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		strObjTag, &Info, reinterpret_cast<CGameObject**>(pOut))))
		return;
}

void CCharacter_State::Particle_Loop_SetUp(CGameObject* pParticle, _bool bCheck)
{
	if (pParticle == nullptr)
		return;

	dynamic_cast<CParticle*>(pParticle)->Get_Component<CVIBuffer_Particle_Point>()->Open_InstancingDesc()->bLoop = bCheck;
}

void CCharacter_State::Free()
{
	__super::Free();

	Safe_Release(m_pOwnerModel);
	Safe_Release(m_pOnwerTransform);
	Safe_Release(m_pOnwerNavigation);
	Safe_Release(m_pOnwerRigidBody);
	Safe_Release(m_pOnwerController);
	Safe_Release(m_pOwnerCollider);

	Safe_Release(m_pParticle);
}
