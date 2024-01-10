#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameInstance;
class CShader;
class CVIBuffer_Rect;

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP {RENDER_PRIORITY,RENDER_NONLIGHT,RENDER_NONBLEND,RENDER_BLEND,RENDER_UI,RENDER_END};
	// 그리는 순서대로 렌더 열거체를 만듬
private:
	CRenderer(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	virtual	~CRenderer() = default;

public:
	HRESULT	Initialize();
	HRESULT	Add_RenderGroup(RENDERGROUP eRenderID, class CGameObject* pGameObject);
	HRESULT	Draw_RenderGroup();
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
	_float4x4					m_matView, m_matProj;

private:
	HRESULT Render_Priority();
	HRESULT Render_NonLight();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();

#ifdef _DEBUG
	HRESULT	Render_Debug();
	HRESULT	Ready_RTV_Debug();

#endif

private:
	HRESULT	Ready_Component();

public:
	static  CRenderer*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	void		Free() override; 
};

END

/*
	- 렌더 그룹별로 객체들을 보관
		- list컨테이너 사용
			- 사입, 삭제가 빈번히 일어나기 때문
	- 렌더 그룹별로 객체들의 렌더 함수를 호출
	- 그래픽 장치들을 가지고 활용
		- 추후
*/