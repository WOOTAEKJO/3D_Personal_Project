#include "..\Public\VIBuffer_Instancing.h"

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Instancing::CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs)
	:CVIBuffer(rhs),m_iInstanceNum(rhs.m_iInstanceNum),m_iInstanceStride(rhs.m_iInstanceStride)
	,m_iIndexCountPerInstance(rhs.m_iIndexCountPerInstance),m_RandomNumber(rhs.m_RandomNumber),
	m_pSpeeds(rhs.m_pSpeeds),m_pLifeTime(rhs.m_pLifeTime), m_Instancing_Desc(rhs.m_Instancing_Desc)
{
}

HRESULT CVIBuffer_Instancing::Initialize_ProtoType()
{
	m_RandomNumber = mt19937_64(m_RandomDevice());

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Initialize(void* pArg)
{
	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iInstanceStride * m_iInstanceNum;	// 동적배열의 크기
	m_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;					// 정적버퍼인지 동적버퍼인지 설정/ 지금은 정적
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 종류 설정
	m_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// 동적만 해당
	m_Buffer_Desc.MiscFlags = 0;								// 동적만 해당
	m_Buffer_Desc.StructureByteStride = m_iInstanceStride;		// 정점 하나의 크기를 집어 넣음

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));



	VTXINSTANCING* pVerpostex = new VTXINSTANCING[m_iVertexNum];		// 버텍스 버퍼 안에 들어 갈 값들을 설정해줌

	_vector vDir = XMVectorSet(1.f, 0.f, 0.f, 0.f);

	uniform_real_distribution<float>	RandomRange(0.1f, m_Instancing_Desc.fRange);
	uniform_real_distribution<float>	RandomAngle(0.f, XMConvertToRadians(360.f));
	uniform_real_distribution<float>	RandomSpeed(m_Instancing_Desc.fScale.x, m_Instancing_Desc.fScale.y);
	uniform_real_distribution<float>	RandomLifeTime(m_Instancing_Desc.fLifeTime.x, m_Instancing_Desc.fLifeTime.y);
	uniform_real_distribution<float>	RandomScale(m_Instancing_Desc.fScale.x, m_Instancing_Desc.fScale.y);

	for (_uint i = 0; i < m_iInstanceNum; i++)
	{
		m_pSpeeds[i] = RandomSpeed(m_RandomNumber);
		m_pLifeTime[i] = RandomLifeTime(m_RandomNumber);

		_float fScale = RandomScale(m_RandomNumber);

		vDir = XMVector3Normalize(vDir) * RandomRange(m_RandomNumber);

		_vector vRot = XMQuaternionRotationRollPitchYaw(RandomAngle(m_RandomNumber), RandomAngle(m_RandomNumber), RandomAngle(m_RandomNumber));

		_matrix matRot = XMMatrixRotationQuaternion(vRot);

		vDir = XMVector3TransformNormal(vDir, matRot);

		pVerpostex[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
		pVerpostex[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
		pVerpostex[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		XMStoreFloat4(&pVerpostex[i].vPos,XMLoadFloat3(&m_Instancing_Desc.vCenter) + vDir);
		pVerpostex[i].vPos.w = 1.f;
	}

	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pInstanceBuffer)))		// 내가 할당한 값들을 버퍼가 할당한 공간에 복사하여 생성
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// 내가 할당한 것은 버퍼가 이미 사용했기에 필요 없으니 해제

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Bind_Buffer()
{
	if (nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;

	ID3D11Buffer* pVerTexBuffers[] = {
		m_pVB,
		m_pInstanceBuffer
	};	// 버텍스 버퍼들 배열

	_uint			iStrides[] = {
		m_iVertexStride,
		m_iInstanceStride
	};	// 버텍스 버퍼들의 정점 하나의 크기 배열

	_uint			iOffsets[] = {
		0,
		0
	}; // 버텍스 버퍼들의 offset

	/* 버텍스 버퍼를 바인딩*/
	m_pContext->IASetVertexBuffers(0, m_iVertexBuffersNum, pVerTexBuffers, iStrides, iOffsets);

	/* 인덱스 버퍼를 바인딩*/
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexForMat, 0);

	/* 정점을 이용해서 어떤식으로 그릴 것인지를 설정*/
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	return S_OK;
}

void CVIBuffer_Instancing::Update(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	D3D11_MAPPED_SUBRESOURCE Subresource = {};

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0,&Subresource);

	VTXINSTANCING* Instancing = ((VTXINSTANCING*)Subresource.pData);

	for (_uint i = 0; i < m_iInstanceNum; i++)
	{
		_float fLifeTime = max( m_pLifeTime[i] - m_fTimeAcc,0.f);
		
		Instancing[i].vColor.w = fLifeTime;

		_vector vDir = XMLoadFloat4(&Instancing[i].vPos) - XMLoadFloat3(&m_Instancing_Desc.vCenter);

		XMStoreFloat4(&Instancing[i].vPos,XMLoadFloat4(&Instancing[i].vPos) + XMVector3Normalize(vDir) * m_pSpeeds[i]);
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);
}

HRESULT CVIBuffer_Instancing::Render()
{
	m_pContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_iInstanceNum, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_Instancing::Free()
{
	__super::Free();

	if (m_bClone == false)
	{
		Safe_Delete_Array(m_pSpeeds);
		Safe_Delete_Array(m_pLifeTime);
	}

	Safe_Release(m_pInstanceBuffer);
}
