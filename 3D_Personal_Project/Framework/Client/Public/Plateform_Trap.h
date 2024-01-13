#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CModel;
class CShader;
class CCollider;

END

BEGIN(Client)

class CPlateform_Trap final : public CGameObject
{
public:
	typedef struct tagPlateform_TRAP_Desc
	{
		wstring		strModelTag;
		_float4		vPos;
		_float3		fAngle = _float3(0.f,0.f,0.f);
		_float		fAmplitude = {1.07f};

	}PLATEFORM_TRAP_DESC;
private:
	CPlateform_Trap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlateform_Trap(const CPlateform_Trap& rhs);
	virtual	~CPlateform_Trap() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

public:
	void		Set_ModelTag(const wstring& strModelTag) { m_strModelTag = strModelTag; }
	wstring		Get_ModelTag() { return m_strModelTag; }

private:
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };
	CCollider*	m_pColliderCom = { nullptr };

private:
	wstring		m_strModelTag;
	_float4		m_vObjectPos = {};	

private:
	_float3	m_fAngle = {0.f,0.f,0.f};
	_float  m_fAmplitude = { 1.07f };
	_float3	m_fAngularVelocity = {};

private:
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Component() ;

private:
	void	Pendulum_Movement(_float fTimeDelta);

public:
	static CPlateform_Trap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END