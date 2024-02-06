#include "..\Public\VIBuffer_Trail.h"

#include "GameObject.h"
#include "Transform.h"

CVIBuffer_Trail::CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail& rhs)
	:CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Trail::Initialize_ProtoType()
{
	

	return S_OK;
}

HRESULT CVIBuffer_Trail::Initialize(void* pArg)
{

	if (pArg == nullptr)
		return E_FAIL;

	m_Trail_Desc = *(VIBUFFER_TRAIL_DESC*)pArg;
	ZeroMemory(&m_iCatMullRomIndex, sizeof(_uint) * 4);

	m_iVertexBuffersNum = 1;
	m_iVertexNum = 2 * m_Trail_Desc.iMaxCnt + 2;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iVtxCount = 0;

	VTXPOSTEX* pVerpostex = new VTXPOSTEX[m_iVertexNum];		

	for (_uint i = 0; i < m_iVertexNum; i += 2)
	{
		pVerpostex[i].fPosition = m_Trail_Desc.vPos_0;
		pVerpostex[i + 1].fPosition = m_Trail_Desc.vPos_1;

		pVerpostex[i].fTexCoord = _float2(1.f, 0.f);
		pVerpostex[i + 1].fTexCoord = _float2(1.f, 1.f);
	}

	pVerpostex[0].fTexCoord = _float2(0.f, 0.f);
	pVerpostex[1].fTexCoord = _float2(0.f, 1.f);

	m_iPrimitiveNum = m_Trail_Desc.iMaxCnt * 2;
	m_iIndexNum = m_iPrimitiveNum * 3;
	m_iIndexStride = sizeof(_ushort3);
	m_eIndexForMat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	_ushort3* pIndex = new _ushort3[m_iIndexNum];
	ZeroMemory(pIndex, sizeof(_ushort3) * m_iIndexNum);

	for (_uint i = 0; i < m_iPrimitiveNum; i += 2)
	{
		pIndex[i] = { _ushort(i),_ushort(i + 2) ,_ushort(i + 3) };
		pIndex[i + 1] = { _ushort(i),_ushort(i + 3) ,_ushort(i + 1) };
	}

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// 동적배열의 크기
	m_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;					// 정적버퍼인지 동적버퍼인지 설정/ 지금은 정적
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 종류 설정
	m_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							// 동적만 해당
	m_Buffer_Desc.MiscFlags = 0;								// 동적만 해당
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// 정점 하나의 크기를 집어 넣음

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));
	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pVB)))		// 내가 할당한 값들을 버퍼가 할당한 공간에 복사하여 생성
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// 내가 할당한 것은 버퍼가 이미 사용했기에 필요 없으니 해제

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iIndexStride * m_iPrimitiveNum;
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_Buffer_Desc.CPUAccessFlags = 0;
	m_Buffer_Desc.MiscFlags = 0;
	m_Buffer_Desc.StructureByteStride = 0;

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));
	m_SubResource_Data.pSysMem = pIndex;

	if (FAILED(Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndex);

	return S_OK;
}

void CVIBuffer_Trail::Update(_fmatrix matOwner)
{
	_matrix matTmp = matOwner;
	matTmp.r[0] = XMVector3Normalize(matTmp.r[0]);
	matTmp.r[1] = XMVector3Normalize(matTmp.r[1]);
	matTmp.r[2] = XMVector3Normalize(matTmp.r[2]);

	D3D11_MAPPED_SUBRESOURCE SubResource;
	if (FAILED(m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource)))
		return;

	if (m_iVtxCount >= m_iVertexNum)
	{
		_uint iRemoveCount = m_iVtxCount / 4;
		//_uint iRemoveCount = m_Trail_Desc.iLerpPointNum * 2;
		m_iVtxCount -= iRemoveCount;

		for (_uint i = 0; i < m_iVtxCount; i += 2)
		{
			((VTXPOSTEX*)SubResource.pData)[i].fPosition = ((VTXPOSTEX*)SubResource.pData)[iRemoveCount + i].fPosition;
			((VTXPOSTEX*)SubResource.pData)[i + 1].fPosition = ((VTXPOSTEX*)SubResource.pData)[iRemoveCount + i + 1].fPosition;
		}
	} // 정점 중첩 갯수가 정점 갯수를 초과한다면 정점의 위치를 조정

	_vector vPos[2];
	vPos[0] = XMVector3TransformCoord(XMVectorSetW(XMLoadFloat3(&m_Trail_Desc.vPos_0), 1.f), matTmp);
	vPos[1] = XMVector3TransformCoord(XMVectorSetW(XMLoadFloat3(&m_Trail_Desc.vPos_1), 1.f), matTmp);
	// 정점들 오너의 월드 위치 적용

	if (m_iVtxCount == 0)
	{
		for (_uint i = 0; i < m_iVertexNum; i += 2)
		{
			XMStoreFloat3(&((VTXPOSTEX*)SubResource.pData)[i].fPosition, vPos[0]);
			XMStoreFloat3(&((VTXPOSTEX*)SubResource.pData)[i+1].fPosition, vPos[1]);
		}
	} // 정점 중첩 갯수가 0이면 처음
	else
	{
		XMStoreFloat3(&((VTXPOSTEX*)SubResource.pData)[m_iVtxCount].fPosition, vPos[0]);
		XMStoreFloat3(&((VTXPOSTEX*)SubResource.pData)[m_iVtxCount + 1].fPosition, vPos[1]);
	}// 처음이 아니면 정점을 이어간다.

	m_iVtxCount += 2;
	// 정점 중첩

	_uint iEndIndex = m_iVtxCount + m_Trail_Desc.iLerpPointNum * 2;
	if (iEndIndex < m_iVertexNum)
	{
		m_iCatMullRomIndex[2] = iEndIndex - 2;
		m_iCatMullRomIndex[3] = iEndIndex;

		XMStoreFloat3(&((VTXPOSTEX*)SubResource.pData)[iEndIndex - 2].fPosition,
			XMLoadFloat3(&((VTXPOSTEX*)SubResource.pData)[m_iVtxCount - 2].fPosition));
		XMStoreFloat3(&((VTXPOSTEX*)SubResource.pData)[iEndIndex - 1].fPosition,
			XMLoadFloat3(&((VTXPOSTEX*)SubResource.pData)[m_iVtxCount - 1].fPosition));

		XMStoreFloat3(&((VTXPOSTEX*)SubResource.pData)[iEndIndex].fPosition, vPos[0]);
		XMStoreFloat3(&((VTXPOSTEX*)SubResource.pData)[iEndIndex + 1].fPosition, vPos[1]);

		for (_uint i = 0; i < m_Trail_Desc.iLerpPointNum; ++i)
		{
			_uint iIndex(i * 2 + m_iVtxCount - 2);
			_float fWeight(_float(i + 1) / (m_Trail_Desc.iLerpPointNum + 1));

			_vector vPos0 = XMLoadFloat3(&((VTXPOSTEX*)SubResource.pData)[m_iCatMullRomIndex[0]].fPosition);
			_vector vPos1 = XMLoadFloat3(&((VTXPOSTEX*)SubResource.pData)[m_iCatMullRomIndex[1]].fPosition);
			_vector vPos2 = XMLoadFloat3(&((VTXPOSTEX*)SubResource.pData)[m_iCatMullRomIndex[2]].fPosition);
			_vector vPos3 = XMLoadFloat3(&((VTXPOSTEX*)SubResource.pData)[m_iCatMullRomIndex[3]].fPosition);

			_vector LerpPoint = XMVectorCatmullRom(vPos0, vPos1, vPos2, vPos3, fWeight);
			XMStoreFloat3(&((VTXPOSTEX*)SubResource.pData)[iIndex].fPosition, LerpPoint);

			vPos0 = XMLoadFloat3(&((VTXPOSTEX*)SubResource.pData)[m_iCatMullRomIndex[0]+1].fPosition);
			vPos1 = XMLoadFloat3(&((VTXPOSTEX*)SubResource.pData)[m_iCatMullRomIndex[1]+1].fPosition);
			vPos2 = XMLoadFloat3(&((VTXPOSTEX*)SubResource.pData)[m_iCatMullRomIndex[2]+1].fPosition);
			vPos3 = XMLoadFloat3(&((VTXPOSTEX*)SubResource.pData)[m_iCatMullRomIndex[3]+1].fPosition);

			LerpPoint = XMVectorCatmullRom(vPos0, vPos1, vPos2, vPos3, fWeight);
			XMStoreFloat3(&((VTXPOSTEX*)SubResource.pData)[iIndex + 1].fPosition, LerpPoint);
		}

		m_iVtxCount = iEndIndex + 2;

		m_iCatMullRomIndex[0] = m_iCatMullRomIndex[1];
		m_iCatMullRomIndex[1] = m_iCatMullRomIndex[2];
	}
	// 캣뮬롬 적용

	for (_uint i = 0; i < m_iVtxCount; i += 2)
	{
		((VTXPOSTEX*)SubResource.pData)[i].fTexCoord = _float2(_float(i) / _float(m_iVtxCount - 2.f), 1.f);
		((VTXPOSTEX*)SubResource.pData)[i+1].fTexCoord = _float2(_float(i) / _float(m_iVtxCount - 2.f), 0.f);
	}// UV 좌표 설정

	m_pContext->Unmap(m_pVB, 0);
}

void CVIBuffer_Trail::Reset(_fmatrix matOwner)
{
	_matrix matTmp = matOwner;
	matTmp.r[0] = XMVector3Normalize(matTmp.r[0]);
	matTmp.r[1] = XMVector3Normalize(matTmp.r[1]);
	matTmp.r[2] = XMVector3Normalize(matTmp.r[2]);

	D3D11_MAPPED_SUBRESOURCE SubResource;
	if (FAILED(m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource)))
		return;


	_vector vPos[2];
	vPos[0] = XMVector3TransformCoord(XMVectorSetW(XMLoadFloat3(&m_Trail_Desc.vPos_0), 1.f), matTmp);
	vPos[1] = XMVector3TransformCoord(XMVectorSetW(XMLoadFloat3(&m_Trail_Desc.vPos_1), 1.f), matTmp);

	for (_uint i = 0; i < m_iVertexNum; i += 2)
	{
		XMStoreFloat3(&((VTXPOSTEX*)SubResource.pData)[i].fPosition, vPos[0]);
		XMStoreFloat3(&((VTXPOSTEX*)SubResource.pData)[i + 1].fPosition, vPos[1]);
	}

	m_pContext->Unmap(m_pVB, 0);
}

CVIBuffer_Trail* CVIBuffer_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Trail::Clone(void* pArg)
{
	CVIBuffer_Trail* pInstance = new CVIBuffer_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	__super::Free();
}
