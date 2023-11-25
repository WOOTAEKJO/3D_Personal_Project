#include "..\Public\BlackBoard.h"
#include "GameObject.h"

CBlackBoard::CBlackBoard()
{
}

HRESULT CBlackBoard::Initialize()
{
	return S_OK;
}

void CBlackBoard::Set_Bool(wstring& strBoolTag, _bool bValue)
{
	if (!Find_Bool(strBoolTag))
		m_umapBools.emplace(strBoolTag, bValue);
}

_bool CBlackBoard::Get_Bool(wstring& strBoolTag)
{
	if (!Find_Bool(strBoolTag))
		return false;
	
	return m_umapBools[strBoolTag];
}

_bool CBlackBoard::Find_Bool(wstring& strBoolTag)
{
	return m_umapBools.find(strBoolTag) != m_umapBools.end();
}

void CBlackBoard::Set_Int(wstring& strIntTag, _int iValue)
{
	if (!Find_Int(strIntTag))
		m_umapInts.emplace(strIntTag, iValue);
}

_int CBlackBoard::Get_Int(wstring& strIntTag)
{
	if (!Find_Int(strIntTag))
		return 0;

	return m_umapInts[strIntTag];
}

_bool CBlackBoard::Find_Int(wstring& strIntTag)
{
	return m_umapInts.find(strIntTag) != m_umapInts.end();
}

void CBlackBoard::Set_Float(wstring& strFloatTag, _float fValue)
{
	if (!Find_Float(strFloatTag))
		m_umapFloats.emplace(strFloatTag, fValue);
}

_float CBlackBoard::Get_Float(wstring& strFloatTag)
{
	if (!Find_Float(strFloatTag))
		return 0.f;

	return m_umapFloats[strFloatTag];
}

_bool CBlackBoard::Find_Float(wstring& strFloatTag)
{
	return m_umapFloats.find(strFloatTag) != m_umapFloats.end();
}

void CBlackBoard::Set_Double(wstring& strDoubleTag, _double dValue)
{
	if (!Find_Double(strDoubleTag))
		m_umapDoubles.emplace(strDoubleTag, dValue);
}

_double CBlackBoard::Get_Double(wstring& strDoubleTag)
{
	if (!Find_Double(strDoubleTag))
		return 0.f;

	return m_umapDoubles[strDoubleTag];
}

_bool CBlackBoard::Find_Double(wstring& strDoubleTag)
{
	return m_umapDoubles.find(strDoubleTag) != m_umapDoubles.end();
}

void CBlackBoard::Set_WString(wstring& strWStringTag, wstring wstrValue)
{
	if (!Find_WString(strWStringTag))
		m_umapWStrings.emplace(strWStringTag, wstrValue);
}

wstring CBlackBoard::Get_WString(wstring& strWStringTag)
{
	if (!Find_WString(strWStringTag))
		return TEXT(" ");

	return m_umapWStrings[strWStringTag];
}

_bool CBlackBoard::Find_WString(wstring& strWStringTag)
{
	return m_umapWStrings.find(strWStringTag) != m_umapWStrings.end();
}

void CBlackBoard::Set_Owner(CGameObject* pOwner)
{
	if (m_pOwner == nullptr) {
		m_pOwner = pOwner;
		Safe_AddRef(m_pOwner);
	}
}

CGameObject* CBlackBoard::Get_Owner()
{
	if (m_pOwner == nullptr)
		return nullptr;

	return m_pOwner;
}

void CBlackBoard::Set_Target(CGameObject* pTarget)
{
	m_pTarget = pTarget;
}

CGameObject* CBlackBoard::Get_Target()
{
	if (m_pTarget == nullptr)
		return nullptr;

	return m_pTarget;
}

CBlackBoard* CBlackBoard::Create()
{
	CBlackBoard* pInstance = new CBlackBoard();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CBlackBoard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackBoard::Free()
{
	__super::Free();

	Safe_Release(m_pOwner);
	Safe_Release(m_pTarget);

	m_umapBools.clear();
	m_umapInts.clear();
	m_umapFloats.clear();
	m_umapDoubles.clear();
	m_umapWStrings.clear();
}
