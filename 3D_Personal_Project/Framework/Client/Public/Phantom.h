#pragma once
#include "Monster.h"

BEGIN(Client)

class CBullet;

class CPhantom final : public CMonster
{
public:
	enum STATE {ATTAQUE_CHASSE,CHASSE,HIT_CHASSE,IDLE,APPEAR,BOUH,HIT,INTRO,
	LASER,MARTEAU,DEAD,SHOOT,SUMMON,SUMMON_BOMB,SUMMON_LOOP,VANISH,STATE_END};

private:
	CPhantom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPhantom(const CPhantom& rhs);
	virtual	~CPhantom() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Load_FromJson(const json& In_Json) override;

public:
	void	Create_Shock_Wave();
	void	Create_Laser();
	void	Adjust_Pos(_float3 vAdjust);
	void	Delete_Laser();

public:
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID) override;

private:
	CGameObject*		m_pLaser = { nullptr };

private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;
	virtual HRESULT	Ready_State() override;

public:
	static	CPhantom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END