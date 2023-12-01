#include "..\Public\Mouse_Manager.h"
#include "GameInstance.h"

CMouse_Manager::CMouse_Manager()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMouse_Manager::Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
	m_hWnd = hWnd;
	m_iWinSize[0] = iWinSizeX;
	m_iWinSize[1] = iWinSizeY;

    return S_OK;
}

void CMouse_Manager::Update_Mouse()
{
	POINT	ptCursor;
	RECT	rcClient;
	GetCursorPos(&ptCursor);
	ScreenToClient(m_hWnd, &ptCursor);
	GetClientRect(m_hWnd, &rcClient);

	_matrix	matView = m_pGameInstance->Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE::VIEW);
	_matrix	matProj = m_pGameInstance->Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE::PROJ);

	_float3		vCursor(2.f * ptCursor.x / rcClient.right - 1.f, -2.f * ptCursor.y / rcClient.bottom + 1.f, 0.f);

	XMStoreFloat3(&m_pRay.vOrigin, XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 0.f, 1.f), XMMatrixInverse(nullptr, matView)));
	XMStoreFloat3(&m_pRay.vDir, XMVector3Normalize(XMVector3TransformNormal(XMVector3TransformCoord(XMLoadFloat3(&vCursor), XMMatrixInverse(nullptr, matProj)),
		XMMatrixInverse(nullptr, matView))));

	XMStoreFloat3(&vCursor, XMVector3TransformCoord(XMVector3TransformCoord(XMLoadFloat3(&vCursor), XMMatrixInverse(nullptr, matProj)),
		XMMatrixInverse(nullptr, matView)));
	
	m_vWorldMouse = _float4(vCursor.x, vCursor.y, vCursor.z, 1.f);
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

void CMouse_Manager::Free_Mouse(_float fTimeDelta, _float fMouseSensitivity, CTransform* pTransCom)
{
	Mouse_Key();
	if (!m_bFix) {
		Mouse_Move(fTimeDelta, fMouseSensitivity, pTransCom);
		Mouse_Fix();
	}

	/*if (m_pGameInstance->Mouse_Pressing(DIM_RB))
	{
		Mouse_Move(fTimeDelta, fMouseSensitivity, pTransCom);
	}*/
}

void CMouse_Manager::Mouse_Fix()
{
	POINT	pt{ (_long)m_iWinSize[0] >> 1, (_long)m_iWinSize[1] >> 1};

	ClientToScreen(m_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CMouse_Manager::Mouse_Move(_float fTimeDelta, _float fMouseSensitivity, CTransform* pTransCom)
{
	_long MouseMove = 0;

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		pTransCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fMouseSensitivity * MouseMove * fTimeDelta);
	}

	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
	{
		pTransCom->Turn(pTransCom->Get_State(CTransform::STATE::STATE_RIGHT),
			fMouseSensitivity * MouseMove * fTimeDelta);
	}
}

void CMouse_Manager::Mouse_Key()
{
	if (m_pGameInstance->Key_Down(DIK_TAB))
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;

	}
	else
		m_bCheck = false;

	if (m_bFix == false)
		return;
}

CMouse_Manager* CMouse_Manager::Create(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
	CMouse_Manager* pInstance = new CMouse_Manager();

	if (FAILED(pInstance->Initialize(hWnd, iWinSizeX, iWinSizeY))) {
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
