#pragma once
#include "Base.h"

BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public: 
	HRESULT	Initialize(_uint iNumLevel); // 여기서 사본객체보관 컨테이너 동적배열
	HRESULT	Add_ProtoType(const wstring& strProtoTypeTag,class CGameObject* pGameObeject);
	HRESULT	Add_Clone(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strProtoTypeTag, CGameObject** ppOut, void* pArg);
	CGameObject* Add_Independent_Clone(const wstring& strProtoTypeTag, void* pArg);
	void	Priority_Tick(_float fTimeDelta);
	void	Tick(_float fTimeDelta);
	void	Late_Tick(_float fTimeDelta);
	void	Clear(_uint iLevelIndex);

public:
	_uint	Get_Current_Level() { return m_iCurrentLevel; }
	void	Set_Current_Level(_uint iLevel) { m_iCurrentLevel = iLevel; }

private:
	map<const wstring, class CGameObject*> m_mapProtoType;				// 원형 객체 보관
	map<const wstring, class CLayer*>* m_mapLayer = {nullptr};		   // 사본 객체 보관
														   // 동적배열로 할당할 것이기 때무에 포인터형으로 선언
	typedef map<const wstring, class CLayer*> LAYERS;
private:
	_uint	m_iNumLevel = {0};

private:
	_uint	m_iCurrentLevel = { 0 };

private:
	class CGameObject* Find_ProtoType(const wstring& strProtoTypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);
	// 중복 확인
public:
	static	CObject_Manager*	Create(_uint iNumLevel);
	virtual	void				Free() override;
};

END

/*
	역할
		- 원본객체를 보관 및 관리
		- 사본객체를 보관 및 관리
		- 사본객체들의 Tick 관리
		- 특정 레벨의 사본객체들 정리
*/