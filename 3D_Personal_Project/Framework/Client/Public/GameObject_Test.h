#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CStateMachine;
class CVIBuffer_Rect;
class CShader;

END

BEGIN(Client)

class CGameObject_Test final : public CGameObject
{	
public:
	enum STATE {STATE1,STATE2,STATE_END};

private:
	CGameObject_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject_Test(const CGameObject_Test& rhs);
	virtual ~CGameObject_Test() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT	Add_Component();
	HRESULT	Add_Event();
	HRESULT	Add_State();

public:
	void	Event_Test1();
	void	Event_Test2();
public:
	void	Action_Test1();
	void	Action_Test2();

private:
	CStateMachine*		m_pStateMachine = { nullptr };
	CVIBuffer_Rect*		m_pVIBuffer_Rect = { nullptr };
	CShader*			m_pShader = { nullptr };
public:
	/* 원형객체를 생성한다. */
	static CGameObject_Test* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);

	/* 사본객체를 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;

	virtual void Free() override;
};

END