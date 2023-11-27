#include "..\Public\PipeLine.h"

CPipeLine::CPipeLine()
{
}

HRESULT CPipeLine::Initialize()
{
	for (_uint i = 0; i < TRANSFORMSTATE::STATE_END; i++)
	{
		XMStoreFloat4x4(&m_matTransform[i], XMMatrixIdentity());
		XMStoreFloat4x4(&m_matTransformInverse[i], XMMatrixIdentity());
	}

	return S_OK;
}

void CPipeLine::Tick()
{
	for (_uint i = 0; i < TRANSFORMSTATE::STATE_END; i++)
	{
		XMStoreFloat4x4(&m_matTransformInverse[i], XMMatrixInverse(nullptr,  XMLoadFloat4x4(&m_matTransform[i])));
	}
	memcpy(&m_vCameraPos, &m_matTransformInverse[TRANSFORMSTATE::VIEW].m[3],sizeof(_float4));
}

void CPipeLine::Set_Transform(TRANSFORMSTATE eState, _float4x4 matMatrix)
{
	m_matTransform[eState] = matMatrix;
}

void CPipeLine::Set_Transform(TRANSFORMSTATE eState, _fmatrix matMatrix)
{
	XMStoreFloat4x4(&m_matTransform[eState], matMatrix);
}

_float4x4 CPipeLine::Get_Transform_Float4x4(TRANSFORMSTATE eState)
{
	return m_matTransform[eState];
}

_matrix CPipeLine::Get_Transform_Matrix(TRANSFORMSTATE eState)
{
	return XMLoadFloat4x4(&m_matTransform[eState]);
}

_float4x4 CPipeLine::Get_Transform_Float4x4_Inverse(TRANSFORMSTATE eState)
{
	return m_matTransformInverse[eState];
}

_matrix CPipeLine::Get_Transform_Matrix_Inverse(TRANSFORMSTATE eState)
{
	return XMLoadFloat4x4(&m_matTransformInverse[eState]);
}

_float4 CPipeLine::Get_Camera_Pos()
{
	return m_vCameraPos;
}

CPipeLine* CPipeLine::Create()
{
	CPipeLine* pInstance = new CPipeLine();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CPipeLine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPipeLine::Free()
{
	__super::Free();
}
