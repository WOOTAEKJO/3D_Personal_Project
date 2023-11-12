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
	//wstring					m_strResultStateTag;		// ���ǿ� �´� ���¸� ��Ÿ��
	//_uint					m_iResultStateID = { 0 };	// ���ǿ� �´� ���¸� ��Ÿ��
private:
	function<bool()>	Find_Transition(const _uint& iResultStateID);
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