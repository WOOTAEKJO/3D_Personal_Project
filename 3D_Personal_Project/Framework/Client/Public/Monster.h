#pragma once
#include "Character.h"

BEGIN(Engine)

class CAICom;

END

BEGIN(Client)

class CMonster abstract : public CCharacter
{
public:
	enum STATE {IDLE, MOVE, ATTACK, DELAY, DEAD, STATE_END};

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

protected:
	CAICom*		m_pAICom = { nullptr };

protected:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;
	virtual HRESULT	Ready_State();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END