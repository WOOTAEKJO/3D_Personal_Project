#pragma once
#include "Demo.h"

BEGIN(Engine)

class CModel;

END

BEGIN(Client)

class CObjectMesh_Demo final : public CDemo
{
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

private:
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };

private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;

public:
	static CObjectMesh_Demo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END