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
	HRESULT	Add_Transition(function<bool()> pFunction,const wstring& strResultStateTag);
	bool	Is_Transition(wstring*	strResultStateTag);
private:
	map<const wstring, function<bool()>>	m_mapTransition;
private:
	wstring					m_strResultStateTag;		// ���ǿ� �´� ���¸� ��Ÿ��
private:
	function<bool()>	Find_Transition(const wstring& strResultStateTag);
public:
	static	CTransition* Create();
	virtual	void	Free() override;

};

END

/*
	- ���ǵ��� �����ϴ� Ŭ����
	- � ���¿��� � ������ �����ϸ� �ٸ� ���·� �����ϱ� ���� ���ǵ��� ����
	- ���ǵ��� ��� ��ȸ�ϸ鼭 �´� ������ ������ true�� ��ȯ

*/