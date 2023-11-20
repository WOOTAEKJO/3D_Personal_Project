#include "..\Public\Transform.h"
#include "GameObject.h"
#include "Shader.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

HRESULT CTransform::Initialize_ProtoType(_float fSpeedPerSec, _float fRotationPerSec)
{
	m_fSpeedPerSec = fSpeedPerSec;
	m_fRotationPerSec = fRotationPerSec;

	XMStoreFloat4x4(&m_matWorldMatrix, XMMatrixIdentity());

	return S_OK;
}

void CTransform::Set_Scaling(_float fX, _float fY, _float fZ)
{
	Set_State(STATE::STATE_RIGHT, XMVector3Normalize(Get_State(STATE::STATE_RIGHT)) * fX);
	Set_State(STATE::STATE_UP, XMVector3Normalize(Get_State(STATE::STATE_UP)) * fY);
	Set_State(STATE::STATE_LOOK, XMVector3Normalize(Get_State(STATE::STATE_LOOK)) * fZ);
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_vector	vPos = Get_State(STATE::STATE_POS);
	_vector	vLook = Get_State(STATE::STATE_LOOK);

	vPos += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::STATE_POS, vPos);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_vector	vPos = Get_State(STATE::STATE_POS);
	_vector	vRight = Get_State(STATE::STATE_RIGHT);

	vPos -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::STATE_POS, vPos);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_vector	vPos = Get_State(STATE::STATE_POS);
	_vector	vRight = Get_State(STATE::STATE_RIGHT);

	vPos += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::STATE_POS, vPos);
}

void CTransform::Go_BackWard(_float fTimeDelta)
{
	_vector	vPos = Get_State(STATE::STATE_POS);
	_vector	vLook = Get_State(STATE::STATE_LOOK);

	vPos -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::STATE_POS, vPos);
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_matrix	matRotation = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

	Set_State(STATE::STATE_RIGHT,XMVector3TransformNormal(Get_State(STATE::STATE_RIGHT), matRotation));
	Set_State(STATE::STATE_UP,XMVector3TransformNormal(Get_State(STATE::STATE_UP), matRotation));
	Set_State(STATE::STATE_LOOK,XMVector3TransformNormal(Get_State(STATE::STATE_LOOK), matRotation));

	// 회전하는데 크기는 영향을 미치지 않음
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_float3 fScale = Get_Scaled();

	_vector	vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * fScale.x;
	_vector	vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * fScale.y;
	_vector	vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * fScale.z;

	// 크기 값이 1이기 때문에 원래 크기로 되돌려 줘야한다.

	_matrix	matRotation = XMMatrixRotationAxis(vAxis, fRadian);

	Set_State(STATE::STATE_RIGHT, XMVector3TransformNormal(vRight, matRotation));
	Set_State(STATE::STATE_UP, XMVector3TransformNormal(vUp, matRotation));
	Set_State(STATE::STATE_LOOK, XMVector3TransformNormal(vLook, matRotation));
}

void CTransform::Towards_Target(_fvector fTargetPos, _float fTimeDelta, _float fSpare)
{
	_vector vPos = Get_State(STATE::STATE_POS);
	_vector vDir = fTargetPos - vPos;

	_float fDistance = XMVectorGetX(XMVector3Length(vDir));

	if(fDistance>= fSpare)
		vPos += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::STATE_POS, vPos);
}

void CTransform::LookAt(_fvector fTargetPos)
{
	_float3 fScale = Get_Scaled();

	_vector	vPos = Get_State(STATE::STATE_POS);

	_vector vLook = XMVector3Normalize(fTargetPos - vPos) * fScale.z;
	_vector	vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 1.f), vLook)) * fScale.x;
	_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * fScale.y;

	Set_State(STATE::STATE_RIGHT, vRight);
	Set_State(STATE::STATE_UP, vUp);
	Set_State(STATE::STATE_LOOK, vLook);
}

void CTransform::LookAt_OnLand(_fvector fTargetPos)
{
	_float3 fScale = Get_Scaled();

	_vector	vPos = Get_State(STATE::STATE_POS);

	_vector	vLook = fTargetPos - vPos;
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * fScale.y;
	_vector vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook)) * fScale.x;
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp)) * fScale.z;

	Set_State(STATE::STATE_RIGHT, vRight);
	Set_State(STATE::STATE_UP, vUp);
	Set_State(STATE::STATE_LOOK, vLook);
}

HRESULT CTransform::Bind_ShaderResources(CShader* pShader, const _char* pMatrixName)
{
	if (FAILED(pShader->Bind_Matrix(pMatrixName, &m_matWorldMatrix)))
		return E_FAIL;

	return S_OK;
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _float fSpeedPerSec, _float fRotationPerSec)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(fSpeedPerSec, fRotationPerSec))) {
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	return nullptr;
}

void CTransform::Free()
{
	__super::Free();
}
