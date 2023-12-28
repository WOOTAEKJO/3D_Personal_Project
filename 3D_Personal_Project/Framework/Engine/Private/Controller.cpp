#include "..\Public\Controller.h"
#include "GameInstance.h"

CController::CController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CController::CController(const CController& rhs)
	:CComponent(rhs)
{
}

HRESULT CController::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CController::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CController::Add_ControllKey(_uint iKeyTag, _int iKeyValue)
{
	_int iKey = Find_Key(iKeyTag);
	if (iKey != -1)
		return E_FAIL;

	m_mapKey.emplace(iKeyTag, iKeyValue);

	return S_OK;
}

_bool CController::Key_Down(_uint iKeyTag)
{
	return m_pGameInstance->Key_Down(Find_Key(iKeyTag));
}

_bool CController::Key_Pressing(_uint iKeyTag)
{
	return m_pGameInstance->Key_Pressing(Find_Key(iKeyTag));
}

_bool CController::Mouse_Down(_uint iKeyTag)
{
	return m_pGameInstance->Mouse_Down((MOUSEKEYSTATE)Find_Key(iKeyTag));
}

_bool CController::Mouse_Pressing(_uint iKeyTag)
{
	return m_pGameInstance->Mouse_Pressing((MOUSEKEYSTATE)Find_Key(iKeyTag));
}

_int CController::Find_Key(_uint iKeyTag)
{
	auto iter = m_mapKey.find(iKeyTag);
	if (iter == m_mapKey.end())
		return -1;

	return iter->second;
}

CController* CController::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CController* pInstance = new CController(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CController");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CController::Clone(void* pArg)
{
	CController* pInstance = new CController(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Created : CController");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CController::Free()
{
	__super::Free();
}
