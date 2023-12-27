#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(const CComponent& rhs);
	virtual	~CComponent() = default;

public:
	virtual	HRESULT	Initialize_ProtoType();
	virtual	HRESULT	Initialize(void* pArg);

public: /* �ӽ�. ���� ����� ���� ����� ���� ���¿��� ���*/
	virtual void	Priority_Tick(_float fTimeDelta) {};
	virtual void	Tick(_float fTimeDelta) {};
	virtual void	Late_Tick(_float fTimeDelta) {};

public:
	wstring	Get_ClassName() { return m_strClassName; }
	void	Set_ClassName(wstring strName) { m_strClassName = strName; }
	size_t	Get_ClassHashCode() { return m_Class_HashCode; }
	_bool	Get_UseJson() { return m_bUseJson; }

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

protected:
	class CGameInstance*	m_pGameInstance = { nullptr };

protected:
	bool		m_bClone = { false };

	wstring		m_strClassName = {};
	size_t		m_Class_HashCode = { 0 };

protected:
	_bool		m_bUseJson = { false };

protected:
	void		Init_ClassName();

public:
	virtual	CComponent* Clone(void* pArg)=0;
	virtual	void	Free() override;
};

END

/*
	- ������Ʈ���� �ֻ��� �θ�� Ŭ����
	- ������ Ÿ���� ���
		- ������, �ʱ�ȭ �Լ�, ���� �Լ��� ���� �ٸ�
	- Ŭ���̾�Ʈ�� �˾ƾ� �ϱ� ������ engine_dll ���
	- ��ġ �������� �޾Ƽ� �̸� ������ ���´�.
		- �ڽ� ������Ʈ�� �߿��� ����ϴ� �͵��� ���� ������ ����
*/