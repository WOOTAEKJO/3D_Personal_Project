#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameInstance;
class CShader;
class CVIBuffer_Rect;
class CComponent;

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP {RENDER_PRIORITY, RENDER_SHADOW, RENDER_BLUR,
		RENDER_NONLIGHT,RENDER_NONBLEND,RENDER_BLEND,RENDER_UI,RENDER_END};
	// �׸��� ������� ���� ����ü�� ����
private:
	CRenderer(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	virtual	~CRenderer() = default;

public:
	HRESULT	Initialize();
	HRESULT	Add_RenderGroup(RENDERGROUP eRenderID, class CGameObject* pGameObject);
	HRESULT	Add_DebugRender(CComponent* pComponent);
	HRESULT	Draw_RenderGroup();

	void	Fog_SetUp(_float2 vForStart_End, _float4 vFogColor);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	CGameInstance*		m_pGameInstance = { nullptr };

private:
	list<class CGameObject*>	m_listRenderObject[RENDERGROUP::RENDER_END];

private:
	CVIBuffer_Rect*				m_pBufferCom = {nullptr	};
	CShader*					m_pShader = {nullptr};

private:
	_float4x4					m_matWorld;
	_float4x4					m_matView, m_matProj;

#ifdef _DEBUG
private:
	list<CComponent*>			m_listComponent;
#endif 

private:
	_float2						m_vFogPosition = {0.f,0.f};
	_float4						m_vFogColor = {1.f,1.f,1.f,1.f};

private:
	ID3D11DepthStencilView* m_pLightDepthDSV = { nullptr };	// ������ ���� Ÿ�� ���� ���� ����

private:
	HRESULT Render_Priority();
	HRESULT Render_Shadow();
	HRESULT Render_NonLight();
	HRESULT Render_NonBlend();
	HRESULT Render_LightAcc();
	HRESULT Render_Deferred();
	HRESULT Render_Bloom();
	HRESULT Render_Blend();
	HRESULT Render_UI();

#ifdef _DEBUG
	HRESULT	Render_Debug();
	HRESULT	Ready_RTV_Debug();

#endif

private:
	HRESULT	Ready_Component();
	HRESULT	Create_DepthStencil();

public:
	static  CRenderer*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	void		Free() override; 
};

END

/*
	- ���� �׷캰�� ��ü���� ����
		- list�����̳� ���
			- ����, ������ ����� �Ͼ�� ����
	- ���� �׷캰�� ��ü���� ���� �Լ��� ȣ��
	- �׷��� ��ġ���� ������ Ȱ��
		- ����
*/