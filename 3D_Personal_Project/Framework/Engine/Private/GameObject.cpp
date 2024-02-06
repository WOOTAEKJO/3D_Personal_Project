#include "..\Public\GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pGameInstance(CGameInstance::GetInstance())
	, m_isCloned(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
	, m_pGameInstance(rhs.m_pGameInstance)
	, m_isCloned(true)
	, m_bDead(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext); 
	Safe_AddRef(m_pGameInstance);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	
	GAMEOBJECT_DESC GameObject_Desc = {};

	if (pArg != nullptr)
		GameObject_Desc = *(GAMEOBJECT_DESC*)pArg;

	m_pTransformCom = CTransform::Create(m_pDevice,m_pContext, GameObject_Desc.fSpeedPerSec, GameObject_Desc.fRotationPerSec);
	if (m_pTransformCom == nullptr)
		return E_FAIL;

	wstring strTag = TAG_NAME<CTransform>() + TEXT("0");

	if (Find_Component(strTag) != nullptr)
		return E_FAIL;

	m_mapComponent.emplace(strTag, m_pTransformCom);
	Safe_AddRef(m_pTransformCom);
	
	return S_OK;
}

void CGameObject::Priority_Tick(_float fTimeDelta)
{
	for (auto& iter : m_mapComponent)
	{
		if (iter.second != nullptr)
			iter.second->Priority_Tick(fTimeDelta);
	}
}

void CGameObject::Tick(_float fTimeDelta)
{
	for (auto& iter : m_mapComponent)
	{
		if (iter.second != nullptr)
			iter.second->Tick(fTimeDelta);
	}
}

void CGameObject::Late_Tick(_float fTimeDelta)
{
	for (auto& iter : m_mapComponent)
	{
		if (iter.second != nullptr)
			iter.second->Late_Tick(fTimeDelta);
	}

	In_WorldPlanes();
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

void CGameObject::In_WorldPlanes()
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_float3 fSize = m_pTransformCom->Get_Scaled();
	//_float fRadius = fSize.x <= fSize.z ? fSize.z : fSize.x;
	_float fRadius = (fSize.z + fSize.x) * 8.f;
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
	m_bIn_WorldPlanes = m_pGameInstance->IsIn_World_FrustumPlanes(vPos, fRadius);
}

void CGameObject::Write_Json(json& Out_Json)
{
	for (auto& iter : m_mapComponent)
	{
		if(iter.second->Get_UseJson())
			iter.second->Write_Json(Out_Json["Component"]);
	}
}

void CGameObject::Load_FromJson(const json& In_Json)
{
	for (auto& iter : m_mapComponent)
	{
		if (iter.second->Get_UseJson())
			iter.second->Load_FromJson(In_Json["Component"]);
	}
}

void CGameObject::Distroy()
{
	for (auto& iter : m_mapComponent) {
		iter.second->Set_Owner(nullptr);
	}
	/*for (auto& iter : m_mapComponent)
		Safe_Release(iter.second);
	m_mapComponent.clear();*/
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strComTag, CComponent** pOut, void* pArg)
{
	CComponent* pComponent = Find_Component(strComTag);
	if (pComponent != nullptr)
		return E_FAIL;

	CComponent* Clone = m_pGameInstance->Add_Component_Clone(iLevelIndex, strPrototypeTag, pArg);
	if (Clone == nullptr)
		return E_FAIL;

	*pOut = Clone;

	m_mapComponent.emplace(strComTag, Clone);

	Safe_AddRef(Clone);

	return S_OK;
}

CComponent* CGameObject::Find_Component(const wstring& strComTag)
{
	auto& iter = m_mapComponent.find(strComTag);

	if (iter == m_mapComponent.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Delete_Component(const wstring& strComTag)
{
	auto& iter = m_mapComponent.find(strComTag);

	if (iter == m_mapComponent.end())
		return E_FAIL;

	Safe_Release(iter->second);

	m_mapComponent.erase(strComTag);

	return S_OK;
}

void CGameObject::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);

	for (auto& iter : m_mapComponent)
		Safe_Release(iter.second);
	m_mapComponent.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
