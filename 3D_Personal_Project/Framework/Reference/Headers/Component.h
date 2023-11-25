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

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

protected:
	class CGameInstance*	m_pGameInstance = { nullptr };

protected:
	bool		m_bClone = { false };

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