#include "..\Public\Level_Manager.h"
#include "Level.h"

CLevel_Manager::CLevel_Manager()
{
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

HRESULT CLevel_Manager::Open_Level(CLevel * pNewLevel)
{
	if (nullptr == pNewLevel)
		return E_FAIL;

	if (0 != Safe_Release(m_pCurrentLevel))
		return E_FAIL;		// 이전 레벨 삭제

	/* 자원들을 정리한다. */

	m_pCurrentLevel = pNewLevel;

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
	Safe_Release(m_pCurrentLevel);
}

