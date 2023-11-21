#pragma once
#include "Base.h"

BEGIN(Engine)

class CNode abstract : public CBase
{
public:
	enum STATUS {INVALID, SUCCESS, FAILURE, RUNNING, STATUS_END};
protected:
	CNode();
	CNode(class CBlackBoard* pBlackBoard);
	virtual	~CNode() = default;

public:
	virtual	HRESULT	Initialize() = 0;
	virtual	STATUS	Update() = 0;
	virtual	HRESULT	Terminate(STATUS eStatus) = 0;
	STATUS	Tick();

public:
	void	Set_BlackBoard(class CBlackBoard* pBlackBoard)
	{	m_pBlackBoard = pBlackBoard;	}
	class CBlackBoard* Get_BlackBoard() { return m_pBlackBoard; }

protected:
	STATUS				m_eStatus = { STATUS::INVALID };
	class CBlackBoard*	m_pBlackBoard = { nullptr };

public:
	virtual	void	Free() override;
};

END

/*
	- ��� ������ �θ�� Ŭ����
	- ������ �ʱ�ȭ, ������Ʈ, �͹̳���Ʈ �Լ��� ������ �ִ�.
	- ���¸� ����ü�� ���� ���
	- �����带 �����ϰ� �߰��ϴ� �Լ��� ���� ����

*/