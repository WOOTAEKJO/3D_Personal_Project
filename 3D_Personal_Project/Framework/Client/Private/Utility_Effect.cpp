#include "..\Public\Utility_Effect.h"

#include "Particle.h"
#include "Effect.h"

#include "Particle_Attack.h"
#include "Particle_Stage.h"

#include "Effect_Trail.h"

void CUtility_Effect::Create_Particle_Normal(CGameInstance* pGameInstance, const wstring& strParticleTag,
	const wstring& strObjTag, CGameObject* pOwner, CGameObject** pOut, _float fLifeTime, vector<CBone*>* vecBone,
	_bool bChild, _bool bBlur)
{
	CParticle::PARTICLEINFO Info = {};
	Info.pOwner = pOwner;
	Info.strParticleTag = strParticleTag;
	Info.fLifeTime = fLifeTime;
	if (vecBone == nullptr)
		Info.pBones = Info.pOwner->Get_Component<CModel>()->Get_Bones();
	else
		Info.pBones = *vecBone;
	Info.bChild = bChild;
	Info.bBlur = bBlur;

	if (FAILED(pGameInstance->Add_Clone(pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		strObjTag, &Info, reinterpret_cast<CGameObject**>(pOut))))
		return;
}

void CUtility_Effect::Create_Particle_Attack(CGameInstance* pGameInstance, const wstring& strParticleTag,
	const wstring& strObjTag, CGameObject* pOwner, _float4 vPos, _float3 vDir, CGameObject** pOut, _float fLifeTime,
	_bool bKeep, _bool bBlur)
{
	CParticle_Attack::ATTACKPARTICLE_DESC Desc = {};
	Desc.pOwner = pOwner;
	Desc.vPos = vPos;
	Desc.vDir = vDir;
	Desc.strParticleTag = strParticleTag;
	Desc.fLifeTime = fLifeTime;
	Desc.bKeep = bKeep;
	Desc.bBlur = bBlur;

	if (FAILED(pGameInstance->Add_Clone(pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		strObjTag, &Desc, reinterpret_cast<CGameObject**>(pOut))))
		return;
}

void CUtility_Effect::Create_Particle_Stage(CGameInstance* pGameInstance, const wstring& strParticleTag,
	_float4 vPos, CGameObject* pOwner, CGameObject** pOut, _bool bBlur)
{
	CParticle_Stage::STAGEPARTICLE_DESC Desc = {};
	Desc.pOwner = pOwner;
	Desc.vPos = vPos;
	Desc.strParticleTag = strParticleTag;
	Desc.bBlur = bBlur;

	if (FAILED(pGameInstance->Add_Clone(pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_PARTICLESTAGE_TAG, &Desc, reinterpret_cast<CGameObject**>(pOut))))
		return;
}

void CUtility_Effect::Create_Effect_Normal(CGameInstance* pGameInstance, const wstring& strTextureTag, const wstring& strObjTag,
	CGameObject * pOwner, _fvector vPos, CGameObject** pOut, _float fLifeTime, _float2 vSize)
{
	CEffect::EFFECTINFO Info = {};
	Info.pOwner = pOwner;
	Info.fLifeTime = fLifeTime;
	Info.strEffectTextureTag = strTextureTag;
	Info.vSize = vSize;

	XMStoreFloat4(&Info.vPos, vPos);

	if (FAILED(pGameInstance->Add_Clone(pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		strObjTag, &Info, reinterpret_cast<CGameObject**>(pOut))))
		return;
}

void CUtility_Effect::Create_Effect_Reaper(CGameInstance* pGameInstance, CGameObject* pOwner, _fvector vPos,
	_float4 vColor, CGameObject** pOut, _float fLifeTime, _float2 vSize, CEffect_Reaper::COMPUTE_TYPE eType)
{
	CEffect_Reaper::EFFECT_REAPERINFO Info = {};
	Info.pOwner = pOwner;
	Info.fLifeTime = fLifeTime;
	Info.strEffectTextureTag = TEX_REAPER_TAG;
	Info.vSize = vSize;
	Info.vColor = vColor;
	Info.eComputeType = eType;

	XMStoreFloat4(&Info.vPos, vPos);

	if (FAILED(pGameInstance->Add_Clone(pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_EFFECTREAPER_TAG, &Info, reinterpret_cast<CGameObject**>(pOut))))
		return;
}

void CUtility_Effect::Create_Damage_Effect(CGameInstance* pGameInstance, CGameObject* pOwner, _float fLifeTime,
	_float2 vSize)
{
	CEffect::EFFECTINFO Info = {};
	Info.pOwner = pOwner;
	Info.fLifeTime = fLifeTime;
	Info.strEffectTextureTag = TEX_DAMAGEIMPACT_TAG;
	Info.vSize = vSize;

	CTransform* pTransform = pOwner->Get_Component<CTransform>();

	_vector vPos = pTransform->Get_State(CTransform::STATE::STATE_POS);
	//vPos.m128_f32[1] += pTransform->Get_Scaled().y * 0.2f;

	XMStoreFloat4(&Info.vPos, vPos);

	if (FAILED(pGameInstance->Add_Clone(pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_EFFECTDAMAGE_TAG, &Info)))
		return;
}

void CUtility_Effect::Create_Effect_Trail(CGameInstance* pGameInstance, const wstring& strTextureTag, const wstring& strMaskTextureTag
	,CGameObject* pOwner, _float fAlpha, _bool bRevers,
	_float3 vTrailPos_0, _float3 vTrailPos_1, _uint iTrailMaxCnt, _uint iLerpPointNum,
	_float4 vColor, CGameObject** pOut)
{
	CEffect_Trail::EFFECT_TRAILINFO Info = {};
	Info.pOwner = pOwner;
	Info.strEffectTextureTag = strTextureTag;
	Info.vTrailPos_0 = vTrailPos_0;
	Info.vTrailPos_1 = vTrailPos_1;
	Info.iTrailMaxCnt = iTrailMaxCnt;
	Info.iLerpPointNum = iLerpPointNum;
	Info.vSolid_Color = vColor;
	Info.strMaskTag = strMaskTextureTag;
	Info.fAlpha = fAlpha;
	Info.bRevers = bRevers;

	if (FAILED(pGameInstance->Add_Clone(pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_EFFECT],
		GO_EFFECTTRAIL_TAG, &Info, reinterpret_cast<CGameObject**>(pOut))))
		return;
}
