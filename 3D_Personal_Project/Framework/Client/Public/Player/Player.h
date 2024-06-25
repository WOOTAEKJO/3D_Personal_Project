#pragma once
#include "Character.h"

/*
	�÷��̾��� ����, ���¸� �����ϴ� Ŭ�����Դϴ�.
	CCharacter Ŭ������ ��ӹ޾ҽ��ϴ�.
*/

BEGIN(Engine)

class CController;

END

BEGIN(Client)

class CPlayer final : public CCharacter
{
public:
	enum STATE { IDLE, RUN, ATTACK1, ATTACK2, ATTACK3, AIR_ATTACK, JUMP, DOUBLEJUMP ,FALL,
		LAND, ROLL, STATE_END };
	// �÷��̾� ���� ����ü

	enum KEY_STATE {KEY_FRONT, KEY_BACK, KEY_RIGHT, KEY_LEFT, KEY_JUMP, KEY_LB_ATTACK,
		KEY_RB_ATTACK, KEY_ROLL,KEY_STATE_END};
	// �÷��̾� Ű ���� ����ü

	enum WEAPON_TYPE {TYPE_SPEAR, TYPE_SHOVEL,TYPE_END };
	// ���� Ÿ�� ����ü

	enum PARTS_TYPE {PARTS_BODY,PARTS_WEAPON,PARTS_END};
	// �÷��̾� ���� Ÿ�� ����ü
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // ����̽��� ����̽� ���ؽ�Ʈ�� �޾� �ʱ�ȭ
	CPlayer(const CPlayer& rhs); // ���� ������
	virtual	~CPlayer() = default; // ���� �Ҹ���

public:
	virtual HRESULT Initialize_Prototype() override; // ���� ��ü �ʱ�ȭ
	virtual HRESULT Initialize(void* pArg) override; // �纻 ��ü �ʱ�ȭ
	virtual void Priority_Tick(_float fTimeDelta) override; // �켱���� ƽ
	virtual void Tick(_float fTimeDelta) override; // �Ϲ����� ƽ
	virtual void Late_Tick(_float fTimeDelta) override; // ���� ƽ
	virtual HRESULT Render() override; // ��ü ����
	
public:
	CGameObject* Find_Parts(PARTS_TYPE ePartsTag); // ������ ã�� �޼���
	CModel* Get_BodyModel(); // �� ���� �� ������Ʈ�� ��ȯ
	CTransform* Get_BodyTransform(); // �� ���� Ʈ������ ������Ʈ�� ��ȯ
	CCollider* Get_WeaponCollider(); // ���� �ݶ��̴� ������Ʈ�� ��ȯ
	_uint		Get_NextAttackID() { return m_iAttackID; } // ���� ���� ���� ��ȯ
	_int	Get_CurrentState(); // ���� ���¸� ��ȯ

public:
	void	Animation_By_Type(STATE eType); // ���¿� �´� �ִϸ��̼����� ��ȯ
	void	Create_Range_Bullet(); // Ư�� �Ѿ� ����

public: // �浹 ���� �޼���
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID) override;

private:
	CController*	m_pControllerCom = { nullptr }; // ��Ʈ�ѷ� ������Ʈ

private:
	map<PARTS_TYPE, CGameObject*>	m_mapParts; // ������ �����ϴ� map �����̳�

private: // ���� Ÿ�Կ� �´� �ִϸ��̼� ����
	typedef map<STATE, _uint> ANIMINDEX;
	map<WEAPON_TYPE, ANIMINDEX>	m_mapTypeAnimation;
	WEAPON_TYPE			m_eCurrentWeaponType = { TYPE_END };

private:
	_uint				m_iAttackID = { CPlayer::STATE::ATTACK1 };

private:
	_int* m_pMaxHP = {nullptr};
	_int* m_pCurHP = { nullptr };

private:
	CGameObject* m_pLightEffect = { nullptr };

private:
	virtual HRESULT Bind_ShaderResources() override; // ���̴� ���ҽ� ���ε�
	virtual HRESULT Ready_Component() override; // ������Ʈ �غ�
	virtual HRESULT	Init_Point_Light() override; // ����Ʈ ����Ʈ �ʱ�ȭ
	HRESULT	Ready_UI(); // UI �غ�

private:
	HRESULT	Ready_State(); // ���� �غ�
	HRESULT	Ready_Parts(); // ���� �غ�
	HRESULT	Ready_Controller(); // ��Ʈ�ѷ� �غ�
	virtual HRESULT	Ready_Animation() override; // �ִϸ��̼� �غ�
	virtual	HRESULT	Ready_Act() override; // �ൿ �غ�
	HRESULT	Add_Parts(const wstring& strPrototypeTag, PARTS_TYPE ePartsTag, void* pArg = nullptr); // ���� �߰�
	HRESULT	Add_WeaponType_By_Animation(WEAPON_TYPE eWeaponType, STATE eStateType, _uint iAnimIndex); // ����Ÿ�Կ� ���� �ִϸ��̼� �߰�
	_int Find_AnimIndex(WEAPON_TYPE eWeaponType, STATE eStateType); // ���� Ÿ�Կ� ���� �ִϸ��̼� �˻�

private:
	void			NextAttackID(); // ���� ���� ������ �غ�

public:
	static	CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // ���� ��ü ����
	virtual CGameObject* Clone(void* pArg) override; // �纻 ��ü ����
	virtual	void	Free() override; // �޸� ����
};

END