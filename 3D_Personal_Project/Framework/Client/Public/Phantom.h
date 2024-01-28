#pragma once
#include "Monster.h"

BEGIN(Client)

class CBullet;

class CPhantom final : public CMonster
{
public:
	enum STATE {ATTAQUE_CHASSE,CHASSE,HIT_CHASSE,IDLE,APPEAR,BOUH,HIT,INTRO,
	LASER,MARTEAU,DEAD,SHOOT,SUMMON,SUMMON_BOMB,SUMMON_LOOP,VANISH,DASH,STATE_END};
	
	enum PHASE { PAHSE1, PAHSE2, PAHSE_END };

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
	PHASE	Get_CurrentPhase() { return m_eCurrentPhase; }
	_uint	Get_PrevState();

	void	Set_SmashTime(_bool bCheck) { m_bSmashTime = bCheck; }

public:
	virtual void Load_FromJson(const json& In_Json) override;

public:
	void	Create_Shock_Wave();
	void	Shock_Wave_Radius_Compute();

	void	Create_Laser();
	void	Update_Laser_Elec();
	void	Delete_Laser();

	void	Adjust_Pos(_float3 vAdjust);

	void	Create_Multiply();
	void	SetUp_Random_Pos();

	void	Appear_OriginPos();
	void	SetUp_OriginPos(_float4 vPos);
	void	Modify_Pos(_float3 vPos);
	void	Appear_PlayerPos();

	void	Create_Bomb();
	void	Start_Point_Toward_Bomb();
	void	Delete_Failed_Bomb();
	void	Delete_Bomb();
	_bool	Is_Bomb_Failed();

	void	Create_TargetBullet();

	void	Add_Hit_Count();
	_bool	Judge_Hit();
	_uint	Get_Hit_Count() { return m_iHitCount; }

	void	Create_Meteor();

	void	Drop_Floor(_uint iFloorType);
	void	Navi_Filter();

	_bool	Is_Target_Near();

	void	Judge_Dead();

public:
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID) override;

private:
	CGameObject*		m_pLaser = { nullptr };
	CGameObject*		m_pBomb[3] = {};

private:
	_float3 m_vRandomPos[20];
	_uint m_iPrevRandomNum = { 0 };

	_float4	m_vOriginPos;

	PHASE m_eCurrentPhase = { PHASE::PAHSE_END };

	_uint				m_iHitCount = { 0 };

private:
	CGameObject* m_pIDLEParicle = { nullptr };

private:
	CGameObject* m_pShockWave_Col = { nullptr };
	CGameObject* m_pShockWave_Effect = { nullptr };

private:
	_bool		m_bSmashTime = { false };

private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;
	virtual HRESULT	Ready_State() override;
	virtual HRESULT	Init_Point_Light() override;

public:
	static	CPhantom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END