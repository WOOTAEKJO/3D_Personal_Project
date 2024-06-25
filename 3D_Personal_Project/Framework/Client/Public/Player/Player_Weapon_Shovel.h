#pragma once
#include "GameObject.h"

/*
	�÷��̾� ���� ���� �����ϴ� Ŭ�����Դϴ�.
	CGameObject Ŭ������ ��ӹ޾ҽ��ϴ�.

*/

BEGIN(Engine)

class CTransform;
class CModel;
class CShader;
class CBone;
class CCollider;
class CVIBuffer_Trail;

END

BEGIN(Client)

class CPlayer_Weapon_Shovel final : public CGameObject
{	
public:
	typedef struct tagPlayer_Weapon_Shovel_Desc
	{
		vector<CBone*>		pBones;
		CTransform* pParentsTransform = nullptr;
		CGameObject* pOwner = nullptr;

	}PLAYERSHOVEL_DESC; // �� ���� ����ü

private:
	CPlayer_Weapon_Shovel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // ����̽��� ����̽� ���ؽ�Ʈ�� �޾� �ʱ�ȭ
	CPlayer_Weapon_Shovel(const CPlayer_Weapon_Shovel& rhs); // ���� ������
	virtual	~CPlayer_Weapon_Shovel() = default;// ����Ҹ���

public:
	virtual HRESULT Initialize_Prototype() override; // ���� ��ü �ʱ�ȭ
	virtual HRESULT Initialize(void* pArg) override; // �纻 ��ü �ʱ�ȭ
	virtual void Priority_Tick(_float fTimeDelta) override; // �켱���� ƽ
	virtual void Tick(_float fTimeDelta) override; // �Ϲ����� ƽ
	virtual void Late_Tick(_float fTimeDelta) override; // ���� ƽ
	virtual HRESULT Render() override; // ��ü ����
	virtual HRESULT Render_Shadow() override; // ��ü �׸��� ����

public:
	virtual void Load_FromJson(const json& In_Json) override; // ���̽� �ε�

public: // �浹 ���� �޼����
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID) override;

private:
	CModel*		m_pModelCom = { nullptr };
	CShader*	m_pShaderCom = { nullptr };
	CCollider*	m_pColliderCom = { nullptr };

private:
	CBone*		m_pSocketBone = { nullptr };
	_uint		m_iSocketBoneIndex = { 0 };

	CTransform* m_pParentsTransform = { nullptr };
	_float4x4	m_matWorldMat = {}; // ��� �� ���̴��� ���ε��� �������

private:
	wstring		m_strModelTag;

private:
	CGameObject* m_pTrailEffect = { nullptr }; // Ʈ���� ����Ʈ
	CPlayer* m_pOwner = { nullptr };
	_bool		m_bTrail = { true };

private:
	HRESULT Bind_ShaderResources(); // ���̴� ���ҽ� ���ε�
	HRESULT Ready_Component(); // ������Ʈ �غ�

public:
	static	CPlayer_Weapon_Shovel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // ���� ��ü ����
	virtual CGameObject* Clone(void* pArg) override; // �纻 ��ü ����
	virtual	void	Free() override; // �޸� ����
};

END