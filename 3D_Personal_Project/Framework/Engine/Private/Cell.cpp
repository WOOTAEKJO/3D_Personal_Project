#include "..\Public\Cell.h"
#include "VIBuffer_Cell.h"
#include "Shader.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
#ifdef _DEBUG
	m_pDevice = pDevice;
	m_pContext = pContext;
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

#endif
}

HRESULT CCell::Initialize(FLOAT3X3 pPoints)
{
	memcpy(&m_pPoints, &pPoints, sizeof(_float3)*3);

	_vector Line = XMLoadFloat3(&m_pPoints[POINT_B]) - XMLoadFloat3(&m_pPoints[POINT_A]);
	XMStoreFloat3(&m_vLineNormal[LINE_AB], XMVectorSet(XMVectorGetZ(Line) * -1.f, 0.f, XMVectorGetX(Line), 0.f));
	Line = XMLoadFloat3(&m_pPoints[POINT_C]) - XMLoadFloat3(&m_pPoints[POINT_B]);
	XMStoreFloat3(&m_vLineNormal[LINE_BC], XMVectorSet(XMVectorGetZ(Line) * -1.f, 0.f, XMVectorGetX(Line), 0.f));
	Line = XMLoadFloat3(&m_pPoints[POINT_A]) - XMLoadFloat3(&m_pPoints[POINT_C]);
	XMStoreFloat3(&m_vLineNormal[LINE_CA], XMVectorSet(XMVectorGetZ(Line) * -1.f, 0.f, XMVectorGetX(Line), 0.f));

#ifdef _DEBUG
	m_pBufferCom = CVIBuffer_Cell::Create(m_pDevice,m_pContext, m_pPoints);
	if (m_pBufferCom == nullptr)
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CCell::Render()
{
	m_pBufferCom->Bind_Buffer();

	m_pBufferCom->Render();

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
		if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_A]), XMLoadFloat3(&DestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_C]), XMLoadFloat3(&DestPoint)))
			return true;
	}

	if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_C]), XMLoadFloat3(&SourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_A]), XMLoadFloat3(&DestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_pPoints[POINT_B]), XMLoadFloat3(&DestPoint)))
			return true;
	}
}

_bool CCell::IsIn(_fvector vPosition, _fmatrix matWorld, _int* iNeighborIndex)
{
	for (_uint i = 0; i < LINE_END; i++)
	{
		_vector vStartPoint = XMVector3TransformCoord(XMLoadFloat3(&m_pPoints[i]), matWorld);
		_vector vNormal = XMVector3TransformNormal(XMLoadFloat3(&m_vLineNormal[i]), matWorld);

		_vector vDir = vPosition - vStartPoint;

		if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDir),
			XMVector3Normalize(vNormal))))
		{
			*iNeighborIndex = m_iNeighborIndex[i];
			return false;
		}
			
	}

	return true;
}

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FLOAT3X3 pPoints)
{
	CCell* pInstance = new CCell(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPoints))) {
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

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

#endif
}
