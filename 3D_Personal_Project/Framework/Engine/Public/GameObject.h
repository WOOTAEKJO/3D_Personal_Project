#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{ // Ŭ���̾�Ʈ���� ��ӹ޾ƾ� �ϱ� ������ Ŭ���̾�Ʈ�� �˰� �־�� �Ѵ�. ENGINE_DLL ���
protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	// ������ ������ �� ����̽����� �޾ƿ��� ������
	CGameObject(const CGameObject& rhs);
	// �纻�� ������ �� ������ ������ �����ϴ� ���� ������
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype(); // ���� �ʱ�ȭ
	virtual HRESULT Initialize(void* pArg); // �纻 �ʱ�ȭ
	virtual void Priority_Tick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	// ������Ʈ�� ����, �߰�, �������� ������ ����Ѵ�.

	virtual HRESULT Render();

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

protected:
	class CGameInstance*		m_pGameInstance = { nullptr };

protected:
	class CTransform*			m_pTransformCom = { nullptr };

protected:
	map<const wstring, class CComponent*>	m_mapComponent;

protected:
	_bool						m_isCloned = { false };
	// Ŭ������ �ƴ����� �Ǵ�

protected:
	HRESULT	Add_Component(_uint iLevelIndex,const wstring& strPrototypeTag,
		const wstring& strComTag, _Inout_ class CComponent** pOut,void* pArg = nullptr);
	class CComponent* Find_Component(const wstring & strComTag);
	
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END

/*
	������ Ÿ���� ����ϴ� ����
		- ������ �������� �����͵��� �����Ѵ�.(�Ǵ� ���� �����)
			-> �����͵��� Ŭ���̾�Ʈ���� �����ϸ� ���ȿ� ���� ����
		- ���⼭ ���͸� �����Ѵٰ� ���� �� �������� �����͵��� ���ͼ� ����������� �Ѵ�.
		- ������ �������� �����͵��� ������ ������ �ӵ��� ���� ������.
		- �Ź� ���͵��� ������ ������ �̷��� ���ָ� ������ ��� ���ߴ� ������ �߻��� �� �ִ�.
		- �׷��� ������ Ÿ���� ����ϴµ�
		- ������ �ε��ϴ� �������� �������� �����͸� �޾� ������ �����.
		- �׸��� �ʿ��� ������ �纻�� ����� ����ϴµ�, �� �� �̹� �������� ���� �����͵��� �ֱ� ������
			�ӵ� ���ϰ� �Ͼ�� �ʴ´�.

	������ Ÿ�� ������ �纻
		- ������ �纻�� �ֱ� ������ ������, ���� �Լ�, �ʱ�ȭ �Լ��� ���� ������ش�.
			- ����
				- ����̽� �������� �޾ƿ��� ������ -> default ������
				- ������ �����ϴ� create �Լ�
					-> ������, �ʱ�ȭ �Լ����� �ʿ��� ������ ���ڷ� ���´�.
				- ������ �ʱ�ȭ ��Ű�� Initializer_Prototype �Լ�
					-> ���������� �ʿ��� �͵��� �ʱ�ȭ
						��) ���� �Ǵ� ���� ����¿��� ���� ������
			- �纻
				- ������ �����͵��� �����ؼ� �����ϴ� ���� ������
				- �纻�� �����ϴ� Clone �Լ�
					-> ���� ������ void*���� �� �ִµ�, �̴� � ������ �޾� �� �� �ְ� �Ϸ��� �ǵ��� �ִ�.
						��) �÷��̾��� �ּ� ��, ��ġ ������ ����Ǿ� �ִ� ������ �迭 ���
				- �纻�� �ʱ�ȭ ��Ű�� Initialize �Լ�
					-> ���������� �ʿ��� �͵��� �ƴ� �纻���� �ʿ��� �͵�
						��) �������� ���� �÷��̾ �Ѵ� ����
							-> �̷� ���� �÷��̾��� �������� �ʿ��ϱ� ������ ������ �� ���ڰ����� 
								�÷��̾��� �ּҸ� �޾� �´�.
*/