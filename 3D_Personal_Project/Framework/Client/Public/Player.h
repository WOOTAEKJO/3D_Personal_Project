#pragma once
#include "Character.h"

BEGIN(Engine)

class CStateMachine;
class CRigidBody;

END

BEGIN(Client)

class CPlayer final : public CCharacter
{
public:
	enum STATE {IDLE,RUN,ATTACK,JUMP,STATE_END};
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual	~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CRigidBody* Get_RigidBody() { return m_pRigidBodyCom; }

private:
	CStateMachine*	m_pStateMachineCom = { nullptr };
	CRigidBody*		m_pRigidBodyCom = { nullptr };

private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;

private:
	HRESULT	Ready_State();

public:
	static	CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END