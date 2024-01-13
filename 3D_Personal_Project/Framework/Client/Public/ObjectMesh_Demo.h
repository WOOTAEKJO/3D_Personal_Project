#pragma once
#include "Demo.h"

BEGIN(Engine)

class CModel;
class CModel_Instancing;

END

BEGIN(Client)

class CObjectMesh_Demo final : public CDemo
{
public:
	enum MODEL_TYPE {TYPE_NORMAL, TYPE_INSTANCING,TYPE_END};

	typedef struct tagObjectMeshDemoValue : public DEMO_DESC
	{
		vector<_float4x4> vecVertexMat;

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

	void	Set_ModelType(MODEL_TYPE eType) {m_eModelType = eType; }
	MODEL_TYPE	Get_ModelType() { return m_eModelType; }

	void	Rotation(_float fX, _float fY, _float fZ);
	void	Set_Scale(_float fX, _float fY, _float fZ);

	_float4x4	Get_WorldMat();

public:
	_bool	Get_Picked(_float4* vOutPos);
	_bool	Get_Picked_Dist(_float4* vOutPos,_float* fDist);

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

private:
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };
	CModel_Instancing* m_pModelInstancingCom = { nullptr };

private:
	MODEL_TYPE	m_eModelType = { MODEL_TYPE::TYPE_END };

private:
	vector<_float4x4>	m_vecVertexMat;

private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;

public:
	static CObjectMesh_Demo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END