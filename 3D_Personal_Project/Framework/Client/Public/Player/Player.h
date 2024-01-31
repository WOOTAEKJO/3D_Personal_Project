#pragma once
#include "Character.h"

BEGIN(Engine)

class CController;

END

BEGIN(Client)

class CPlayer final : public CCharacter
{
public:
	enum STATE { IDLE, RUN, ATTACK1, ATTACK2, ATTACK3, AIR_ATTACK, JUMP, DOUBLEJUMP ,FALL,
		LAND, ROLL, STATE_END };

	enum KEY_STATE {KEY_FRONT, KEY_BACK, KEY_RIGHT, KEY_LEFT, KEY_JUMP, KEY_LB_ATTACK,
		KEY_RB_ATTACK, KEY_ROLL,KEY_STATE_END};

	enum WEAPON_TYPE {TYPE_SPEAR, TYPE_SHOVEL,TYPE_END };

	enum PARTS_TYPE {PARTS_BODY,PARTS_WEAPON,PARTS_END};

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
	CGameObject* Find_Parts(PARTS_TYPE ePartsTag);
	CModel* Get_BodyModel();
	CTransform* Get_BodyTransform();
	CCollider* Get_WeaponCollider();
	_uint		Get_NextAttackID() { return m_iAttackID; }
	_int	Get_CurrentState();

public:
	void	Animation_By_Type(STATE eType);
	void	Create_Range_Bullet();

public:
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID) override;

private:
	CController*	m_pControllerCom = { nullptr };

private:
	map<PARTS_TYPE, CGameObject*>	m_mapParts;

private:
	typedef map<STATE, _uint> ANIMINDEX;
	map<WEAPON_TYPE, ANIMINDEX>	m_mapTypeAnimation;
	WEAPON_TYPE			m_eCurrentWeaponType = { TYPE_END };

private:
	_uint				m_iAttackID = { CPlayer::STATE::ATTACK1 };



private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;
	virtual HRESULT	Init_Point_Light() override;

private:
	HRESULT	Ready_State();
	HRESULT	Ready_Parts();
	HRESULT	Ready_Controller();
	virtual HRESULT	Ready_Animation() override;
	virtual	HRESULT	Ready_Act() override;
	HRESULT	Add_Parts(const wstring& strPrototypeTag, PARTS_TYPE ePartsTag, void* pArg = nullptr);
	HRESULT	Add_WeaponType_By_Animation(WEAPON_TYPE eWeaponType, STATE eStateType, _uint iAnimIndex);
	_int Find_AnimIndex(WEAPON_TYPE eWeaponType, STATE eStateType);

private:
	void			NextAttackID();

private:
	void			ShadowLight_SetUp();

public:
	static	CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END