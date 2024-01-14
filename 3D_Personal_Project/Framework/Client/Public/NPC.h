#pragma once
#include "Character.h"

BEGIN(Client)

class CNPC abstract : public CCharacter
{
public:
	enum STATE { IDLE, FOLLOW, ATTACK, TALK, STATE_END };

	enum NPC_TYPE { CROW, OWL, NPC_TYPE_END };

protected:
	CNPC(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNPC(const CNPC& rhs);
	virtual	~CNPC() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:

	void	PlayerLook();
	void	Target_Follow_Look(_bool bGround = true);
	_bool	Turn(_float fTimeDelta);
	_bool	Is_Target_Range(_float fRange);
	void	Target_Follow(_float fTimeDelta);
	_float	Distance_Proportional(_float fMin, _float fMax, _float fRange);

	_bool	Is_Target_State(_uint iTargetState);

	void	Trans_Attack(_bool bCheck);

public:
	void		Set_ModelTag(const wstring& strModelTag) { m_strModelTag = strModelTag; }
	wstring		Get_ModelTag() { return m_strModelTag; }

	NPC_TYPE	Get_NPCType() { return m_eNPCType; }

protected:
	class CPlayer*	m_pPlayer = { nullptr };
	CTransform*		m_pPlayer_Transform = { nullptr };

protected:
	_float4			m_vTargetPos = {};

protected:
	NPC_TYPE		m_eNPCType = { NPC_TYPE::NPC_TYPE_END };

protected:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;
	virtual HRESULT	Ready_State();
	virtual HRESULT	Ready_Animation() override;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END