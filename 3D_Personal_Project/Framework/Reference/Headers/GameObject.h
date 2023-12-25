#pragma once

#include "Base.h"
#include "GameInstance.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{ // Ŭ���̾�Ʈ���� ��ӹ޾ƾ� �ϱ� ������ Ŭ���̾�Ʈ�� �˰� �־�� �Ѵ�. ENGINE_DLL ���
public:
	typedef	struct tagGameObjectDesc
	{
		tagGameObjectDesc() {}
		tagGameObjectDesc(_float fSpeed, _float fRotation) :fSpeedPerSec(fSpeed),fRotationPerSec(fRotation) {}

		_float	fSpeedPerSec = 0.f;
		_float	fRotationPerSec = 0.f;

	}GAMEOBJECT_DESC;

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
public:
	template<typename T>
	T* Get_Component()
	{
		CComponent* pComponent = Find_Component(TAG_NAME<T>());
		if (pComponent == nullptr)
		{
			MSG_BOX("Failed Find Component");
			return nullptr;
		}

		return dynamic_cast<T*>(pComponent);
	}

public:
	void	Set_WorldMatrix(_float4x4 matWorld);
	_float4x4 Get_WorldMatrix();

public:
	void	Set_Dead() { m_bDead = true; }
	_bool	Get_Dead() { return m_bDead; }

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

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
	_bool						m_bDead = { false };

protected:
	_bool						m_isCloned = { false };
	// Ŭ������ �ƴ����� �Ǵ�

protected:
	HRESULT	Add_Component(_uint iLevelIndex,const wstring& strPrototypeTag,
		const wstring& strComTag, _Inout_ class CComponent** pOut,void* pArg = nullptr);

	template<typename T>
	HRESULT Add_Component(const wstring& strPrototypeTag, T** pCom, void* pArg = nullptr)
	{
		class CComponent* pComponent = Find_Component(TAG_NAME<T>());
		if (pComponent != nullptr)
			return E_FAIL;

		class CComponent* Clone = m_pGameInstance->Add_Component_Clone(m_pGameInstance->Get_Current_Level()
			, strPrototypeTag, pArg);
		if (Clone == nullptr)
			return E_FAIL;

		*pCom = dynamic_cast<T*>(Clone);

		m_mapComponent.emplace(TAG_NAME<T>(), Clone);

		Safe_AddRef(Clone);

		return S_OK;
	}

	class CComponent* Find_Component(const wstring & strComTag);
	HRESULT Delete_Component(const wstring& strComTag);

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