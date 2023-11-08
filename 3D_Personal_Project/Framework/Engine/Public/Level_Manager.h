#pragma once

#include "Base.h"

/* ���� ������ ƽ, ������ ������ ȣ���Ѵ�. */

/* Open_Level() */
/* ���� ȭ�鿡 ��������� ������ �ּҸ� ���´�. */
/* ���� ������ �����Ѵ�. */
/* ���� ������ �ڿ����� �����Ѵ�. */

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();

public:
	HRESULT Open_Level(class CLevel* pNewLevel);

private:
	class CLevel*				m_pCurrentLevel = { nullptr };

public:
	static CLevel_Manager* Create();
	virtual void Free() override;
};

END