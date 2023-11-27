#include "..\Public\GameInstance.h"
#include "Graphic_Device.h"
#include "TImer_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Event_Manager.h"
#include "Mouse_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext)
{
	/* �׷��� ����̽��� �ʱ�ȭ ����.*/
	m_pGraphic_Device = CGraphic_Device::Create(GraphicDesc, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* Ÿ�̸Ӹ� ����� �غ� ����. */
	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	/* ������ ����� �غ� */
	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	/* ������Ʈ �Ŵ��� ��� �غ�*/
	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	/* ������ ��� �غ�*/
	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	/* �̺�Ʈ �Ŵ��� ��� �غ�*/
	m_pEvent_Manager = CEvent_Manager::Create();
	if (nullptr == m_pEvent_Manager)
		return E_FAIL;

	/* ������Ʈ �Ŵ��� ��� �غ�*/
	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	/* ���콺 �Ŵ��� ��� �غ�*/
	m_pMouse_Manager = CMouse_Manager::Create();
	if (nullptr == m_pMouse_Manager)
		return E_FAIL;

	/* ���������� ��� �غ�*/
	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Priority_Tick(fTimeDelta);
	m_pObject_Manager->Tick(fTimeDelta);
	m_pPipeLine->Tick();
	m_pObject_Manager->Late_Tick(fTimeDelta);

	m_pLevel_Manager->Tick(fTimeDelta);
}

HRESULT CGameInstance::Render_Engine()
{
	if (nullptr == m_pLevel_Manager || nullptr == m_pRenderer)
		return E_FAIL;

	m_pRenderer->Draw_RenderGroup();

#ifdef _DEBUG
	m_pLevel_Manager->Render();
#endif

	return S_OK;
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	if (m_pObject_Manager == nullptr||
		m_pComponent_Manager == nullptr)
		return;

	m_pComponent_Manager->Clear(iLevelIndex);
	m_pObject_Manager->Clear(iLevelIndex);

}

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

HRESULT CGameInstance::Open_Level(_uint iCurrentLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iCurrentLevelIndex,pNewLevel);
}

HRESULT CGameInstance::Add_ProtoType(const wstring& strProtoTypeTag, CGameObject* pGameObeject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_ProtoType(strProtoTypeTag, pGameObeject);
}

HRESULT CGameInstance::Add_Clone(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strProtoTypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Clone(iLevelIndex, strLayerTag, strProtoTypeTag, pArg);
}

HRESULT CGameInstance::Add_RenderGroup(CRenderer::RENDERGROUP eRenderID, CGameObject* pGameObject)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	return m_pRenderer->Add_RenderGroup(eRenderID, pGameObject);
}

HRESULT CGameInstance::Add_Event(const wstring& strEventTag, function<void()> pFunction)
{
	if (nullptr == m_pEvent_Manager)
		return E_FAIL;

	return m_pEvent_Manager->Add_Event(strEventTag, pFunction);
}

HRESULT CGameInstance::Execute_Event(const wstring& strEventTag)
{
	if (nullptr == m_pEvent_Manager)
		return E_FAIL;

	return m_pEvent_Manager->Execute_Event(strEventTag);
}

HRESULT CGameInstance::Add_Component_ProtoType(const _uint& iLevelIndex, const wstring& strProtoTypeTag, CComponent* pComponent)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Component_ProtoType(iLevelIndex, strProtoTypeTag, pComponent);
}

CComponent* CGameInstance::Add_Component_Clone(const _uint& iLevelIndex, const wstring& strProtoTypeTag, void* pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Add_Component_Clone(iLevelIndex, strProtoTypeTag, pArg);
}

void CGameInstance::Update_Mouse(HWND hWnd)
{
	if (nullptr == m_pMouse_Manager)
		return;

	return m_pMouse_Manager->Update_Mouse(hWnd);
}

_bool CGameInstance::Intersect(_float3* pOut, _fvector vV1, _fvector vV2, _fvector vV3, _matrix matWorld)
{
	if (nullptr == m_pMouse_Manager)
		return false;

	return m_pMouse_Manager->Intersect( pOut, vV1, vV2, vV3, matWorld);
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _float4x4 matMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eState, matMatrix);
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix matMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eState, matMatrix);
}

_float4x4 CGameInstance::Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_Transform_Float4x4(eState);
}

_matrix CGameInstance::Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_Transform_Matrix(eState);
}

_float4x4 CGameInstance::Get_Transform_Float4x4_Inverse(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4x4();

	return m_pPipeLine->Get_Transform_Float4x4_Inverse(eState);
}

_matrix CGameInstance::Get_Transform_Matrix_Inverse(CPipeLine::TRANSFORMSTATE eState)
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_Transform_Matrix_Inverse(eState);
}

_float4 CGameInstance::Get_Camera_Pos()
{
	if (nullptr == m_pPipeLine)
		return _float4();

	return m_pPipeLine->Get_Camera_Pos();
}

void CGameInstance::Release_Manager()
{
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pMouse_Manager);
	Safe_Release(m_pEvent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pGraphic_Device);
}

void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->Release_Manager();
	// �����Լ��̱� ������ this�� ������� ���Ѵ�. �׷��� ������ �Լ��� �ϳ� �� ���� �Ŵ������� ���������ְ� �ִ�.
	// �Ŵ����� Ŭ�������� gameinstance�� �����ϴ� ��ü�� �ֱ� ������ ���� ������ �Ŀ� ��� ������ ��������
	// ���������� gameinstance�� destroy���ش�.
		// -> ���۷��� ī��Ʈ�� ���� 0���� �������� ���������� �������� ����
	CGameInstance::GetInstance()->DestroyInstance();
}
