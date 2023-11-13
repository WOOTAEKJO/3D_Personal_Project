#include "stdafx.h"
#include "..\Public\GameObject_Test.h"
#include "GameInstance.h"
#include "StateMachine.h"

#include "Test_State1.h"
#include "Test_State2.h"

CGameObject_Test::CGameObject_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CGameObject_Test::CGameObject_Test(const CGameObject_Test& rhs)
	: CGameObject(rhs)
{

}

HRESULT CGameObject_Test::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CGameObject_Test::Initialize(void* pArg)
{

#pragma region TEST

	if (FAILED(Add_Component()))
		return E_FAIL;
	/*if (FAILED(Add_Event()))
		return E_FAIL;*/
	if (FAILED(Add_State()))
		return E_FAIL;
	
#pragma endregion

	return S_OK;
}

void CGameObject_Test::Priority_Tick(_float fTimeDelta)
{
	int a = 10;
}

void CGameObject_Test::Tick(_float fTimeDelta)
{
	
	int a = 10;

	/*if (GetKeyState('1') & 0x8000)
	{
		m_pGameInstance->Execute_Event(TEXT("TEST1"));
	}

	if (GetKeyState('2') & 0x8000)
	{
		m_pGameInstance->Execute_Event(TEXT("TEST2"));
	}*/
}

void CGameObject_Test::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_PRIORITY, this)))
		return;
}

HRESULT CGameObject_Test::Render()
{
	int a = 10;

	return S_OK;
}

HRESULT CGameObject_Test::Add_Component()
{
	m_pStateMachine = dynamic_cast<CStateMachine*>(m_pGameInstance->Add_Component_Clone(TEXT("StateMachine")));
	if (m_pStateMachine == nullptr)
		return E_FAIL;
	
	return S_OK;
}

HRESULT CGameObject_Test::Add_Event()
{
	if(FAILED(m_pGameInstance->Add_Event(TEXT("TEST1"), [this]() {
		this->Event_Test1();
		})))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Event(TEXT("TEST2"), [this]() {
		this->Event_Test2();
		})))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CGameObject_Test::Add_State()
{
	if (FAILED(m_pStateMachine->Add_State(STATE::STATE1, CTest_State1::Create(this,m_pStateMachine))))
		return E_FAIL;

	if (FAILED(m_pStateMachine->Add_State(STATE::STATE2, CTest_State2::Create(this, m_pStateMachine))))
		return E_FAIL;
	
	if (FAILED(m_pStateMachine->Init_State(STATE::STATE1)))
		return E_FAIL;
	
	return S_OK;
}

void CGameObject_Test::Event_Test1()
{
	int a = 10;
	SetWindowText(g_hWnd, TEXT("TEST1입니다."));
}
void CGameObject_Test::Event_Test2()

{
	SetWindowText(g_hWnd, TEXT("TEST2입니다."));
}

void CGameObject_Test::Action_Test1()
{
	SetWindowText(g_hWnd, TEXT("상태1 행동입니다."));
}

void CGameObject_Test::Action_Test2()
{
	SetWindowText(g_hWnd, TEXT("상태2 행동입니다."));
}

CGameObject_Test* CGameObject_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGameObject_Test* pInstance = new CGameObject_Test(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGameObject_Test");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CGameObject_Test::Clone(void* pArg)
{
	CGameObject_Test* pInstance = new CGameObject_Test(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGameObject_Test");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGameObject_Test::Free()
{
	__super::Free();

}

