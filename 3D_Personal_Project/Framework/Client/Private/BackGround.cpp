#include "stdafx.h"
#include "..\Public\BackGround.h"
#include "GameInstance.h"
#include "StateMachine.h"


CBackGround::CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CBackGround::CBackGround(const CBackGround & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBackGround::Initialize_Prototype()
{
	/* 원형객체의 초기화과정을 수행한다. */
	/* 1.서버로부터 값을 받아와서 초기화한다 .*/
	/* 2.파일입출력을 통해 값을 받아온다.*/

	return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{

#pragma region TEST

	m_pGameInstance->Add_Event(TEXT("TEST1"), [this]() {
		this->Test1();
		});
	m_pGameInstance->Add_Event(TEXT("TEST2"), [this]() {
		this->Test2();
		});

	m_pStateMachine = dynamic_cast<CStateMachine*>( m_pGameInstance->Add_Component_Clone(TEXT("StateMachine")));

#pragma endregion

	return S_OK;
}

void CBackGround::Priority_Tick(_float fTimeDelta)
{
	int a = 10;
}

void CBackGround::Tick(_float fTimeDelta)
{
	int a = 10;

	if (GetKeyState('1') & 0x8000)
	{
		m_pGameInstance->Execute_Event(TEXT("TEST1"));
	}

	if (GetKeyState('2') & 0x8000)
	{
		m_pGameInstance->Execute_Event(TEXT("TEST2"));
	}
}

void CBackGround::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_PRIORITY, this)))
		return;
}

HRESULT CBackGround::Render()
{
	int a = 10;

	return S_OK;
}

void CBackGround::Test1(void)
{
	int a = 10;
	SetWindowText(g_hWnd, TEXT("TEST1입니다."));
}
void CBackGround::Test2()

{
	SetWindowText(g_hWnd, TEXT("TEST2입니다."));
}

CBackGround * CBackGround::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBackGround*		pInstance = new CBackGround(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBackGround::Clone(void* pArg)
{
	CBackGround*		pInstance = new CBackGround(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBackGround");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();
}

