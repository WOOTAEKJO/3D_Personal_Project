#include "..\Public\Mouse_Manager.h"
#include "GameInstance.h"

CMouse_Manager::CMouse_Manager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMouse_Manager::Initialize()
{
    return S_OK;
}

void CMouse_Manager::Update_Mouse(HWND hWnd)
{
	POINT	ptCursor;
	RECT	rcClient;
	GetCursorPos(&ptCursor);
	ScreenToClient(hWnd, &ptCursor);
	GetClientRect(hWnd, &rcClient);

	_matrix	matView = m_pGameInstance->Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE::VIEW);
	_matrix	matProj = m_pGameInstance->Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE::PROJ);

	_float3		vCursor(2.f * ptCursor.x / rcClient.right - 1.f, -2.f * ptCursor.y / rcClient.bottom + 1.f, 0.f);

	XMStoreFloat3(&m_pRay.vOrigin, XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 0.f, 1.f), XMMatrixInverse(nullptr, matView)));
	XMStoreFloat3(&m_pRay.vDir, XMVector3Normalize(XMVector3TransformNormal(XMVector3TransformCoord(XMLoadFloat3(&vCursor), XMMatrixInverse(nullptr, matProj)),
		XMMatrixInverse(nullptr, matView))));
	m_pRay.fLength = 0.f;
}

_bool CMouse_Manager::Intersect(_float3* pOut, _fvector vV1, _fvector vV2, _fvector vV3, _matrix matWorld)
{
	_float fDist = 0.f;

	_vector vPickPos;
	_vector	vRayPos = XMVector3TransformCoord(XMLoadFloat3(&m_pRay.vOrigin), XMMatrixInverse(nullptr, matWorld));
	_vector	vRayDir = XMVector3Normalize(XMVector3TransformNormal(XMLoadFloat3(&m_pRay.vDir), XMMatrixInverse(nullptr, matWorld)));

	if (DirectX::TriangleTests::Intersects(vRayPos, vRayDir, vV1, vV2, vV3, fDist))
	{
		vPickPos = vRayPos + XMVector3Normalize(vRayDir) * fDist;
		XMStoreFloat3(pOut, vPickPos);

		return true;
	}

    return false;
}

CMouse_Manager* CMouse_Manager::Create()
{
	CMouse_Manager* pInstance = new CMouse_Manager();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CMouse_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMouse_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}
