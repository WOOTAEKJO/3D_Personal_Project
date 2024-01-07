#include "..\Public\Cell.h"
#include "VIBuffer_Cell.h"
#include "VIBuffer_DCell.h"
#include "Shader.h"
#include "Navigation.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
#ifdef _DEBUG
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

#endif
}

HRESULT CCell::Initialize(FLOAT3X3 pPoints, _uint iIndex, CNavigation::NAVITYPE eType, CELLTYPE eCellType)
{
	memcpy(&m_pPoints, &pPoints, sizeof(_float3)*3);

	m_iIndex = iIndex;

	m_eNaviType = eType;

	m_eCell_Type = eCellType;

	_vector Line = XMLoadFloat3(&m_pPoints[POINT_B]) - XMLoadFloat3(&m_pPoints[POINT_A]);
	XMStoreFloat3(&m_vLine[LINE_AB], Line);
	XMStoreFloat3(&m_vLineNormal[LINE_AB], XMVectorSet(XMVectorGetZ(Line) * -1.f, 0.f, XMVectorGetX(Line), 0.f));
	Line = XMLoadFloat3(&m_pPoints[POINT_C]) - XMLoadFloat3(&m_pPoints[POINT_B]);
	XMStoreFloat3(&m_vLine[LINE_BC], Line);
	XMStoreFloat3(&m_vLineNormal[LINE_BC], XMVectorSet(XMVectorGetZ(Line) * -1.f, 0.f, XMVectorGetX(Line), 0.f));
	Line = XMLoadFloat3(&m_pPoints[POINT_A]) - XMLoadFloat3(&m_pPoints[POINT_C]);
	XMStoreFloat3(&m_vLine[LINE_CA], Line);
	XMStoreFloat3(&m_vLineNormal[LINE_CA], XMVectorSet(XMVectorGetZ(Line) * -1.f, 0.f, XMVectorGetX(Line), 0.f));

#ifdef _DEBUG

	if (m_eNaviType == CNavigation::NAVITYPE::TYPE_LOAD) {

		m_pBufferCom = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_pPoints);
		if (m_pBufferCom == nullptr)
			return E_FAIL;
	}
	else if (m_eNaviType == CNavigation::NAVITYPE::TYPE_DEMO)
	{
		m_pDBufferCom = CVIBuffer_DCell::Create(m_pDevice, m_pContext, m_pPoints);
		if (m_pDBufferCom == nullptr)
			return E_FAIL;
	}

#endif

	return S_OK;
}

HRESULT CCell::Render()
{

	if(m_eNaviType== CNavigation::NAVITYPE::TYPE_LOAD)
	{
		m_pBufferCom->Bind_Buffer();

		m_pBufferCom->Render();
	}
	else if (m_eNaviType == CNavigation::NAVITYPE::TYPE_DEMO)
	{
		m_pDBufferCom->Bind_Buffer();

		m_pDBufferCom->Render();
	}

	return S_OK;
}

_bool CCell::Compare_Points(_float3 SourPoint, _float3 DestPoint)
{
	if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_A]), XMLoadFloat3(&SourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_B]), XMLoadFloat3(&DestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_C]), XMLoadFloat3(&DestPoint)))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_B]), XMLoadFloat3(&SourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_C]), XMLoadFloat3(&DestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_A]), XMLoadFloat3(&DestPoint)))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_C]), XMLoadFloat3(&SourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_A]), XMLoadFloat3(&DestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_B]), XMLoadFloat3(&DestPoint)))
			return true;
	}

	return false;
}

_bool CCell::IsIn(_fvector vPosition, _fmatrix matWorld, _int* iNeighborIndex, _Out_ _float3* vLine)
{
	
	for (_uint i = 0; i < (_uint)LINE_END; i++)
	{
		_vector vStartPoint = XMVector3TransformCoord(XMLoadFloat3(&m_pPoints[i]), matWorld);
		_vector vNormal = XMVector3TransformNormal(XMLoadFloat3(&m_vLineNormal[i]), matWorld);

		_vector vDir = vPosition - vStartPoint;

		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir),
			XMVector3Normalize(vNormal))))
		{
			*iNeighborIndex = m_iNeighborIndex[i];
			XMStoreFloat3(vLine, XMVector3Normalize(XMLoadFloat3(&m_vLine[i])));
			return false;
		}
			
	}

	return true;
}

_float CCell::Get_Height(_float3 vPos)
{
	_vector vTmp = XMPlaneFromPoints(XMLoadFloat3(&m_pPoints[POINT_A]),
		XMLoadFloat3(&m_pPoints[POINT_B]), XMLoadFloat3(&m_pPoints[POINT_C]));

	_float4 vPlane = {};
	XMStoreFloat4(&vPlane, vTmp);

	_float fHeight = ((-vPlane.x * vPos.x) - (vPlane.z * vPos.z) - vPlane.w) / vPlane.y;

	return fHeight;
}

_bool CCell::Is_Height(_float3 vPos)
{
	_float fHeight = Get_Height(vPos);

	if (vPos.y > fHeight)
		return true;

	return false;
}

void CCell::Update_Buffer(FLOAT3X3 vPositions)
{
	if (m_pDBufferCom == nullptr)
		return;

	m_pPoints[0] = vPositions.vVertex0;
	m_pPoints[1] = vPositions.vVertex1;
	m_pPoints[2] = vPositions.vVertex2;

	m_pDBufferCom->Update_Buffer(vPositions);
}

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FLOAT3X3 pPoints, _uint iIndex,
	CNavigation::NAVITYPE eType, CELLTYPE eCellType)
{
	CCell* pInstance = new CCell(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pPoints, iIndex, eType, eCellType))) {
		MSG_BOX("Failed to Created : CCell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pDBufferCom);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

#endif
}
