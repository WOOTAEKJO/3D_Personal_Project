#include "..\Public\GameInstance.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "TImer_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Event_Manager.h"
#include "Mouse_Manager.h"
#include "SaveLoad_Manager.h"
#include "Collider_Manager.h"
#include "Font_Manager.h"
#include "RenderTarget_Manager.h"
#include "Light_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const wstring& strFilePath, const GRAPHIC_DESC& GraphicDesc, HINSTANCE hInst,
	_Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext)
{
	/* 그래픽 디바이스를 초기화 하자.*/
	m_pGraphic_Device = CGraphic_Device::Create(GraphicDesc, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	/* 인풋 디바이스를 준비*/
	m_pInput_Device = CInput_Device::Create(hInst, GraphicDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;

	/* 타이머를 사용할 준비를 하자. */
	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	/* 레벨을 사용할 준비 */
	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	/* 오브젝트 매니저 사용 준비*/
	m_pObject_Manager = CObject_Manager::Create(iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	/* 렌더 타겟 매니저 사용 준비*/
	m_pRenderTarget_Manager = CRenderTarget_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderTarget_Manager)
		return E_FAIL;

	/* 렌더러 사용 준비*/
	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	/* 이벤트 매니저 사용 준비*/
	m_pEvent_Manager = CEvent_Manager::Create();
	if (nullptr == m_pEvent_Manager)
		return E_FAIL;

	/* 컴포넌트 매니저 사용 준비*/
	m_pComponent_Manager = CComponent_Manager::Create(iNumLevels);
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	/* 마우스 매니저 사용 준비*/
	m_pMouse_Manager = CMouse_Manager::Create(GraphicDesc.hWnd, GraphicDesc.iBackBufferSizeX, GraphicDesc.iBackBufferSizeY);
	if (nullptr == m_pMouse_Manager)
		return E_FAIL;

	/* 파이프라인 사용 준비*/
	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	/* 세이브 및 로드 사용 준비*/
	m_pSaveLoad_Manager = CSaveLoad_Manager::Create();
	if (nullptr == m_pSaveLoad_Manager)
		return E_FAIL;

	/* 파일 매니저 사용 준비*/
	m_pFile_Manager = CFile_Manager::Create(strFilePath);
	if (nullptr == m_pFile_Manager)
		return E_FAIL;

	/* 콜라이더 매니저 사용 준비*/
	m_pCollider_Manager = CCollider_Manager::Create();
	if (nullptr == m_pCollider_Manager)
		return E_FAIL;

	/* 폰트 매니저 사용 준비*/
	m_pFont_Manager = CFont_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	/* 라이트 매니저 사용 준비*/
	m_pLight_Manager = CLight_Manager::Create();
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pDevice = *ppDevice;
	m_pContext = *ppContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

void CGameInstance::Tick_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager || nullptr == m_pObject_Manager)
		return;
	m_pInput_Device->Update_InputDev();

	m_pObject_Manager->Priority_Tick(fTimeDelta);

	m_pObject_Manager->Tick(fTimeDelta);
	m_pPipeLine->Tick();

	m_pObject_Manager->Late_Tick(fTimeDelta);
	m_pCollider_Manager->Update();
	
	m_pLevel_Manager->Tick(fTimeDelta);

	m_pInput_Device->LateUpdate_InputDev();

	m_pCollider_Manager->Delete_Collider();
	m_pObject_Manager->Delete_Objects();
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
	//m_pCollider_Manager->Clear();
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

ID3D11RenderTargetView* CGameInstance::Get_BackBuffer()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;

	return m_pGraphic_Device->Get_BackBuffer();
}

ID3D11DepthStencilView* CGameInstance::Get_DSV()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;

	return m_pGraphic_Device->Get_DSV();
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseState(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}

bool CGameInstance::Key_Pressing(_ubyte byKeyID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Key_Pressing(byKeyID);
}

bool CGameInstance::Key_Down(_ubyte byKeyID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Key_Down(byKeyID);
}

bool CGameInstance::Key_Up(_ubyte byKeyID)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Key_Up(byKeyID);
}

bool CGameInstance::Mouse_Pressing(MOUSEKEYSTATE eMouse)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Mouse_Pressing(eMouse);
}

bool CGameInstance::Mouse_Down(MOUSEKEYSTATE eMouse)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Mouse_Down(eMouse);
}

bool CGameInstance::Mouse_Up(MOUSEKEYSTATE eMouse)
{
	if (nullptr == m_pInput_Device)
		return false;

	return m_pInput_Device->Mouse_Up(eMouse);
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

void CGameInstance::Set_CurNavigationTag(const wstring& strNavigationTag)
{
	if (nullptr == m_pLevel_Manager)
		return;

	m_pLevel_Manager->Set_CurNavigationTag(strNavigationTag);
}

wstring CGameInstance::Get_CurNavigationTag()
{
	if (nullptr == m_pLevel_Manager)
		return wstring();

	return m_pLevel_Manager->Get_CurNavigationTag();
}

HRESULT CGameInstance::Add_ProtoType(const wstring& strProtoTypeTag, CGameObject* pGameObeject)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_ProtoType(strProtoTypeTag, pGameObeject);
}

HRESULT CGameInstance::Add_Clone(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strProtoTypeTag, void* pArg, CGameObject** ppOut)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Clone(iLevelIndex, strLayerTag, strProtoTypeTag, ppOut, pArg);
}

CGameObject* CGameInstance::Add_Independent_Clone(const wstring& strProtoTypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Add_Independent_Clone(strProtoTypeTag, pArg);
}

_uint CGameInstance::Get_Current_Level()
{
	if (nullptr == m_pObject_Manager)
		return _uint();

	return m_pObject_Manager->Get_Current_Level();
}

void CGameInstance::Set_Current_Level(_uint iLevel)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Set_Current_Level(iLevel);
}

list<class CGameObject*> CGameInstance::Get_ObjectList(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return list<class CGameObject*>();


	return m_pObject_Manager->Get_ObjectList(iLevelIndex, strLayerTag);
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

CComponent_Manager::PROTOTYPE CGameInstance::Get_Com_ProtoType(const _uint& iLevelIndex)
{
	if (nullptr == m_pComponent_Manager)
		return CComponent_Manager::PROTOTYPE();

	return m_pComponent_Manager->Get_Com_ProtoType(iLevelIndex);
}

void CGameInstance::Update_Mouse()
{
	if (nullptr == m_pMouse_Manager)
		return;

	return m_pMouse_Manager->Update_Mouse();
}

_bool CGameInstance::Intersect(_float3* pOut, _float* fDist, _fvector vV1, _fvector vV2, _fvector vV3, _matrix matWorld)
{
	if (nullptr == m_pMouse_Manager)
		return false;

	return m_pMouse_Manager->Intersect( pOut, fDist, vV1, vV2, vV3, matWorld);
}

_bool CGameInstance::Intersect_Sphere(BoundingSphere* pSphere, _float* fDist)
{
	if (nullptr == m_pMouse_Manager)
		return false;

	return m_pMouse_Manager->Intersect_Sphere(pSphere, fDist);
}

void CGameInstance::Free_Mouse(_float fTimeDelta, _float fMouseSensitivity, CTransform* pTransCom)
{
	if (nullptr == m_pMouse_Manager)
		return;

	m_pMouse_Manager->Free_Mouse(fTimeDelta, fMouseSensitivity, pTransCom);
}

void CGameInstance::Mouse_Move(_float fTimeDelta, _float fMouseSensitivity, CTransform* pTransCom)
{
	if (nullptr == m_pMouse_Manager)
		return;

	m_pMouse_Manager->Mouse_Move(fTimeDelta, fMouseSensitivity, pTransCom);
}

_float4 CGameInstance::Get_WorldMouse_Float4()
{
	if (nullptr == m_pMouse_Manager)
		return _float4();

	return m_pMouse_Manager->Get_WorldMouse_Float4();
}

_vector CGameInstance::Get_WorldMouse_Vector()
{
	if (nullptr == m_pMouse_Manager)
		return _vector();

	return m_pMouse_Manager->Get_WorldMouse_Vector();
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

_float4 CGameInstance::Get_CameraState(CPipeLine::CAMERASTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _float4();

	return m_pPipeLine->Get_CameraState(eState);
}

_vector CGameInstance::Get_CameraState_Mat(CPipeLine::CAMERASTATE eState)
{
	if (nullptr == m_pPipeLine)
		return _vector();

	return m_pPipeLine->Get_CameraState_Mat(eState);
}

HRESULT CGameInstance::Save_Data_Mesh(const _char* strFileName, CMeshData::MESHDATADESC MeshDataDesc)
{
	if (nullptr == m_pSaveLoad_Manager)
		return E_FAIL;

	return m_pSaveLoad_Manager->Save_Data_Mesh(strFileName, MeshDataDesc);
}

HRESULT CGameInstance::Load_Data_Mesh(CVIBuffer* pBuffer, const _char* strFileName)
{
	if (nullptr == m_pSaveLoad_Manager)
		return E_FAIL;

	return m_pSaveLoad_Manager->Load_Data_Mesh(pBuffer, strFileName);
}

HRESULT CGameInstance::Save_Data_Json(const _char* strFilePath, CGameObject* pObject)
{
	if (nullptr == m_pSaveLoad_Manager)
		return E_FAIL;

	return m_pSaveLoad_Manager->Save_Data_Json(strFilePath, pObject);
}

HRESULT CGameInstance::Load_Data_Json(const wstring& strTag, CGameObject* pObject)
{
	if (nullptr == m_pSaveLoad_Manager)
		return E_FAIL;

	return m_pSaveLoad_Manager->Load_Data_Json(strTag, pObject);
}

string CGameInstance::Load_Data_Path(wstring strTag)
{
	if (nullptr == m_pFile_Manager)
		return string();

	return m_pFile_Manager->Load_Data_Path(strTag);
}

string CGameInstance::Load_ShaderFiles_Path(wstring strTag)
{
	if (nullptr == m_pFile_Manager)
		return string();

	return m_pFile_Manager->Load_ShaderFiles_Path(strTag);
}

string CGameInstance::Load_Models_Path(wstring strTag)
{
	if (nullptr == m_pFile_Manager)
		return string();

	return m_pFile_Manager->Load_Models_Path(strTag);
}

string CGameInstance::Load_Texture_Path(wstring strTag)
{
	if (nullptr == m_pFile_Manager)
		return string();

	return m_pFile_Manager->Load_Texture_Path(strTag);
}

HRESULT CGameInstance::Add_Collision(_uint iColLayer, CCollider* pCollider)
{
	if (nullptr == m_pCollider_Manager)
		return E_FAIL;

	return m_pCollider_Manager->Add_Collision(iColLayer, pCollider);
}

HRESULT CGameInstance::Add_Pair_Collision(_uint iSourColLayer, _uint iDestColLayer)
{
	if (nullptr == m_pCollider_Manager)
		return E_FAIL;

	return m_pCollider_Manager->Add_Pair_Collision(iSourColLayer, iDestColLayer);
}

void CGameInstance::Collision_Clear()
{
	if (nullptr == m_pCollider_Manager)
		return;

	m_pCollider_Manager->Clear();
}

HRESULT CGameInstance::Add_Font(_uint iFontTag, const wstring& strFontFilePath)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Add_Font(iFontTag, strFontFilePath);
}

HRESULT CGameInstance::Render(_uint iFontTag, const wstring& strText, _float2 vPosition, _fvector vColor, _float fScale, _float2 vOrigin, _float fRotation)
{
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return m_pFont_Manager->Render(iFontTag, strText, vPosition, vColor, fScale, vOrigin, fRotation);
}

HRESULT CGameInstance::Add_RenderTarget(RTV_TYPE eType, _uint iSizeX, _uint iSizeY, DXGI_FORMAT Pixel_Format, const _float4& vColor)
{
	if (nullptr == m_pRenderTarget_Manager)
		return E_FAIL;

	return m_pRenderTarget_Manager->Add_RenderTarget(eType, iSizeX, iSizeY, Pixel_Format, vColor);
}

HRESULT CGameInstance::Add_MRT(const wstring& strMRTTag, RTV_TYPE eType)
{
	if (nullptr == m_pRenderTarget_Manager)
		return E_FAIL;

	return m_pRenderTarget_Manager->Add_MRT(strMRTTag, eType);
}

HRESULT CGameInstance::Begin_MRT(const wstring& strMRTTag)
{
	if (nullptr == m_pRenderTarget_Manager)
		return E_FAIL;

	return m_pRenderTarget_Manager->Begin_MRT(strMRTTag);
}

HRESULT CGameInstance::End_MRT()
{
	if (nullptr == m_pRenderTarget_Manager)
		return E_FAIL;

	return m_pRenderTarget_Manager->End_MRT();
}

HRESULT CGameInstance::Bind_RenderTarget_ShaderResource(RTV_TYPE eType, CShader* pShader, const _char* pConstantName)
{
	if (nullptr == m_pRenderTarget_Manager)
		return E_FAIL;

	return m_pRenderTarget_Manager->Bind_ShaderResource(eType, pShader, pConstantName);
}

HRESULT CGameInstance::Ready_RTV_Debug(RTV_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	if (nullptr == m_pRenderTarget_Manager)
		return E_FAIL;

	return m_pRenderTarget_Manager->Ready_Debug(eType, fX, fY, fSizeX, fSizeY);
}

HRESULT CGameInstance::Render_MRT_Debug(const wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pBuffer)
{
	if (nullptr == m_pRenderTarget_Manager)
		return E_FAIL;

	return m_pRenderTarget_Manager->Render_Debug(strMRTTag, pShader, pBuffer);
}

HRESULT CGameInstance::Add_Light(const LIGHT_DESC& eLightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(eLightDesc);
}

HRESULT CGameInstance::Render_Light(CShader* pShader, CVIBuffer_Rect* pBuffer)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Render(pShader, pBuffer);
}

void CGameInstance::Release_Manager()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pFile_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pCollider_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pEvent_Manager);
	Safe_Release(m_pMouse_Manager);
	Safe_Release(m_pSaveLoad_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pRenderTarget_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pInput_Device);
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
