#pragma once
#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "Effect_Reaper.h"

BEGIN(Engine)

class CGameInstance;
class CGameObject;
class CBone;

END

BEGIN(Client)

class CUtility_Effect
{
public:
	static void Create_Particle_Normal(CGameInstance* pGameInstance, const wstring& strParticleTag, const wstring& strObjTag,
		CGameObject* pOwner, CGameObject** pOut, _float fLifeTime = 0.f, vector<CBone*>* vecBone = nullptr, _bool bChild = false);

	static void Create_Particle_Attack(CGameInstance* pGameInstance, const wstring& strParticleTag, const wstring& strObjTag,
		CGameObject* pOwner, _float4 vPos, _float3 vDir, CGameObject** pOut = nullptr, _float fLifeTime = 0.f,_bool bKeep = false);

	static void Create_Particle_Stage(CGameInstance* pGameInstance, const wstring& strParticleTag,_float4 vPos,
		CGameObject* pOwner, CGameObject** pOut);

	static void	Create_Effect_Normal(CGameInstance* pGameInstance, const wstring& strTextureTag, const wstring& strObjTag,
		CGameObject* pOwner,_fvector vPos, CGameObject** pOut, _float fLifeTime, _float2 vSize);

	static void	Create_Effect_Reaper(CGameInstance* pGameInstance,
		CGameObject* pOwner, _fvector vPos,_float4 vColor, CGameObject** pOut, _float fLifeTime, _float2 vSize,
		CEffect_Reaper::COMPUTE_TYPE eType = CEffect_Reaper::COMPUTE_TYPE::POS_END);

	static void	Create_Damage_Effect(CGameInstance* pGameInstance, CGameObject* pOwner, _float fLifeTime, _float2 vSize);
	

};

END