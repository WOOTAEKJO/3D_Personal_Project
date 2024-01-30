#pragma once

#include "Renderer.h"
#include "Component_Manager.h"
#include "PipeLine.h"
#include "Json/Json_Utility.h"
#include "File_Manager.h"

#include "Utility_String.h"

/* 클라이언트에서 엔진의 기능을 사용하기위해 반드시 거쳐야하는 객체. */

/* DX11을 통해 필요한 객체를 생성하고 렌더링하는 작업을 수행한다. */
/* 지금 생성한 레벨(씬)을 보관하고 필요에따라 업데이트 수행. */
/* 내가 생성한 객체들을 보관하고 업데이트하고 렌더한다. */
/* 내가 생성한 여러 컴포넌트들의 원형을 보관하고 복제하여 가져온다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.Engine */
	/* 엔진라이브러리를 사용하기위한 준비를 모두 거친다. */
	HRESULT Initialize_Engine(_uint iNumLevels, const wstring & strFilePath,const GRAPHIC_DESC& GraphicDesc, HINSTANCE hInst,
		_Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);
	void Tick_Engine(_float fTimeDelta);
	HRESULT Render_Engine();	// 렌더는 렌더러 클래스를 통해 실행된다.
	void Clear(_uint iLevelIndex);

	void	Random_Float(_float * vVec, _float fMin, _float fMax);
	void	Random_Float2(_float2 * vVec, _float fMin, _float fMax);
	void	Random_Float3(_float3 * vVec, _float fMin, _float fMax);
	void	Random_Float4(_float4 * vVec, _float fMin, _float fMax);

public: /* For.Graphic_Device */		
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();	
	HRESULT Present();
	ID3D11RenderTargetView* Get_BackBuffer();
	ID3D11DepthStencilView* Get_DSV();

public: /* For.Input_Device*/
	_byte   Get_DIKeyState(_ubyte byKeyID);
	_byte   Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long   Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

	bool   Key_Pressing(_ubyte byKeyID);
	bool   Key_Down(_ubyte byKeyID);
	bool   Key_Up(_ubyte byKeyID);

	bool   Mouse_Pressing(MOUSEKEYSTATE eMouse);
	bool   Mouse_Down(MOUSEKEYSTATE eMouse);
	bool   Mouse_Up(MOUSEKEYSTATE eMouse);

public: /* For.Timer_Manager */
	HRESULT	Add_Timer(const wstring& strTimeTag);
	_float Compute_TimeDelta(const wstring& strTimeTag);

public: /* For.Level_Manager */
	HRESULT Open_Level(_uint iCurrentLevelIndex,class CLevel* pNewLevel);
	void	Set_CurNavigationTag(const wstring & strNavigationTag);
	wstring	Get_CurNavigationTag();

public: /* For.Object_Manager */
	HRESULT	Add_ProtoType(const wstring & strProtoTypeTag, class CGameObject* pGameObeject);
	HRESULT	Add_Clone(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strProtoTypeTag, void* pArg = nullptr, CGameObject * *ppOut = nullptr);
	CGameObject* Add_Independent_Clone(const wstring & strProtoTypeTag, void* pArg = nullptr);
	_uint	Get_Current_Level();
	void	Set_Current_Level(_uint iLevel);
	list<class CGameObject*>	Get_ObjectList(_uint iLevelIndex, const wstring & strLayerTag);

	template <typename T>
	HRESULT Add_GameObject_ProtoType(const wstring & strProtoTypeTag)
	{
		return Add_ProtoType(strProtoTypeTag, T::Create(m_pDevice, m_pContext));
	}


public: /* For.Renderer*/
	HRESULT	Add_RenderGroup(CRenderer::RENDERGROUP eRenderID, class CGameObject* pGameObject);
	HRESULT	Add_DebugRender(CComponent* pComponent);

	void	Fog_SetUp(_float2 vForStart_End, _float4 vFogColor);

public: /* For.Event_Manager*/
	HRESULT	Add_Event(const wstring & strEventTag, function<void()> pFunction);
	HRESULT	Execute_Event(const wstring & strEventTag);

public: /* For.Component_Manager*/
	HRESULT	Add_Component_ProtoType(const _uint& iLevelIndex,const wstring & strProtoTypeTag, class CComponent* pComponent);
	class CComponent*	Add_Component_Clone(const _uint & iLevelIndex, const wstring & strProtoTypeTag, void* pArg = nullptr);
	CComponent_Manager::PROTOTYPE Get_Com_ProtoType(const _uint& iLevelIndex);

	template <typename VertexType>
	HRESULT	Add_Shader_ProtoType(const wstring& strProtoTypeTag)
	{
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			CShader::Create(m_pDevice, m_pContext, 
				CUtility_String::string_To_Wstring(Load_ShaderFiles_Path(strProtoTypeTag)),
				VertexType::Elements, VertexType::iElementsNum));
	}

	HRESULT Add_Texture_ProtoType(const wstring& strProtoTypeTag, _uint iNum = 0)
	{
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			CTexture::Create(m_pDevice, m_pContext,
				CUtility_String::string_To_Wstring( Load_Texture_Path(strProtoTypeTag)), iNum));
	}

	HRESULT	Add_Model_ProtoType(const wstring& strProtoTypeTag, _fmatrix matPivot)
	{
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM,
				Load_Models_Path(strProtoTypeTag), matPivot));
	}

	HRESULT	Add_ModelInstancing_ProtoType(const wstring& strProtoTypeTag, _fmatrix matPivot)
	{
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			CModel_Instancing::Create(m_pDevice, m_pContext,
				Load_Models_Path(strProtoTypeTag), matPivot));
	}

	HRESULT	Add_ANIM_Model_ProtoType(const wstring& strProtoTypeTag, _fmatrix matPivot)
	{
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_ANIM,
				Load_Models_Path(strProtoTypeTag), matPivot));
	}

	HRESULT	Add_Terrain_ProtoType_Height(const wstring& strProtoTypeTag, const wstring& strHeightFilePath)
	{
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext, CVIBuffer_Terrain::LOADTYPE::TYPE_HEIGHT, strHeightFilePath));
	}

	HRESULT	Add_Terrain_ProtoType_Binary(const wstring& strProtoTypeTag)
	{
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext, CVIBuffer_Terrain::LOADTYPE::TYPE_BINARY,
				CUtility_String::string_To_Wstring(Load_Data_Path(strProtoTypeTag))));
	}

	HRESULT	Add_Navigation_ProtoType_File(const wstring& strProtoTypeTag)
	{
		Set_CurNavigationTag(strProtoTypeTag);

		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			CNavigation::Create(m_pDevice, m_pContext, CNavigation::NAVITYPE::TYPE_LOAD,
				Load_Data_Path(strProtoTypeTag).c_str()));
	}

	HRESULT	Add_Navigation_ProtoType_Demo(const wstring& strProtoTypeTag)
	{
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			CNavigation::Create(m_pDevice, m_pContext, CNavigation::NAVITYPE::TYPE_DEMO, nullptr));
	}

	HRESULT	Add_Particle_ProtoType(const wstring& strProtoTypeTag)
	{
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			CVIBuffer_Particle_Point::Create(m_pDevice, m_pContext,
				Load_Data_Path(strProtoTypeTag).c_str()));
	}

	template <typename T>
	HRESULT	Add_Buffer_ProtoType(const wstring& strProtoTypeTag)
	{
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			T::Create(m_pDevice, m_pContext));
	}

	template <typename T>
	HRESULT Add_ETC_ProtoType(const wstring& strProtoTypeTag)
	{
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			T::Create(m_pDevice, m_pContext));
	}
	
public: /* For.Mouse_Manager*/
	void	Update_Mouse();
	_bool	Intersect(_float3 * pOut, _float * fDist, _fvector vV1, _fvector vV2, _fvector vV3, _matrix matWorld);
	_bool	Intersect_Sphere(BoundingSphere* pSphere, _float* fDist);
	void	Free_Mouse(_float fTimeDelta, _float fMouseSensitivity, CTransform * pTransCom);
	void	Mouse_Move(_float fTimeDelta, _float fMouseSensitivity, CTransform* pTransCom);
	_float4	Get_WorldMouse_Float4();
	_vector	Get_WorldMouse_Vector();

public: /* For.PipeLine*/
	void	Set_Transform(CPipeLine::TRANSFORMSTATE eState, _float4x4 matMatrix);
	void	Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix matMatrix);
	_float4x4 Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE eState);
	_matrix	Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE eState);
	_float4x4	Get_Transform_Float4x4_Inverse(CPipeLine::TRANSFORMSTATE eState);
	_matrix		Get_Transform_Matrix_Inverse(CPipeLine::TRANSFORMSTATE eState);
	_float4		Get_CameraState(CPipeLine::CAMERASTATE eState);
	_vector		Get_CameraState_Mat(CPipeLine::CAMERASTATE eState);

public: /* For. SaveLoad_Manager*/
	HRESULT	Save_Data_Mesh(const _char* strFileName, CMeshData::MESHDATADESC MeshDataDesc);
	HRESULT	Load_Data_Mesh(CVIBuffer* pBuffer, const _char* strFileName);
	HRESULT	Save_Data_Json(const _char* strFilePath, CGameObject* pObject);
	HRESULT	Load_Data_Json(const wstring& strTag, CGameObject* pObject);
	HRESULT	Save_Data_Particle(const _char* strFilePath, INSTANCING_DESC Dest);
	HRESULT	Load_Data_Particle(const _char* strFilePath, INSTANCING_DESC* pOut);

public: /* For. File_Manager*/
	string	Load_Data_Path(wstring strTag);
	string	Load_ShaderFiles_Path(wstring strTag);
	string	Load_Models_Path(wstring strTag);
	string	Load_Texture_Path(wstring strTag);

public: /* For. Collider_Manager*/
	HRESULT	Add_Collision(_uint iColLayer, CCollider* pCollider);
	HRESULT	Add_Pair_Collision(_uint iSourColLayer, _uint iDestColLayer);
	void	Collision_Clear();

public: /* For. Font_Manager*/
	HRESULT	Add_Font(_uint iFontTag, const wstring& strFontFilePath);
	HRESULT	Render_Font(_uint iFontTag, const wstring& strText, _float2 vPosition, 
		_fvector vColor = XMVectorSet(1.f,1.f,1.f,1.f),
		_float fScale = 1.f, _float2 vOrigin = _float2(0.f,0.f), _float fRotation = 0.f);

public: /* For. CRednerTarget_Manager*/
	HRESULT	Add_RenderTarget(RTV_TYPE eType, _uint iSizeX, _uint iSizeY, DXGI_FORMAT Pixel_Format, const _float4& vColor);
	HRESULT	Add_MRT(const wstring& strMRTTag, RTV_TYPE eType);
	HRESULT	Begin_MRT(const wstring& strMRTTag);
	HRESULT	End_MRT();
	HRESULT Bind_RenderTarget_ShaderResource(RTV_TYPE eType, CShader* pShader, const _char* pConstantName);

#ifdef _DEBUG
	HRESULT	Ready_RTV_Debug(RTV_TYPE eType, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT	Render_MRT_Debug(const wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pBuffer);

#endif

public: /* For. CLight_Manager*/
	HRESULT	Add_Light(const LIGHT_DESC& eLightDesc, _Out_ class CLight** ppLight = nullptr);
	void Delete_Light(CLight* ppLight);
	HRESULT	Render_Light(CShader* pShader, CVIBuffer_Rect* pBuffer);

public: /* For. Camera_Manager*/
	HRESULT	Add_Camera(const wstring& strCameraTag, class CCamera* pCamera);
	void	SetUp_Camera_Offset(_float3 vOffset);

public: /* For. Frustum*/
	void	Transform_ToLocalSpace_Frustum(_fmatrix matWorld);
	_bool	IsIn_Local_FrustumPlanes(_fvector vPoint, _float fRadius);
	_bool	IsIn_World_FrustumPlanes(_fvector vPoint, _float fRadius);

public: /* For. Production_Manager*/
	HRESULT	Add_Production(const wstring& strProductionTag, class CProduction* pProduction);
	HRESULT	Add_Actor(const wstring& strProductionTag, const wstring& strActorTag, CGameObject* pActor);
	void	SetUp_Production(const wstring& strProductionTag);
	void	Finish_Production();

private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CInput_Device*			m_pInput_Device = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CRenderer*				m_pRenderer = { nullptr };
	class CEvent_Manager*			m_pEvent_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CMouse_Manager*			m_pMouse_Manager = { nullptr };
	class CPipeLine*				m_pPipeLine = { nullptr };
	class CSaveLoad_Manager*		m_pSaveLoad_Manager = { nullptr };
	class CFile_Manager*			m_pFile_Manager = { nullptr };
	class CCollider_Manager*		m_pCollider_Manager = { nullptr };
	class CFont_Manager*			m_pFont_Manager = { nullptr };
	class CRenderTarget_Manager*	m_pRenderTarget_Manager = { nullptr };
	class CLight_Manager*			m_pLight_Manager = { nullptr };
	class CCamera_Manager*			m_pCamera_Manager = { nullptr };
	class CFrustum*					m_pFrustum = { nullptr };
	class CProduction_Manager*		m_pProduction_Manager = { nullptr };
	// 매니저급 클래스들을 관리하기 위함


private:
	ID3D11Device* m_pDevice = { nullptr };		
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	random_device	m_RandomDevice;
	mt19937_64		m_RandomNumber;

public:
	void Release_Manager();
	static void Release_Engine();
	// 객체를 이용해서 호출하고 싶지 않기 때문에 전역함수로 만들었다.
	// 당연히 멤벼 변수 사용 x
	// this도 사용 x
};

END