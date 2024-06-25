#pragma once
#include "Character.h"

/*
	���͵��� �θ�� �߻� Ŭ�����Դϴ�.
	CCharacter Ŭ������ ��� �޾ҽ��ϴ�.
*/

BEGIN(Engine)

class CAICom;

END

BEGIN(Client)

class CMonster abstract : public CCharacter
{
public:
	enum STATE {IDLE, MOVE, ATTACK, DELAY,HITED, DEAD,APPEAR, STATE_END};
	// ���͵��� �⺻ ���� ����ü

	enum MONSTER_TYPE {SPOOKETON,SKULLCROSSBOW,MONSTER_TYPE_END};
	// �⺻ ���͵��� Ÿ�� ����ü

protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // ����̽��� ����̽� ���ؽ�Ʈ�� �޾� �ʱ�ȭ
	CMonster(const CMonster& rhs); // ���� ������
	virtual	~CMonster() = default; // ���� �Ҹ���

public:
	virtual HRESULT Initialize_Prototype() override; // ���� ��ü �ʱ�ȭ
	virtual HRESULT Initialize(void* pArg) override; // �纻 ��ü �ʱ�ȭ
	virtual void Priority_Tick(_float fTimeDelta) override; // �켱���� ƽ
	virtual void Tick(_float fTimeDelta) override; // �Ϲ����� ƽ
	virtual void Late_Tick(_float fTimeDelta) override; // ���� ƽ
	virtual HRESULT Render() override; // ��ü�� ����
	virtual HRESULT Render_Shadow() override; // ��ü �׸����� ����

public:
	void	TargetLook(); // Y��ǥ ������� �ʰ� Ÿ���� �������θ� �ٶ󺻴�.
	void	TargetLook_Y(); // Y��ǥ ����Ͽ� Ÿ���� �ٶ󺻴�. 
	_bool	Turn(_float fTimeDelta,_bool bCheck = true); // ȸ��
	_bool	Is_Target_Range(_float fRange); // Ÿ���� ���� ���� �ִ��� üũ

public:
	CCollider* Get_WeaponCollider(); // ���� �ݶ��̴� ��ȯ
	MONSTER_TYPE	Get_MonsterType() { return m_eMonsterType; } // ���� Ÿ�� ��ȯ
	CTransform* Get_PlayerTransform() { return m_pPlayer_Transform; } // �÷��̾��� Ʈ������ ������Ʈ ��ȯ

	void		Set_Activate() { m_bActivate = true; } // ���� Ȱ��ȭ
	_bool		Is_Activate() { return m_bActivate; } // ���� Ȱ��ȭ üũ

	_float4x4	Get_Col_WorldMat(); // �ݶ��̴��� ��������� ������

	void		Start_DeadTime() { m_bDeadTime = true; } // ���� Ÿ�̸� ����

	_bool		Get_DeadTime() { return m_bDeadTime; } // ���� Ÿ�̸� üũ

protected:
	CTexture*	m_pNoiseTextureCom = { nullptr }; // ������ �ؽ��� ������Ʈ

protected:
	CBone*		m_pSocketBone = { nullptr }; // ���� ��
	CCollider*	m_pWeaponColliderCom = { nullptr }; // ���� �ݶ��̴� ������Ʈ

protected:
	class CPlayer*	m_pPlayer = { nullptr }; // �÷��̾� ��ü
	CTransform*		m_pPlayer_Transform = { nullptr }; // �÷��̾��� Ʈ������ ������Ʈ

protected:
	MONSTER_TYPE		m_eMonsterType = { MONSTER_TYPE::MONSTER_TYPE_END };

protected:
	_bool				m_bActivate = { true };
	_bool				m_bDeadTime = { false };

protected: // ������ ���� ����
	_float m_fDissolveAmount = { 0.f };
	_float m_fDissolveGradiationDistance = { 0.f };
	_float3 m_vDissolveGradiationStartColor;
	_float3 m_vDissolveGradiationGoalColor;

protected:
	CGameObject*	m_pLightEffect = { nullptr }; // ����Ʈ ����Ʈ ��ü

protected:
	void	Monster_Dead(_float fTimeDelta); // ���� ���� ó��
	void	Dissolve(_float fAmount, _float fDistance,_float fTimeDelta); // ������ ó��

protected:
	virtual HRESULT Bind_ShaderResources() override; // ���̴� ���ҽ� ���ε�
	virtual HRESULT Ready_Component() override; // ������Ʈ �غ�
	virtual HRESULT	Ready_State(); // �Ϲ����� ���� ���� �غ�


public:
	virtual CGameObject* Clone(void* pArg) = 0; // �纻 ��ü ���� ���� ���� �Լ�
	virtual	void	Free() override; // �޸� ����
};

END