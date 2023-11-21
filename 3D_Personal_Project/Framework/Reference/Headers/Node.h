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
	- 모든 노드들의 부모격 클래스
	- 노드들은 초기화, 업데이트, 터미네이트 함수를 가지고 있다.
	- 상태를 열거체로 만들어서 사용
	- 블랙보드를 생성하고 추가하는 함수는 만들 예정

*/