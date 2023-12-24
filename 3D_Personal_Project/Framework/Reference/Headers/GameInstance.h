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

public: /* For.Graphic_Device */		
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT Clear_DepthStencil_View();	
	HRESULT Present();

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

public: /* For.Object_Manager */
	HRESULT	Add_ProtoType(const wstring & strProtoTypeTag, class CGameObject* pGameObeject);
	HRESULT	Add_Clone(_uint iLevelIndex, const wstring & strLayerTag, const wstring & strProtoTypeTag, void* pArg = nullptr, CGameObject * *ppOut = nullptr);
	CGameObject* Add_Independent_Clone(const wstring & strProtoTypeTag, void* pArg = nullptr);
	_uint	Get_Current_Level();
	void	Set_Current_Level(_uint iLevel);

	template <typename T>
	HRESULT Add_GameObject_ProtoType(const wstring & strProtoTypeTag)
	{
		return Add_ProtoType(strProtoTypeTag, T::Create(m_pDevice, m_pContext));
	}


public: /* For.Renderer*/
	HRESULT	Add_RenderGroup(CRenderer::RENDERGROUP eRenderID, class CGameObject* pGameObject);

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
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			CNavigation::Create(m_pDevice, m_pContext, CNavigation::NAVITYPE::TYPE_LOAD,
				Load_Data_Path(strProtoTypeTag).c_str()));
	}

	HRESULT	Add_Navigation_ProtoType_Demo(const wstring& strProtoTypeTag)
	{
		return Add_Component_ProtoType(Get_Current_Level(), strProtoTypeTag,
			CNavigation::Create(m_pDevice, m_pContext, CNavigation::NAVITYPE::TYPE_DEMO, nullptr));
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
	_float4	Get_WorldMouse_Float4();
	_vector	Get_WorldMouse_Vector();

public: /* For.PipeLine*/
	void	Set_Transform(CPipeLine::TRANSFORMSTATE eState, _float4x4 matMatrix);
	void	Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix matMatrix);
	_float4x4 Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE eState);
	_matrix	Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE eState);
	_float4x4	Get_Transform_Float4x4_Inverse(CPipeLine::TRANSFORMSTATE eState);
	_matrix		Get_Transform_Matrix_Inverse(CPipeLine::TRANSFORMSTATE eState);
	_float4		Get_Camera_Pos();

public: /* For. SaveLoad_Manager*/
	HRESULT	Save_Data_Mesh(const _char* strFileName, CMeshData::MESHDATADESC MeshDataDesc);
	HRESULT	Load_Data_Mesh(CVIBuffer* pBuffer, const _char* strFileName);
	HRESULT	Save_Data_Json(const _char* strFilePath, CGameObject* pObject);
	HRESULT	Load_Data_Json(const wstring& strTag, CGameObject* pObject);

public: /* For. File_Manager*/
	string	Load_Data_Path(wstring strTag);
	string	Load_ShaderFiles_Path(wstring strTag);
	string	Load_Models_Path(wstring strTag);
	string	Load_Texture_Path(wstring strTag);


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
	// 매니저급 클래스들을 관리하기 위함


private:
	ID3D11Device* m_pDevice = { nullptr };		
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	void Release_Manager();
	static void Release_Engine();
	// 객체를 이용해서 호출하고 싶지 않기 때문에 전역함수로 만들었다.
	// 당연히 멤벼 변수 사용 x
	// this도 사용 x
};

END