#pragma once
#include "Demo.h"

BEGIN(Engine)

class CModel;

END

BEGIN(Client)

class CObjectMesh_Demo final : public CDemo
{
public:
	typedef struct tagObjectMeshDemoValue
	{
		wstring		strModelTag;
		_float4		vPos;

	}OBDEMOVALUE;
private:
	CObjectMesh_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObjectMesh_Demo(const CObjectMesh_Demo& rhs);
	virtual	~CObjectMesh_Demo() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Set_Control_Variable(void* pArg) override;

public:
	void	Set_TransformState(CTransform::STATE eType, _float4 vVector);
	_float4	Get_TransformState(CTransform::STATE eType);

	void	Rotation(_float fX, _float fY, _float fZ);
	void	Set_Scale(_float fX, _float fY, _float fZ);

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
	wstring		m_strModelTag;
	_float4		m_vObjectPos = {};	

private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;

public:
	static CObjectMesh_Demo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END