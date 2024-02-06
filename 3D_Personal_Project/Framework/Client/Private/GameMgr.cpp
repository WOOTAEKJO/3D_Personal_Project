#include "stdafx.h"
#include "..\Public\GameMgr.h"

#include "GameInstance.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CGameMgr)

CGameMgr::CGameMgr()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CGameMgr::Initialize()
{
	return S_OK;
}

void CGameMgr::Tick()
{
	Is_End_Game();
}

HRESULT CGameMgr::Add_GameToken(CGameObject* pToken)
{
	if (pToken == nullptr)
		return E_FAIL;

	m_listToken.push_back(pToken);

	++m_iListCount;

	return S_OK;
}

void CGameMgr::Start_Game(GAME_EVENT_DESC Event_Desc)
{
	m_Event_Desc = Event_Desc;

	m_bGameActivate = true;

	m_pGameInstance->Execute_Event(m_Event_Desc.strStartEventName);
}

void CGameMgr::Is_End_Game()
{
	if (!m_bGameActivate || m_listToken.empty())
		return;

	_uint iCount = 0;

	for (auto& iter : m_listToken)
	{
		/*if (!iter->Get_Dead())
			return;*/
		if ((iter == nullptr) || iter->Get_Dead())
			++iCount;

	}

	if (iCount == m_iListCount)
	{
		m_pGameInstance->Execute_Event(m_Event_Desc.strEndEventName);
		m_bGameActivate = false;
		m_iListCount = 0;
		m_listToken.clear();
	}

}

void CGameMgr::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
}

