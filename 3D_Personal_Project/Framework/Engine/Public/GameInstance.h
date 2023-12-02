#pragma once

#include "Renderer.h"
#include "Component_Manager.h"
#include "PipeLine.h"

/* Ŭ���̾�Ʈ���� ������ ����� ����ϱ����� �ݵ�� ���ľ��ϴ� ��ü. */

/* DX11�� ���� �ʿ��� ��ü�� �����ϰ� �������ϴ� �۾��� �����Ѵ�. */
/* ���� ������ ����(��)�� �����ϰ� �ʿ信���� ������Ʈ ����. */
/* ���� ������ ��ü���� �����ϰ� ������Ʈ�ϰ� �����Ѵ�. */
/* ���� ������ ���� ������Ʈ���� ������ �����ϰ� �����Ͽ� �����´�. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public: /* For.Engine */
	/* �������̺귯���� ����ϱ����� �غ� ��� ��ģ��. */
	HRESULT Initialize_Engine(_uint iNumLevels,const GRAPHIC_DESC& GraphicDesc, HINSTANCE hInst, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);
	void Tick_Engine(_float fTimeDelta);
	HRESULT Render_Engine();	// ������ ������ Ŭ������ ���� ����ȴ�.
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

public: /* For.Renderer*/
	HRESULT	Add_RenderGroup(CRenderer::RENDERGROUP eRenderID, class CGameObject* pGameObject);

public: /* For.Event_Manager*/
	HRESULT	Add_Event(const wstring & strEventTag, function<void()> pFunction);
	HRESULT	Execute_Event(const wstring & strEventTag);

public: /* For.Component_Manager*/
	HRESULT	Add_Component_ProtoType(const _uint& iLevelIndex,const wstring & strProtoTypeTag, class CComponent* pComponent);
	class CComponent*	Add_Component_Clone(const _uint & iLevelIndex, const wstring & strProtoTypeTag, void* pArg = nullptr);

public: /* For.Mouse_Manager*/
	void	Update_Mouse();
	_bool	Intersect(_float3 * pOut, _float * fDist, _fvector vV1, _fvector vV2, _fvector vV3, _matrix matWorld);
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
	// �Ŵ����� Ŭ�������� �����ϱ� ����

public:
	void Release_Manager();
	static void Release_Engine();
	// ��ü�� �̿��ؼ� ȣ���ϰ� ���� �ʱ� ������ �����Լ��� �������.
	// �翬�� �⺭ ���� ��� x
	// this�� ��� x
};

END