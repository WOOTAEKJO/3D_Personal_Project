#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(_bool bLoad,const _char* strNavigationPath);
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual	HRESULT	Render();

public:
	void	Update(_float4x4 matWorld);
	_bool	IsMove(_fvector vPosition);
	HRESULT	Add_Cell(_float3* pPoints);

#ifdef _DEBUG
private:
	class CShader*	m_pShaderCom = { nullptr };
#endif

private:
	vector<class CCell*>	m_vecCell;

private:
	_int					m_iCurrentCellIndex = { -1 };

private:
	static _float4x4		m_matWorld;

private:
	HRESULT	Init_Neighbor();
	HRESULT	File_Load(const _char * strNavigationPath);

public:
	static CNavigation* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,_bool bLoad, const _char* strNavigationPath);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	 void		Free() override;
};

END