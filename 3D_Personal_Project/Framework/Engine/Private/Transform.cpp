#include "..\Public\Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "Navigation.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

HRESULT CTransform::Initialize_ProtoType(_float fSpeedPerSec, _float fRotationPerSec)
{
	m_fSpeedPerSec = fSpeedPerSec;
	m_fRotationPerSec = fRotationPerSec;

	XMStoreFloat4x4(&m_matWorldMatrix, XMMatrixIdentity());

	m_bUseJson = true;

	return S_OK;
}

void CTransform::Set_Scaling(_float fX, _float fY, _float fZ)
{
	Set_State(STATE::STATE_RIGHT, XMVector3Normalize(Get_State(STATE::STATE_RIGHT)) * fX);
	Set_State(STATE::STATE_UP, XMVector3Normalize(Get_State(STATE::STATE_UP)) * fY);
	Set_State(STATE::STATE_LOOK, XMVector3Normalize(Get_State(STATE::STATE_LOOK)) * fZ);

	if (isnan(m_matWorldMatrix.m[0][0]))
	{
		int a = 0;
	}	
}

void CTransform::Go_Straight(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector	vPos = Get_State(STATE::STATE_POS);
	_vector	vLook = Get_State(STATE::STATE_LOOK);

	vPos += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	_float3 vLine = {};

	if (pNavigation != nullptr)
	{
		if (!pNavigation->IsMove(vPos, &vLine))
		{
			return;
		}
	}

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

void CTransform::Go_BackWard(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector	vPos = Get_State(STATE::STATE_POS);
	_vector	vLook = Get_State(STATE::STATE_LOOK);

	vPos -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	/*if (pNavigation != nullptr)
	{
		if (!pNavigation->IsMove(vPos))
		{
			return;
		}
	}*/

	Set_State(STATE::STATE_POS, vPos);
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	if (XMVector3NearEqual(XMVectorZero(), vAxis, XMVectorSet(0.f, 0.f, 0.f, 0.f)))
		return;

	_matrix	matRotation = XMMatrixRotationAxis(vAxis, /*m_fRotationPerSec* */  fTimeDelta);

	Set_State(STATE::STATE_RIGHT,XMVector3TransformNormal(Get_State(STATE::STATE_RIGHT), matRotation));
	Set_State(STATE::STATE_UP,XMVector3TransformNormal(Get_State(STATE::STATE_UP), matRotation));
	Set_State(STATE::STATE_LOOK,XMVector3TransformNormal(Get_State(STATE::STATE_LOOK), matRotation));

	// 회전하는데 크기는 영향을 미치지 않음

	if (isnan(m_matWorldMatrix.m[0][0]))
	{
		int a = 0;
	}
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

	if (isnan(m_matWorldMatrix.m[0][0]))
	{
		int a = 0;
	}
}

void CTransform::Rotation_Total(_float fX, _float fY, _float fZ)
{

	_float3 fScale = Get_Scaled();

	_vector	vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * fScale.x;
	_vector	vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * fScale.y;
	_vector	vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * fScale.z;

	_matrix matRotT,matRotX, matRotY, matRotZ;

	matRotX = XMMatrixRotationX(fX);
	matRotY = XMMatrixRotationY(fY);
	matRotZ = XMMatrixRotationZ(fZ);

	matRotT = matRotX * matRotY * matRotZ;

	Set_State(STATE::STATE_RIGHT, XMVector3TransformNormal(vRight, matRotT));
	Set_State(STATE::STATE_UP, XMVector3TransformNormal(vUp, matRotT));
	Set_State(STATE::STATE_LOOK, XMVector3TransformNormal(vLook, matRotT));

	if (isnan(m_matWorldMatrix.m[0][0]))
	{
		int a = 0;
	}
}

void CTransform::Towards_Target(_fvector fTargetPos, _float fTimeDelta, _float fSpare)
{

	_vector vPos = Get_State(STATE::STATE_POS);
	_vector vDir = fTargetPos - vPos;

	_float fDistance = XMVectorGetX(XMVector3Length(vDir));

	if(fDistance>= fSpare)
		vPos += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE::STATE_POS, vPos);

	if (isnan(m_matWorldMatrix.m[0][0]))
	{
		int a = 0;
	}
}

void CTransform::LookAt(_fvector fTargetPos)
{
	_float3 fScale = Get_Scaled();

	_vector	vPos = Get_State(STATE::STATE_POS);

	_vector vLook = XMVector3Normalize(fTargetPos - vPos) * fScale.z;
	_vector	vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * fScale.x;
	_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * fScale.y;

	Set_State(STATE::STATE_RIGHT, vRight);
	Set_State(STATE::STATE_UP, vUp);
	Set_State(STATE::STATE_LOOK, vLook);

	if (isnan(m_matWorldMatrix.m[0][0]))
	{
		int a = 0;
	}
}

void CTransform::LookAt_OnLand(_fvector fTargetPos)
{

	_float3 fScale = Get_Scaled();

	_vector	vPos = Get_State(STATE::STATE_POS);

	_vector	vLook = fTargetPos - vPos;

	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * fScale.x;

	vLook = XMVector3Normalize(XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f))) * fScale.z;

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * fScale.y;
	
	Set_State(STATE::STATE_RIGHT, vRight);
	Set_State(STATE::STATE_UP, vUp);
	Set_State(STATE::STATE_LOOK, vLook);

	if (isnan(m_matWorldMatrix.m[0][0]))
	{
		int a = 0;
	}
}

void CTransform::Translate(_fvector vTranslation, CNavigation* pNavigation, _float fTimeDelta)
{
	if (pNavigation == nullptr)
		return;

	_vector vPosition = Get_State(STATE::STATE_POS);
	_vector vWorldPos = vPosition + vTranslation;
	_vector vSlidePos;

	_float3 vLine = {};
	_bool	bWall = false;

	if (!pNavigation->IsMove(vWorldPos, &vLine))
	{
		bWall = true;

		_vector vLook = Get_State(CTransform::STATE::STATE_LOOK);

		vSlidePos = Sliding(vLook,vLine, fTimeDelta);

		vSlidePos.m128_f32[1] = 0.f;

		vPosition += vSlidePos;

		if (!pNavigation->IsMove(vPosition, &vLine))
			return;
	
		vWorldPos = vPosition;
	}
	
	_vector vTemp;
	vTemp = bWall ? vSlidePos : vTranslation;

	_float3 fvPos = {};

	XMStoreFloat3(&fvPos, vWorldPos);

	_float fHeight = pNavigation->Get_Cell_Height(fvPos);

	_float fY = vWorldPos.m128_f32[1];

	if (fHeight >= fY)
	{
		m_bGround = true;

		vTemp.m128_f32[1] = 0.f;

		memcpy(&m_matWorldMatrix.m[3][1], &fHeight, sizeof(_float));

		XMStoreFloat4x4(&m_matWorldMatrix, XMLoadFloat4x4(&m_matWorldMatrix) *= XMMatrixTranslationFromVector(vTemp));
	}
	else {
		m_bGround = false;
		XMStoreFloat4x4(&m_matWorldMatrix, XMLoadFloat4x4(&m_matWorldMatrix) *= XMMatrixTranslationFromVector(vTemp));
	}

	if (isnan(m_matWorldMatrix.m[0][0]))
	{
		int a = 0;
	}
}

void CTransform::LookAt_Dir(_fvector vDir, _float fTimeDelta, _bool bGround)
{
	_vector vLook = XMVector3Normalize(vDir);
	_vector vPos = Get_State(CTransform::STATE::STATE_POS);

	if(bGround)
		vLook.m128_f32[1] = 0.f;

	LookAt(vPos + vLook);

}

_bool CTransform::Turn_Dir_Yaxis(_fvector vDir, _float fTimeDelta, _bool bGround)
{
	_vector vLook = XMVector3Normalize(vDir);
	_vector vPos = Get_State(CTransform::STATE::STATE_POS);
	
	if(bGround)
		vLook.m128_f32[1] = 0.f;

	return Turn_Target_Yaxis(vPos + vLook, fTimeDelta);
}

_vector CTransform::Get_Dir_Angle(_fvector vDir, _fvector vAxis, _float fAngle)
{
	_matrix	matRot = XMMatrixRotationAxis(vAxis, fAngle);
	return XMVector3TransformNormal(vDir, matRot);
}

_bool CTransform::Turn_Target_Yaxis(_fvector vTargetPos, _float fTimeDelta)
{
	_vector Dir = XMVector3Normalize(vTargetPos -
		Get_State(CTransform::STATE::STATE_POS));

	_vector vLook = Get_State(CTransform::STATE::STATE_LOOK);

	_float fAngle = acos(XMVectorGetX(XMVector3Dot(Dir, XMVector3Normalize(vLook))));
	
	if (isnan(fAngle))
		return false;

	_vector vCross = XMVector3Cross(Dir, vLook);

	if (XMVectorGetY((vCross)) > 0.f)
		fAngle *= -1.f;

	if (XMVector3NearEqual(Dir,
		vLook,
		XMVectorSet(0.1f, 1000.f, 0.1f, 0.f)))
		return true;
	
	Turn(Get_State(CTransform::STATE::STATE_UP), fTimeDelta * fAngle);

	if (isnan(m_matWorldMatrix.m[0][0]))
	{
		int a = 0;
	}

	return false;
}

_vector CTransform::Sliding(_fvector vDir,_float3 vLine, _float fTimeDelta)
{
	_vector vSlidePos;
	_vector vLineDir = XMLoadFloat3(&vLine);
	//_vector vLook = Get_State(CTransform::STATE::STATE_LOOK);
	_float fDot = XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir), XMVector3Normalize(vLineDir)));

	fTimeDelta *= fabsf(fDot);

	if (fDot > 0.f)
	{
		vSlidePos = XMVector3Normalize(vLineDir) * fTimeDelta * 2.f;
	}
	else {
		vSlidePos = -XMVector3Normalize(vLineDir) * fTimeDelta * 2.f;
	}

	return vSlidePos;
}

HRESULT CTransform::Bind_ShaderResources(CShader* pShader, const _char* pMatrixName)
{
	if (FAILED(pShader->Bind_Matrix(pMatrixName, &m_matWorldMatrix)))
		return E_FAIL;

	return S_OK;
}

void CTransform::Write_Json(json& Out_Json)
{
	Out_Json.clear();

	Out_Json.emplace("Transform", m_matWorldMatrix.m);
}

void CTransform::Load_FromJson(const json& In_Json)
{
	if (In_Json.find("Transform") == In_Json.end())
		return;

	for (_uint i = 0; i < 4; i++)
	{
		for (_uint j = 0; j < 4; j++)
		{
			m_matWorldMatrix.m[i][j] = In_Json["Transform"][i][j];
		}
	}
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
