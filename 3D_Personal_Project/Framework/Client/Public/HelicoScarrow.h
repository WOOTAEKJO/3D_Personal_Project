#pragma once
#include "Monster.h"

/*
	HelicoScarrow ���� ���͸� �����ϴ� Ŭ�����Դϴ�.
	CMonster Ŭ������ ��ӹ޾ҽ��ϴ�.
*/

BEGIN(Client)

class CHelicoScarrow final : public CMonster
{
public: // ������ ���¸� ��Ÿ���� ����ü�Դϴ�.
	enum STATE {DEAD,DIVE,DIVE_IDLE,EXHAUSTED_HIT,EXHAUSTED_IDLE,EXHAUSTED_WAKE,
	HELICO_ATTACK,HELICO_END,HELICO_START,IDLE,DIVE_UP,GROUND_SMASH,STATE_END};

private:
	CHelicoScarrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // ����̽��� ����̽� ���ؽ�Ʈ�� �޾� �ʱ�ȭ �մϴ�.
	CHelicoScarrow(const CHelicoScarrow& rhs); // ���� ������
	virtual	~CHelicoScarrow() = default; // ���� �Ҹ���

public:
	virtual HRESULT Initialize_Prototype() override; // ���� ��ü �ʱ�ȭ
	virtual HRESULT Initialize(void* pArg) override; // �纻 ��ü �ʱ�ȭ
	virtual void Priority_Tick(_float fTimeDelta) override; // �켱 ���� ƽ
	virtual void Tick(_float fTimeDelta) override; // �Ϲ����� ƽ
	virtual void Late_Tick(_float fTimeDelta) override; // ���� ƽ
	virtual HRESULT Render() override; // ��ü�� ������
	virtual HRESULT Render_Shadow() override; // ��ü�� �׸��� ����
	virtual HRESULT Render_Blur() override; // ��ü�� �� ����

public:
	_bool	Is_AllHited(_uint iNum); // ��Ʈ ī��Ʈ üũ
	void	Reset_Hited(); // ��Ʈ ī��Ʈ ����
	 
	void	Create_Monster(); // ���� ���� ����
	_bool	Is_SubMonster_AllDead(); // ���� ���Ͱ� ��� �������� �Ǵ�

	void	Dead_CountDown(); // ������ ���� ī��Ʈ ó��

	void	Creat_Bullet(); // �Ѿ� ����
	void	Create_Shock_Wave(); // ����� ����
	void	Shock_Wave_Radius_Compute(); // ����� �ݰ� ���

public: // �浹 �߻� ��
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID) override;

private:
	vector<CGameObject*> m_vecSubMonster; // ���� ���� ���� ����

private:
	_uint		m_iHited_Count[2] = {0,0};
	_bool		m_bSelectBonNum = { true };

private:
	CGameObject* m_pShockWave_Col = { nullptr }; // ����� �浹ü ��ü
	CGameObject* m_pShockWave_Effect = { nullptr }; // ����� ����Ʈ ��ü

private:
	virtual HRESULT Bind_ShaderResources() override; // ���̴� ���ҽ� ���ε�
	virtual HRESULT Ready_Component() override; // ������Ʈ �غ�
	virtual HRESULT	Ready_State() override; // ���� �غ�
	virtual HRESULT	Init_Point_Light() override; // ����Ʈ �ʱ�ȭ

private:
	void	Hide(); // ���� ���� ���� �޼���

public:
	static	CHelicoScarrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // ���� ��ü ����
	virtual CGameObject* Clone(void* pArg) override; // �纻 ��ü ����
	virtual	void	Free() override; // �޸� ����
};

END