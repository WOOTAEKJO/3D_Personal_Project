#include "..\Public\GameInstance.h"
#include "Graphic_Device.h"
#include "TImer_Manager.h"
#include "Level_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext)
{
	/* 그래픽 디바이스를 초기화 하자.*/
	m_pGraphic_Device = CGraphic_Device::Create(GraphicDesc, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 타이머를 사용할 준비를 하자. */
	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	/* 레벨을 사용할 준비 */
	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager)
		return;

	m_pLevel_Manager->Tick(fTimeDelta);
}

#ifdef _DEBUG

void CGameInstance::Render_Engine()
{
	if (nullptr == m_pLevel_Manager)
		return;

	m_pLevel_Manager->Render();
}

#endif

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Present();
}

HRESULT CGameInstance::Add_Timer(const wstring & strTimeTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimeTag);
}

_float CGameInstance::Compute_TimeDelta(const wstring & strTimeTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.0f;

	return m_pTimer_Manager->Compute_TimeDelta(strTimeTag);
}

HRESULT CGameInstance::Open_Level(CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(pNewLevel);	
}

void CGameInstance::Release_Manager()
{
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pGraphic_Device);
}

void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->Release_Manager();
	// 전역함수이기 때문에 this를 사용하지 못한다. 그렇기 때문에 함수를 하나 더 만들어서 매니저들을 해제시켜주고 있다.
	// 매니저급 클래스에서 gameinstance를 참조하는 객체가 있기 때문에 먼저 릴리즈 후에 모든 참조가 없어지면
	// 마지막으로 gameinstance를 destroy해준다.
		// -> 레퍼런스 카운트를 먼저 0으로 만들어줘야 실질적으로 지워지기 때문
	CGameInstance::GetInstance()->DestroyInstance();
}
