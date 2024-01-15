#pragma once
#include "Character.h"

BEGIN(Engine)

class CAICom;

END

BEGIN(Client)

class CMonster abstract : public CCharacter
{
public:
	enum STATE {IDLE, MOVE, ATTACK, DELAY,HITED, DEAD,APPEAR, STATE_END};

	enum MONSTER_TYPE {SPOOKETON,SKULLCROSSBOW,MONSTER_TYPE_END};

protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual	~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void	TargetLook();
	_bool	Turn(_float fTimeDelta);
	_bool	Is_Target_Range(_float fRange);

public:
	CCollider* Get_WeaponCollider();
	MONSTER_TYPE	Get_MonsterType() { return m_eMonsterType; }
	CTransform* Get_PlayerTransform() { return m_pPlayer_Transform; }

	void		Set_Activate() { m_bActivate = true; }
	_bool		Is_Activate() { return m_bActivate; }

	_float4x4	Get_Col_WorldMat(); // 콜라이더의 월드행렬을 가져옴

protected:
	CBone*		m_pSocketBone = { nullptr };
	CCollider*	m_pWeaponColliderCom = { nullptr };

protected:
	class CPlayer*	m_pPlayer = { nullptr };
	CTransform*		m_pPlayer_Transform = { nullptr };

protected:
	MONSTER_TYPE		m_eMonsterType = { MONSTER_TYPE::MONSTER_TYPE_END };

protected:
	_bool				m_bActivate = { true };

protected:

protected:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;
	virtual HRESULT	Ready_State();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END