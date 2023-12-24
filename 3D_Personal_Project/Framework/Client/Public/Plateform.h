#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CModel;
class CShader;

END

BEGIN(Client)

class CPlateform final : public CGameObject
{
public:
	typedef struct tagPlateform_Desc
	{
		wstring		strModelTag;
		_float4		vPos;

	}PLATEFORM_DESC;
private:
	CPlateform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlateform(const CPlateform& rhs);
	virtual	~CPlateform() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	wstring	Get_ModelTag() { return m_strModelTag; }

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

private:
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };

private:
	wstring		m_strModelTag;
	_float4		m_vObjectPos = {};	

private:
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Component() ;

public:
	static CPlateform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END