#pragma once
#include "Engine_Defines.h"
#include "Client_Defines.h"

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
		CGameObject* pOwner, CGameObject** pOut, _float fLifeTime = 0.f, vector<CBone*>* vecBone = nullptr);



	static void	Create_Effect_Normal(CGameInstance* pGameInstance, const wstring& strTextureTag, const wstring& strObjTag,
		CGameObject* pOwner,_fvector vPos, CGameObject** pOut, _float fLifeTime, _float2 vSize);

	static void	Create_Damage_Effect(CGameInstance* pGameInstance, CGameObject* pOwner, _float fLifeTime, _float2 vSize);
	

};

END