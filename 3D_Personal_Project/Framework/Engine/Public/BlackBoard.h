#pragma once
#include "Base.h"

BEGIN(Engine)

class CBlackBoard final : public CBase
{
private:
	CBlackBoard();
	virtual	~CBlackBoard() = default;

public:
	HRESULT	Initialize();

public:
	void	Set_Bool(wstring& strBoolTag, _bool bValue);
	_bool	Get_Bool(wstring& strBoolTag);
	_bool	Find_Bool(wstring& strBoolTag);

	void	Set_Int(wstring& strIntTag, _int iValue);
	_int	Get_Int(wstring& strIntTag);
	_bool	Find_Int(wstring& strIntTag);

	void	Set_Float(wstring& strFloatTag, _float fValue);
	_float	Get_Float(wstring& strFloatTag);
	_bool	Find_Float(wstring& strFloatTag);

	void	Set_Double(wstring& strDoubleTag, _double dValue);
	_double	Get_Double(wstring& strDoubleTag);
	_bool	Find_Double(wstring& strDoubleTag);

	void	Set_WString(wstring& strWStringTag, wstring wstrValue);
	wstring	Get_WString(wstring& strWStringTag);
	_bool	Find_WString(wstring& strWStringTag);

	void	Set_Owner(class CGameObject* pOwner);
	class CGameObject* Get_Owner();

	void	Set_Target(class CGameObject* pTarget);
	class CGameObject* Get_Target();

private:
	unordered_map<wstring, _bool>		m_umapBools;
	unordered_map<wstring, _int>		m_umapInts;
	unordered_map<wstring, _float>	m_umapFloats;
	unordered_map<wstring, _double>	m_umapDoubles;
	unordered_map<wstring, wstring>	m_umapWStrings;

private:
	class CGameObject*						m_pOwner = { nullptr };
	class CGameObject*						m_pTarget = { nullptr };

public:
	static	CBlackBoard* Create();
	virtual	void	Free() override;
};

END

/*
	- bool, int, float, double, wstring 자료형들을 저장하는 unordered_map
	- 소유자를 가지고 있다
		- 게임오브젝트 객체
	- 타겟을 가지고 있다.
		- 게임오브젝트 객체
*/