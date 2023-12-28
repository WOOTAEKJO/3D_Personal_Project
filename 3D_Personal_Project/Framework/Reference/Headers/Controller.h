#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CController final : public CComponent
{
private:
	CController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CController(const CController& rhs);
	virtual	~CController() = default;

public:
	virtual	HRESULT	Initialize_ProtoType();
	virtual	HRESULT	Initialize(void* pArg);

public:
	HRESULT	Add_ControllKey(_uint iKeyTag,_int iKeyValue);
	_bool	Key_Down(_uint iKeyTag);
	_bool	Key_Pressing(_uint iKeyTag);
	_bool	Mouse_Down(_uint iKeyTag);
	_bool	Mouse_Pressing(_uint iKeyTag);

private:
	map<_uint, _int>	m_mapKey;

private:
	_int	Find_Key(_uint iKeyTag);

public:
	static	CController* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END