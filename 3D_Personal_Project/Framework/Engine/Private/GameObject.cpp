#include "..\Public\GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pGameInstance(CGameInstance::GetInstance())
	, m_isCloned(false)
{// 디바이스와 디바이스 컨텍스트를 받아 초기화
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
{ // 복사 생성자
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext); 
	Safe_AddRef(m_pGameInstance);
}

HRESULT CGameObject::Initialize_Prototype()
{// 원형 객체 초기화
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{// 사본 객체 초기화
	
	GAMEOBJECT_DESC GameObject_Desc = {};

	if (pArg != nullptr)
		GameObject_Desc = *(GAMEOBJECT_DESC*)pArg;

	m_pTransformCom = CTransform::Create(m_pDevice,m_pContext, GameObject_Desc.fSpeedPerSec, GameObject_Desc.fRotationPerSec);
	if (m_pTransformCom == nullptr)
		return E_FAIL;
	// 트랜스폼 컴포넌트 생성

	wstring strTag = TAG_NAME<CTransform>() + TEXT("0");

	if (Find_Component(strTag) != nullptr)
		return E_FAIL;

	m_mapComponent.emplace(strTag, m_pTransformCom);
	Safe_AddRef(m_pTransformCom);
	// 생성한 트랜스폼 컴포넌트 추가
	
	return S_OK;
}

void CGameObject::Priority_Tick(_float fTimeDelta)
{// 우선순위 틱
	for (auto& iter : m_mapComponent)
	{
		if (iter.second != nullptr)
			iter.second->Priority_Tick(fTimeDelta);
	}
	// 소유한 컴포넌트의 우선순위 틱 호출
}

void CGameObject::Tick(_float fTimeDelta)
{// 일반적인 틱
	for (auto& iter : m_mapComponent)
	{
		if (iter.second != nullptr)
			iter.second->Tick(fTimeDelta);
	}
	// 소유한 컴포넌트의 일반적인 틱 호출
}

void CGameObject::Late_Tick(_float fTimeDelta)
{// 늦은 틱
	for (auto& iter : m_mapComponent)
	{
		if (iter.second != nullptr)
			iter.second->Late_Tick(fTimeDelta);
	}
	// 소유한 컴포넌트의 늦은 틱 호출

	In_WorldPlanes();
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

void CGameObject::In_WorldPlanes()
{// 게임 오브젝트가 월드 절두체 평면 안에 있는지 판단
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_float3 fSize = m_pTransformCom->Get_Scaled();
	//_float fRadius = fSize.x <= fSize.z ? fSize.z : fSize.x;
	_float fRadius = (fSize.z + fSize.x) * 8.f;
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
	m_bIn_WorldPlanes = m_pGameInstance->IsIn_World_FrustumPlanes(vPos, fRadius);
}

void CGameObject::Write_Json(json& Out_Json)
{// 제이슨 저장
	for (auto& iter : m_mapComponent)
	{
		if(iter.second->Get_UseJson())
			iter.second->Write_Json(Out_Json["Component"]);
	}
}

void CGameObject::Load_FromJson(const json& In_Json)
{// 제이슨 로드
	for (auto& iter : m_mapComponent)
	{
		if (iter.second->Get_UseJson())
			iter.second->Load_FromJson(In_Json["Component"]);
	}
}

void CGameObject::Distroy()
{// 객체 파괴 시
	for (auto& iter : m_mapComponent) {
		iter.second->Set_Owner(nullptr);
	}
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strComTag, CComponent** pOut, void* pArg)
{// 컴포넌트 추가
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
{// 컴포넌트 검색
	auto& iter = m_mapComponent.find(strComTag);

	if (iter == m_mapComponent.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Delete_Component(const wstring& strComTag)
{// 컴포넌트 삭제
	auto& iter = m_mapComponent.find(strComTag);

	if (iter == m_mapComponent.end())
		return E_FAIL;

	Safe_Release(iter->second);

	m_mapComponent.erase(strComTag);

	return S_OK;
}

void CGameObject::Free()
{// 메모리 해제
	__super::Free();

	Safe_Release(m_pTransformCom);

	for (auto& iter : m_mapComponent)
		Safe_Release(iter.second);
	m_mapComponent.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
