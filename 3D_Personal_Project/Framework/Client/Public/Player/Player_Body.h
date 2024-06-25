#pragma once
#include "GameObject.h"

/*
	�÷��̾��� ��ü ������ �����ϴ� Ŭ�����Դϴ�.
	CGameObject Ŭ������ ��ӹ޾ҽ��ϴ�.
*/

BEGIN(Engine)

class CTransform;
class CModel;
class CShader;

END

BEGIN(Client)

class CPlayer_Body final : public CGameObject
{	
public:
	typedef struct tagPlayer_Body_Desc
	{
		CTransform* pParentsTransform = nullptr;

	}PLAYERBODY_DESC; // �÷��̾� ��ü ����ü
private:
	CPlayer_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // ����̽��� ����̽� ���ؽ�Ʈ�� �޾� �ʱ�ȭ
	CPlayer_Body(const CPlayer_Body& rhs); // ���� ������
	virtual	~CPlayer_Body() = default; // ���� �Ҹ���

public:
	virtual HRESULT Initialize_Prototype() override; // ���� ��ü �ʱ�ȭ
	virtual HRESULT Initialize(void* pArg) override; // �纻 ��ü �ʱ�ȭ
	virtual void Priority_Tick(_float fTimeDelta) override; // �켱���� ƽ
	virtual void Tick(_float fTimeDelta) override; // �Ϲ����� ƽ
	virtual void Late_Tick(_float fTimeDelta) override; // ���� ƽ
	virtual HRESULT Render() override; // ��ü ����
	virtual HRESULT Render_Shadow() override; // ��ü �׸��� ����
	virtual HRESULT Render_Blur() override; // ��ü �� ����

public:
	virtual void Write_Json(json& Out_Json) override; // ���̽� ���� ����
	virtual void Load_FromJson(const json& In_Json) override; // ���̽� �ε�

	HRESULT	Load_Data(const _char* strFilePath); // Ư�� ���� �ּ��� ���̽� ���� �ε�

public:
	void Set_AnimationIndex(_uint iAnimIndex); // �ִϸ��̼� ��ȯ

private:
	CModel*		m_pModelCom = { nullptr };
	CShader*	m_pShaderCom = { nullptr };

private:
	CTransform* m_pParentsTransform = { nullptr };
	_float4x4	m_matWorldMat = {}; // ��� �� ���̴��� ���ε��� �������

private:
	HRESULT Bind_ShaderResources(); // ���̴� ���ҽ� ���ε�
	HRESULT Ready_Component(); // ������Ʈ �غ�

public:
	static	CPlayer_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); // ���� ��ü ����
	virtual CGameObject* Clone(void* pArg) override; // �纻 ��ü ����
	virtual	void	Free() override; // �޸� ����
};

END