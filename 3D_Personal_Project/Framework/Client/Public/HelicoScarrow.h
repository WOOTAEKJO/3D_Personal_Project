#pragma once
#include "Monster.h"

BEGIN(Client)

class CHelicoScarrow final : public CMonster
{
public:
	enum STATE {DEAD,DIVE,DIVE_IDLE,EXHAUSTED_HIT,EXHAUSTED_IDLE,EXHAUSTED_WAKE,
	HELICO_ATTACK,HELICO_END,HELICO_START,IDLE,DIVE_UP,GROUND_SMASH,STATE_END};

private:
	CHelicoScarrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHelicoScarrow(const CHelicoScarrow& rhs);
	virtual	~CHelicoScarrow() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID) override;

private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;
	virtual HRESULT	Ready_State() override;

public:
	static	CHelicoScarrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END