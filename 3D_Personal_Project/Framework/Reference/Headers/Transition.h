#pragma once
#include "Base.h"

BEGIN(Engine)

class CTransition final : public CBase
{
private:
	CTransition();
	virtual	~CTransition() = default;
public:
	HRESULT	Initialize();
	HRESULT	Add_Transition(function<bool()> pFunction,const _uint& iResultStateID);
	bool	Is_Transition(_uint* iResultStateID);
private:
	map<const _uint, function<bool()>>	m_mapTransition;
private:
	//wstring					m_strResultStateTag;		// 조건에 맞는 상태를 나타냄
	//_uint					m_iResultStateID = { 0 };	// 조건에 맞는 상태를 나타냄
private:
	function<bool()>	Find_Transition(const _uint& iResultStateID);
public:
	static	CTransition* Create();
	virtual	void	Free() override;

};

END

/*
	- 조건들을 저장하는 클래스
	- 어떤 상태에서 어떤 조건을 만족하면 다른 상태로 전이하기 위한 조건들을 저장
	- 조건들을 계속 순회하면서 맞는 조건이 있으면 true를 반환

*/