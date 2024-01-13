#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CModel_Instancing;
class CShader;

END

BEGIN(Client)

class CPlateform_Instancing final : public CGameObject
{
public:
	typedef struct tagPlateform_Instancing_Desc
	{
		wstring		strModelTag;
		_float4		vPos;
		vector<_float4x4> vecInstanceVertex;

	}PLATEFORM_INSTANCING_DESC;
private:
	CPlateform_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlateform_Instancing(const CPlateform_Instancing& rhs);
	virtual	~CPlateform_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void		Set_ModelTag(const wstring& strModelTag) { m_strModelTag = strModelTag; }
	wstring		Get_ModelTag() { return m_strModelTag; }

private:
	CShader*				m_pShaderCom = { nullptr };
	CModel_Instancing*		m_pModelCom = { nullptr };

private:
	wstring		m_strModelTag;
	_float4		m_vObjectPos = {};	

private:
	vector<_float4x4>	m_vecInstanceVertex;

private:
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Component() ;

public:
	static CPlateform_Instancing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END