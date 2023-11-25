#include "stdafx.h"
#include "..\Public\GameObject_Test.h"
#include "GameInstance.h"

#include "Test_State1.h"
#include "Test_State2.h"

#include "BehaviorTree.h"

#include "TestAction.h"

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
	__super::Initialize(pArg);

#pragma region TEST

	if (FAILED(Ready_Component()))
		return E_FAIL;
	/*if (FAILED(Add_Event()))
		return E_FAIL;*/
	/*if (FAILED(Add_State()))
		return E_FAIL;*/
	
#pragma endregion

	return S_OK;
}

void CGameObject_Test::Priority_Tick(_float fTimeDelta)
{
	int a = 10;
	CGameObject::Priority_Tick(fTimeDelta);
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
	CGameObject::Tick(fTimeDelta);
}

void CGameObject_Test::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_PRIORITY, this)))
		return;
	CGameObject::Late_Tick(fTimeDelta);
}

HRESULT CGameObject_Test::Render()
{
	Bind_ShaderResources();

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_Buffer();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CGameObject_Test::Ready_Component()
{
	/* For.Com_StateMachine*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"),
		TEXT("Com_StateMachine"), reinterpret_cast<CComponent**>(&m_pStateMachineCom))))
		return E_FAIL;

	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Shader*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VTXPOSTEX"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Ready_Tree()))
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
	if (FAILED(m_pStateMachineCom->Add_State(STATE::STATE1, CTest_State1::Create(this))))
		return E_FAIL;

	if (FAILED(m_pStateMachineCom->Add_State(STATE::STATE2, CTest_State2::Create(this))))
		return E_FAIL;
	
	if (FAILED(m_pStateMachineCom->Init_State(STATE::STATE1)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CGameObject_Test::Ready_Tree()
{

	CAICom::AIDESC AIDesc = {};

	CNode* pNodes[] = {
		CBuilder().Leaf<CTestAction>().Build(),
		CBuilder().Composite<CSequence>().Leaf<CTestAction>().End().Build(),
		CBuilder().Decorator<CSucceeder>().Leaf<CTestAction>().End().Build()
	};

	wstring	pString[] = {
		TEXT("AI1"),TEXT("AI2"),TEXT("AI3")
	};

	AIDesc.ppNodes = pNodes;
	AIDesc.pstrNodeTags = pString;
	AIDesc.iNodeNum = 3;
	
	/* For.Com_AI*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_AI"),
		TEXT("Com_AI"), reinterpret_cast<CComponent**>(&m_pAICom), &AIDesc)))
		return E_FAIL;

	m_pAICom->Set_Tree(TEXT("AI1"));
	
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

HRESULT CGameObject_Test::Bind_ShaderResources()
{
	_float4x4	mat;

	XMStoreFloat4x4(&mat, XMMatrixIdentity());
	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &mat)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &mat)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &mat)))
		return E_FAIL;

	return S_OK;
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

	Safe_Release(m_pStateMachineCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pAICom);

}

