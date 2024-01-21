#include "..\Public\VIBuffer_Instancing.h"

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Instancing::CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs)
	:CVIBuffer(rhs),m_iInstanceNum(rhs.m_iInstanceNum),m_iInstanceStride(rhs.m_iInstanceStride)
	,m_iIndexCountPerInstance(rhs.m_iIndexCountPerInstance),m_RandomNumber(rhs.m_RandomNumber),
	m_pSpeeds(rhs.m_pSpeeds),m_pLifeTime(rhs.m_pLifeTime), m_Instancing_Desc(rhs.m_Instancing_Desc),
	m_eInstanceType(rhs.m_eInstanceType)
{
}

HRESULT CVIBuffer_Instancing::Initialize_ProtoType()
{
	m_RandomNumber = mt19937_64(m_RandomDevice());

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Initialize(void* pArg) 
{

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
	};	// ���ؽ� ���۵� �迭

	_uint			iStrides[] = {
		m_iVertexStride,
		m_iInstanceStride
	};	// ���ؽ� ���۵��� ���� �ϳ��� ũ�� �迭

	_uint			iOffsets[] = {
		0,
		0
	}; // ���ؽ� ���۵��� offset

	/* ���ؽ� ���۸� ���ε�*/
	m_pContext->IASetVertexBuffers(0, m_iVertexBuffersNum, pVerTexBuffers, iStrides, iOffsets);

	/* �ε��� ���۸� ���ε�*/
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexForMat, 0);

	/* ������ �̿��ؼ� ������� �׸� �������� ����*/
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	return S_OK;
}

void CVIBuffer_Instancing::Update(_float fTimeDelta)
{
	

	D3D11_MAPPED_SUBRESOURCE Subresource = {};

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0,&Subresource);

	VTXINSTANCING* Instancing = ((VTXINSTANCING*)Subresource.pData);

	_vector vDir;
	_bool bRuntimeDir = { false };
	if (!XMVector3Equal(XMLoadFloat3(&m_Instancing_Desc.vRunDir), XMVectorZero()))
	{
		bRuntimeDir = true;
		vDir = XMLoadFloat3(&m_Instancing_Desc.vRunDir);
	} // ������ ������ 0�� �ƴϸ� ������ ������ ��ġ ���� ����
		
	for (_uint i = 0; i < m_iInstanceNum; i++)
	{
		if (m_pLifeTime[i] != 0.f)
		{
			m_pTimeAcc[i] += fTimeDelta;

			_float fLifeTime = max(m_pLifeTime[i] - m_pTimeAcc[i], 0.f);

			Instancing[i].vColor.w = fLifeTime;
			// ������Ÿ���� ���� ���
		}
		else
		{

		}

		Instancing[i].vRight = _float4(m_pScale[i], 0.f, 0.f, 0.f);
		Instancing[i].vUp = _float4(0.f, m_pScale[i], 0.f, 0.f);
		Instancing[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		// ũ��

		if (m_pRunRotation[i] != 0.f) {
			//_matrix matRot = XMMatrixRotationZ(m_pRunRotation[i] * fTimeDelta);

			_vector vRot = XMQuaternionRotationRollPitchYaw(m_pRunRotation[i] * fTimeDelta,
				m_pRunRotation[i] * fTimeDelta, m_pRunRotation[i] * fTimeDelta);

			_matrix matRot = XMMatrixRotationQuaternion(vRot);

			XMStoreFloat4(&Instancing[i].vRight,XMVector3TransformNormal(XMLoadFloat4(&Instancing[i].vRight), matRot));
			XMStoreFloat4(&Instancing[i].vUp, XMVector3TransformNormal(XMLoadFloat4(&Instancing[i].vUp), matRot));
			XMStoreFloat4(&Instancing[i].vLook, XMVector3TransformNormal(XMLoadFloat4(&Instancing[i].vLook), matRot));
		} // ȸ��
		
		vDir = bRuntimeDir == false ?
			CenterToPos(Instancing[i].vPos) : vDir;

		XMStoreFloat4(&Instancing[i].vPos,XMLoadFloat4(&Instancing[i].vPos) + XMVector3Normalize(vDir) * m_pSpeeds[i] * fTimeDelta);
		// ��ġ
	}

	m_pContext->Unmap(m_pInstanceBuffer, 0);
}

HRESULT CVIBuffer_Instancing::Render()
{
	m_pContext->DrawIndexedInstanced(m_iIndexCountPerInstance, m_iInstanceNum, 0, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Init_Particle(VTXINSTANCING* pVerpostex)
{
	//_vector vDir = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	_vector vDir =  XMVectorSetW(XMLoadFloat3(&m_Instancing_Desc.vDir),0.f);

	uniform_real_distribution<float>	RandomRange(0.1f, m_Instancing_Desc.fRange);
	uniform_real_distribution<float>	RandomRunAngle(m_Instancing_Desc.vRunRotation.x,
		m_Instancing_Desc.vRunRotation.y);
	uniform_real_distribution<float>	RandomSpeed(m_Instancing_Desc.fSpeed.x, m_Instancing_Desc.fSpeed.y);
	uniform_real_distribution<float>	RandomLifeTime(m_Instancing_Desc.fLifeTime.x, m_Instancing_Desc.fLifeTime.y);
	uniform_real_distribution<float>	RandomScale(m_Instancing_Desc.fScale.x, m_Instancing_Desc.fScale.y);
	uniform_real_distribution<float>	RandomRotation(m_Instancing_Desc.vRotation.x, m_Instancing_Desc.vRotation.y);


	for (_uint i = 0; i < m_iInstanceNum; i++)
	{
		m_pSpeeds[i] = RandomSpeed(m_RandomNumber);
		m_pLifeTime[i] = RandomLifeTime(m_RandomNumber);

		m_pRunRotation[i] = XMConvertToRadians(RandomRunAngle(m_RandomNumber));

		m_pScale[i] = RandomScale(m_RandomNumber);

		m_pRotation[i] = 

		vDir = XMVector3Normalize(vDir) * RandomRange(m_RandomNumber);

		//_vector vRot = XMQuaternionRotationRollPitchYaw(RandomAngle(m_RandomNumber), RandomAngle(m_RandomNumber), RandomAngle(m_RandomNumber));
		_vector vRot = XMQuaternionRotationRollPitchYaw(m_Instancing_Desc.vRotation.x,
			m_Instancing_Desc.vRotation.y, m_Instancing_Desc.vRotation.z);

		_matrix matRot = XMMatrixRotationQuaternion(vRot);

		vDir = XMVector3TransformNormal(vDir, matRot);

		pVerpostex[i].vRight = _float4(m_pScale[i], 0.f, 0.f, 0.f);
		pVerpostex[i].vUp = _float4(0.f, m_pScale[i], 0.f, 0.f);
		pVerpostex[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		XMStoreFloat4(&pVerpostex[i].vPos, XMLoadFloat3(&m_Instancing_Desc.vCenter) + vDir);
		pVerpostex[i].vPos.w = 1.f;
		m_pPos[i] = pVerpostex[i].vPos;
		pVerpostex[i].vColor = m_Instancing_Desc.vColor;
	}

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Init_Mesh(VTXINSTANCING* pVerpostex)
{
	for (_uint i = 0; i < m_iInstanceNum; i++)
	{
		memcpy(&pVerpostex[i].vRight, &m_vecInstanceVertex[i].m[0], sizeof(_float4));
		memcpy(&pVerpostex[i].vUp, &m_vecInstanceVertex[i].m[1], sizeof(_float4));
		memcpy(&pVerpostex[i].vLook, &m_vecInstanceVertex[i].m[2], sizeof(_float4));
		memcpy(&pVerpostex[i].vPos, &m_vecInstanceVertex[i].m[3], sizeof(_float4));
		// �ʱ�ȭ ��� ���� ���� ����� �ν��Ͻ� ������ �����Ѵ�.
	}

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Init_InstanceBuffer()
{
	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iInstanceStride * m_iInstanceNum;	// �����迭�� ũ��
	m_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;					// ������������ ������������ ����/ ������ ����
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// ���� ���� ����
	m_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// ������ �ش�
	m_Buffer_Desc.MiscFlags = 0;								// ������ �ش�
	m_Buffer_Desc.StructureByteStride = m_iInstanceStride;		// ���� �ϳ��� ũ�⸦ ���� ����

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	VTXINSTANCING* pVerpostex = new VTXINSTANCING[m_iInstanceNum];		// ���ؽ� ���� �ȿ� ��� �� ������ ��������

	switch (m_eInstanceType)
	{
	case Engine::CVIBuffer_Instancing::TYPE_PARTICLE:
		if (FAILED(Init_Particle(pVerpostex)))
			return E_FAIL;
		break;
	case Engine::CVIBuffer_Instancing::TYPE_MESH:
		if (FAILED(Init_Mesh(pVerpostex)))
			return E_FAIL;
		break;
	}

	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pInstanceBuffer)))		// ���� �Ҵ��� ������ ���۰� �Ҵ��� ������ �����Ͽ� ����
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// ���� �Ҵ��� ���� ���۰� �̹� ����߱⿡ �ʿ� ������ ����

	return S_OK;
}

_vector CVIBuffer_Instancing::CenterToPos(_float4 vPos)
{
	_vector vDir = XMLoadFloat4(&vPos) - XMLoadFloat3(&m_Instancing_Desc.vCenter);
	vDir = XMVectorSetW(vDir, 0.f);

	return vDir;
}

void CVIBuffer_Instancing::Reset(VTXINSTANCING* pInstancing,_uint iIndx)
{
	m_pTimeAcc[iIndx] = 0.f; // ���� �ð� ��
	pInstancing[iIndx].vColor.w = m_Instancing_Desc.vColor.w; // ���� ��

	pInstancing[iIndx].vRight = _float4(m_pScale[iIndx], 0.f, 0.f, 0.f);
	pInstancing[iIndx].vUp = _float4(0.f, m_pScale[iIndx], 0.f, 0.f);
	pInstancing[iIndx].vLook = _float4(0.f, 0.f, 1.f, 0.f);
	// ũ��

	//if (m_pRunRotation[i] != 0.f) {
	//	//_matrix matRot = XMMatrixRotationZ(m_pRunRotation[i] * fTimeDelta);

	//	_vector vRot = XMQuaternionRotationRollPitchYaw(m_pRunRotation[i] * fTimeDelta,
	//		m_pRunRotation[i] * fTimeDelta, m_pRunRotation[i] * fTimeDelta);

	//	_matrix matRot = XMMatrixRotationQuaternion(vRot);

	//	XMStoreFloat4(&Instancing[i].vRight, XMVector3TransformNormal(XMLoadFloat4(&Instancing[i].vRight), matRot));
	//	XMStoreFloat4(&Instancing[i].vUp, XMVector3TransformNormal(XMLoadFloat4(&Instancing[i].vUp), matRot));
	//	XMStoreFloat4(&Instancing[i].vLook, XMVector3TransformNormal(XMLoadFloat4(&Instancing[i].vLook), matRot));
	//} // ȸ��

	//vDir = bRuntimeDir == false ?
	//	CenterToPos(Instancing[i].vPos) : vDir;

	//XMStoreFloat4(&Instancing[i].vPos, XMLoadFloat4(&Instancing[i].vPos) + XMVector3Normalize(vDir) * m_pSpeeds[i] * fTimeDelta);
	// ��ġ
}

void CVIBuffer_Instancing::Free()
{
	__super::Free();

	/*if (m_bClone == false)
	{
		Safe_Delete_Array(m_pSpeeds);
		Safe_Delete_Array(m_pLifeTime);
	}*/
	Safe_Delete_Array(m_pSpeeds);
	Safe_Delete_Array(m_pLifeTime);
	Safe_Delete_Array(m_pRunRotation);
	Safe_Delete_Array(m_pScale);
	Safe_Delete_Array(m_pTimeAcc);
	Safe_Delete_Array(m_pPos);
	Safe_Delete_Array(m_pRotation);

	Safe_Release(m_pInstanceBuffer);
}
