#pragma once
#include "Base.h"

BEGIN(Engine)

class CComponent_Manager final : public CBase
{
private:
	CComponent_Manager();
	virtual	~CComponent_Manager()=default;

public:
	HRESULT	Initialize();
	HRESULT	Add_Component_ProtoType(const wstring& strProtoTypeTag, class CComponent* pComponent);
	class CComponent*	Add_Component_Clone(const wstring& strProtoTypeTag,void* pArg);
	void	Priority_Tick(_float fTimeDelta);
	void	Tick(_float fTimeDelta);
	void	Late_Tick(_float fTimeDelta);

private:
	map<const wstring, class CComponent*> m_mapCom_ProtoType;
	vector<class CComponent*>	m_vecClone;		// 임시 테스트를 위한 용도/ 추하 수정해야 함 무조건

private:
	class CComponent*	Find_Com_ProtoType(const wstring& strProtoTypeTag);

public:
	static	CComponent_Manager* Create();
	virtual	void	Free() override;
};

END