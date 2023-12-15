#pragma once
#include "Component.h"

BEGIN(Engine)

class CNavigation;

class ENGINE_DLL CTransform final : public CComponent
{
public:
	enum STATE {STATE_RIGHT,STATE_UP,STATE_LOOK,STATE_POS,STATE_END};
private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CTransform() = default;

public:
	HRESULT	Initialize_ProtoType(_float fSpeedPerSec, _float fRotationPerSec);

public:
	void	Set_State(STATE eState,const _float4& fVector) {
		memcpy(&m_matWorldMatrix.m[eState], &fVector, sizeof(_float3));
	}

	void	Set_State(STATE eState, _fvector fVector) {
		m_matWorldMatrix.m[eState][0] = XMVectorGetX(fVector);
		m_matWorldMatrix.m[eState][1] = XMVectorGetY(fVector);
		m_matWorldMatrix.m[eState][2] = XMVectorGetZ(fVector);
		m_matWorldMatrix.m[eState][3] = XMVectorGetW(fVector);
	}

	_vector	Get_State(STATE eState) {
		return XMVectorSet(m_matWorldMatrix.m[eState][0], m_matWorldMatrix.m[eState][1],
			m_matWorldMatrix.m[eState][2], m_matWorldMatrix.m[eState][3]);
	}

	void	Set_Scaling(_float fX, _float fY, _float fZ);

	_float3	Get_Scaled() {
		return _float3(XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_matWorldMatrix).r[STATE::STATE_RIGHT])),
			XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_matWorldMatrix).r[STATE::STATE_UP])),
			XMVectorGetX(XMVector3Length(XMLoadFloat4x4(&m_matWorldMatrix).r[STATE::STATE_LOOK])));
	}

	void	Set_WorldMatrix(_float4x4 matMat) { m_matWorldMatrix = matMat; }

	_float4x4	Get_WorldMatrix_Float4x4() { return m_matWorldMatrix; }
	_matrix	Get_WorldMatrix_Matrix() { return XMLoadFloat4x4(&m_matWorldMatrix); }
	_float4x4	Get_WorldMatrix_Inverse_Float4x4() { 
		_float4x4 matWorld;
		XMStoreFloat4x4(&matWorld,XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_matWorldMatrix)));
		return matWorld; }
	_matrix	Get_WorldMatrix_Inverse_Matrix() { 
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_matWorldMatrix)); }

public: /* 카메라에 사용*/
	void	Go_Straight(_float fTimeDelta, CNavigation* pNavigation = nullptr);
	void	Go_Left(_float fTimeDelta);
	void	Go_Right(_float fTimeDelta);
	void	Go_BackWard(_float fTimeDelta);
	void	Turn(_fvector vAxis, _float fTimeDelta);
	void	Rotation(_fvector vAxis, _float fRadian);
	void	Rotation_Total(_float fX, _float fY, _float fZ);
	void	Towards_Target(_fvector	fTargetPos, _float fTimeDelta, _float fSpare);
	void	LookAt(_fvector	fTargetPos);
	void	LookAt_OnLand(_fvector	fTargetPos);

public:
	HRESULT	Bind_ShaderResources(class CShader* pShader, const _char* pMatrixName);

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

private:
	_float		m_fSpeedPerSec = { 0.f };
	_float		m_fRotationPerSec = { 0.f };

	_float4x4	m_matWorldMatrix = {};

public:
	static	CTransform* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _float fSpeedPerSec, _float fRotationPerSec);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;

};

END