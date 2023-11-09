#pragma once
#include "Base.h"

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP {RENDER_PRIORITY,RENDER_NONLIGHT,RENDER_NONBLEND,RENDER_BLEND,RENDER_UI,RENDER_END};
	// �׸��� ������� ���� ����ü�� ����
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
	list<class CGameObject*>	m_listRenderObject[RENDERGROUP::RENDER_END];

private:
	HRESULT Render_Priority();
	HRESULT Render_NonLight();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend();
	HRESULT Render_UI();
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