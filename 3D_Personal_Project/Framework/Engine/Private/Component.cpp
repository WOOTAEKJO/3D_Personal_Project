#include "..\Public\Component.h"
#include "GameInstance.h"

CComponent::CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),m_pContext(pContext), m_bClone(false),
	m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

CComponent::CComponent(const CComponent& rhs)
	:m_pDevice(rhs.m_pDevice),m_pContext(rhs.m_pContext), m_bClone(true),
	m_pGameInstance(rhs.m_pGameInstance), m_strClassName(rhs.m_strClassName), m_bUseJson(rhs.m_bUseJson)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CComponent::Initialize_ProtoType()
{
	return S_OK;
}

HRESULT CComponent::Initialize(void* pArg)
{
	

	return S_OK;
}

void CComponent::Init_ClassName()
{
	/*m_strClassName = typeid(this).name();
	m_Class_HashCode = typeid(this).hash_code();*/
}

void CComponent::Free()
{
	__super::Free();

	//Safe_Release(m_pOwner);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
