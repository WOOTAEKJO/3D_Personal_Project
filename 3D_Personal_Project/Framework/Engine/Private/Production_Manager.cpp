#include "..\Public\Production_Manager.h"

#include "GameInstance.h"

#include "Production.h"

CProduction_Manager::CProduction_Manager()
    :m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CProduction_Manager::Initialize()
{
    return S_OK;
}

void CProduction_Manager::Tick()
{
    if (m_pCurrentProduction == nullptr)
        return;

    m_pCurrentProduction->Tick();
}

void CProduction_Manager::Render()
{
    if (m_pCurrentProduction == nullptr)
        return;

    m_pCurrentProduction->Render();
}

HRESULT CProduction_Manager::Add_Production(const wstring& strProductionTag, CProduction* pProduction)
{
    CProduction* pProduc = Find_Production(strProductionTag);
    if (pProduc != nullptr)
        return E_FAIL;

    m_mapProduction.emplace(strProductionTag, pProduction);

    return S_OK;
}

HRESULT CProduction_Manager::Add_Actor(const wstring& strProductionTag, const wstring& strActorTag, CGameObject* pActor)
{
    if (pActor == nullptr)
        return E_FAIL;

    CProduction* pProduc = Find_Production(strProductionTag);
    if (pProduc == nullptr)
        return E_FAIL;

    return pProduc->Add_Actor(strActorTag, pActor);
}

void CProduction_Manager::SetUp_Production(const wstring& strProductionTag)
{
    CProduction* pProduc = Find_Production(strProductionTag);
    if (pProduc == nullptr)
        return;

    m_pCurrentProduction = pProduc;
    pProduc->Enter();
}

void CProduction_Manager::Finish_Production()
{
    if (m_pCurrentProduction == nullptr)
        return;

    m_pCurrentProduction->Exite();
    m_pCurrentProduction = nullptr;
}

CProduction* CProduction_Manager::Find_Production(const wstring& strProductionTag)
{
    auto& iter = m_mapProduction.find(strProductionTag);
    if(iter == m_mapProduction.end())
        return nullptr;

    return iter->second;
}

CProduction_Manager* CProduction_Manager::Create()
{
    CProduction_Manager* pInstance = new CProduction_Manager();

    if (FAILED(pInstance->Initialize())) {
        MSG_BOX("Failed to Created : CProduction_Manager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CProduction_Manager::Free()
{
    __super::Free();

    for (auto& iter : m_mapProduction)
        Safe_Release(iter.second);
    m_mapProduction.clear();

    Safe_Release(m_pGameInstance);
}
