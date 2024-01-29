#pragma once
#include "NPC.h"

BEGIN(Client)

class CCrow final : public CNPC
{	
private:
	CCrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCrow(const CCrow& rhs);
	virtual	~CCrow() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Set_IDLE_Pos();
	void	Player_Head_Pos();

	void	Attack_Cool(_float fCool,_float fTimeDelta);
	_bool	Attack_Input();

	_bool	Find_Range_Monster(_float fRange);

	_bool	Is_Col();
	void	Reset_Col() { m_bCol = false; }
public:
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID) override;

private:
	_bool	m_bAttack = { true };
	_float	m_fAttackTime = { 0.f };

private:
	CGameObject* m_pTrailEffect = { nullptr };

private:
	_bool	m_bCol = { false };

private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT	Ready_Component() override;
	virtual HRESULT	Ready_State() override;
	virtual HRESULT	Ready_Animation() override;

public:
	static	CCrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END