#pragma once
#include "Demo.h"

BEGIN(Engine)

class CModel;

END

BEGIN(Client)

class CSubObject_Demo final : public CDemo
{
public:
	typedef struct tagSubObject_Demo_Desc
	{
		CBone* pSocketBone = nullptr;
		CTransform* pParentsTransform = nullptr;

		wstring		strModelTag;
		_float4		vPos;

	}SUBOBJECTDEMO_DESC;
private:
	CSubObject_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSubObject_Demo(const CSubObject_Demo& rhs);
	virtual	~CSubObject_Demo() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Set_Control_Variable(void* pArg) override;

public:
	wstring	Get_ModelTag() { return m_strModelTag; }

public:
	_bool	Get_Picked();

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

private:
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };

private:
	CTransform* m_pParentsTransform = { nullptr };
	CBone*		m_pSocketBone = { nullptr };
	_float4x4	m_matWorldMat;


	wstring		m_strModelTag;
	_float4		m_vObjectPos = {};	

private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;

public:
	static CSubObject_Demo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END