#pragma once
#include "VIBuffer_Rect.h"
#include "Shader.h"
#include "StateMachine.h"

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
private:
	CComponent_Manager();
	virtual	~CComponent_Manager()=default;

public:
	HRESULT	Initialize(const _uint& iLevelNum);
	HRESULT	Add_Component_ProtoType(const _uint& iLevelIndex,const wstring& strProtoTypeTag, class CComponent* pComponent);
	class CComponent*	Add_Component_Clone(const _uint& iLevelIndex, const wstring& strProtoTypeTag,void* pArg);
	void	Priority_Tick(_float fTimeDelta);
	void	Tick(_float fTimeDelta);
	void	Late_Tick(_float fTimeDelta);

private:
	map<const wstring, class CComponent*>* m_mapCom_ProtoType;
	typedef	map<const wstring, class CComponent*> PROTOTYPE;
	vector<class CComponent*>	m_vecClone;		// 임시 테스트를 위한 용도/ 추하 수정해야 함 무조건

private:
	_uint				m_iLevelNum = { 0 };

private:
	class CComponent*	Find_Com_ProtoType(const _uint& iLevelIndex, const wstring& strProtoTypeTag);

public:
	static	CComponent_Manager* Create(const _uint& iLevelNum);
	virtual	void	Free() override;
};

END