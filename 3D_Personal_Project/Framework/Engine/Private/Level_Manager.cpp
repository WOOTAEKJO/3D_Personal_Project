#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "GameInstance.h"

CLevel_Manager::CLevel_Manager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CLevel_Manager::Initialize()
{
	return S_OK;
}

void CLevel_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Tick(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render();
}

HRESULT CLevel_Manager::Open_Level(_uint iCurrentLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == pNewLevel)
		return E_FAIL;

	if (m_pCurrentLevel != nullptr) {
		/* 자원들을 정리한다. */
		m_pGameInstance->Clear(m_iCurrentLevelIndex);
	}
	/*
		- 지워지지 말아야 하는 자원들 STATIC인 경우를 생각해서 Clear를 해준다.
		- m_pCurrentLevel가 없는 상태이면 초기 상태(즉 Level_Static)이기 때문에
	*/

	if (0 != Safe_Release(m_pCurrentLevel))
		return E_FAIL;		// 이전 레벨 삭제

	m_pCurrentLevel = pNewLevel;

	m_iCurrentLevelIndex = iCurrentLevelIndex;

	return S_OK;
}

CLevel_Manager * CLevel_Manager::Create()
{
	CLevel_Manager*		pInstance = new CLevel_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Manager");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pCurrentLevel);
}

