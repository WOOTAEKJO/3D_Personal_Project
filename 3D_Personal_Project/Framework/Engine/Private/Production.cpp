#include "..\Public\Production.h"

#include "GameInstance.h"

CProduction::CProduction()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CProduction::Initialize()
{
	return S_OK;
}

void CProduction::Enter()
{
}

void CProduction::Tick()
{
	// 카메라 전환
	// 대화창
}

void CProduction::Render()
{
	// 폰트 렌더
}

void CProduction::Exite()
{
}

HRESULT CProduction::Add_Actor(const wstring& strActorTag, CGameObject* pActor)
{
	CGameObject* pAct = Find_Actor(strActorTag);
	if (pAct != nullptr)
		return E_FAIL;

	m_mapActor.emplace(strActorTag, pActor);
	//Safe_AddRef(pActor);

	return S_OK;
}

HRESULT CProduction::Add_Dialog(const wstring& strDialogTag, PRODUCTION_DIALOG_DESC Dialog)
{
	PRODUCTION_DIALOG_DESC* pDialog = Find_Dialog(strDialogTag);
	if (pDialog != nullptr)
		return E_FAIL;

	m_mapDialogue.emplace(strDialogTag, Dialog);

	return S_OK;
}

void CProduction::Render_Dialog(const wstring& strDialogTag)
{
	PRODUCTION_DIALOG_DESC* pDialog = Find_Dialog(strDialogTag);
	if (pDialog != nullptr)
		return;

	m_pGameInstance->Render_Font((*pDialog).iFontTag, (*pDialog).strText, (*pDialog).vPosition, XMLoadFloat4(&(*pDialog).vColor),
		(*pDialog).fScale, (*pDialog).vOrigin, (*pDialog).fRotation);
}

CGameObject* CProduction::Find_Actor(const wstring& strActorTag)
{
	auto& iter = m_mapActor.find(strActorTag);
	if(iter == m_mapActor.end())
		return nullptr;
	
	return iter->second;
}

CProduction::PRODUCTION_DIALOG_DESC* CProduction::Find_Dialog(const wstring& strDialogTag)
{
	auto& iter = m_mapDialogue.find(strDialogTag);
	if (iter == m_mapDialogue.end())
		return nullptr;

	return &iter->second;
}

void CProduction::Free()
{
	__super::Free();

	/*for (auto& iter : m_mapActor)
		Safe_Release(iter.second);
	m_mapActor.clear();*/

	Safe_Release(m_pGameInstance);
}
