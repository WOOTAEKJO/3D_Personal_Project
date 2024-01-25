#include "..\Public\VIBuffer_Instancing.h"

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Instancing::CVIBuffer_Instancing(const CVIBuffer_Instancing& rhs)
	:CVIBuffer(rhs),m_iInstanceNum(rhs.m_iInstanceNum),m_iInstanceStride(rhs.m_iInstanceStride)
	,m_iIndexCountPerInstance(rhs.m_iIndexCountPerInstance),m_RandomNumber(rhs.m_RandomNumber),
	m_pSpeeds(rhs.m_pSpeeds),m_pLifeTime(rhs.m_pLifeTime), m_Instancing_Desc(rhs.m_Instancing_Desc),
	m_eInstanceType(rhs.m_eInstanceType), m_bLoad(rhs.m_bLoad),
	m_pScale(rhs.m_pScale),m_pPos(rhs.m_pPos),m_pRunRotation(rhs.m_pRunRotation),m_pSpeedAcc(rhs.m_pSpeedAcc),
	m_pTimeAcc(rhs.m_pTimeAcc), m_pScaleTimeAcc(rhs.m_pTimeAcc), m_pRotation(rhs.m_pRotation),
	m_pRunRotationAcc(rhs.m_pRunRotationAcc)
{

	/*_float3* m_pSpeeds = { nullptr };
	_float3* m_pSpeedAcc = { nullptr };

	_float* m_pLifeTime = { nullptr };
	_float* m_pTimeAcc = { nullptr };

	_float* m_pScale = { nullptr };
	_float* m_pScaleTimeAcc = { nullptr };

	_float4* m_pPos = { nullptr };

	_float3* m_pRotation = { nullptr };
	_float3* m_pRunRotation = { nullptr };*/
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

	D3D11_MAPPED_SUBRESOURCE Subresource = {};

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0,&Subresource);

	VTXINSTANCING* Instancing = ((VTXINSTANCING*)Subresource.pData);

	//_vector vDir;
	//_bool bRuntimeDir = { false };
	//if (!XMVector3Equal(XMLoadFloat3(&m_Instancing_Desc.vRunDir), XMVectorZero()))
	//{
	//	bRuntimeDir = true;
	//	vDir = XMLoadFloat3(&m_Instancing_Desc.vRunDir);
	//} // 정해진 방향이 0이 아니면 정해진 방향대로 위치 값을 변경
		
	for (_uint i = 0; i < m_iInstanceNum; i++)
	{
		_float fScale = 0.f;
		
		if (m_pLifeTime[i] != 0.f)
		{
			if (m_pLifeTime[i] <= m_pTimeAcc[i] && m_Instancing_Desc.bLoop)
				Reset(Instancing, i);

			m_pTimeAcc[i] += fTimeDelta;
			_float fLifeTime = max(m_pLifeTime[i] - m_pTimeAcc[i], 0.f);
			Instancing[i].vColor.w = fLifeTime; // 알파 값
			// 라이프타임이 있을 경우

			//fScale = min(m_pScale[i] * fLifeTime, m_pScale[i]);

			m_pScaleTimeAcc[i] += fTimeDelta * m_Instancing_Desc.fScaleControl;
			fScale = max(m_pScale[i] - m_pScaleTimeAcc[i], 0.0001f);
		}
		else
		{
			if (m_pScale[i] <= m_pScaleTimeAcc[i] && m_Instancing_Desc.bLoop)
				Reset(Instancing, i);

			m_pScaleTimeAcc[i] += fTimeDelta * m_Instancing_Desc.fScaleControl;
			fScale = max(m_pScale[i] - m_pScaleTimeAcc[i], 0.0001f);
		}

		if (fScale < 0.f)
			int a = 0;

		if (m_pRunRotation[i].x != 0 || m_pRunRotation[i].y != 0 || m_pRunRotation[i].z != 0)
		{
			Instancing[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
			Instancing[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
			Instancing[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);

			m_pRunRotationAcc[i].x += m_pRunRotation[i].x * fTimeDelta;
			m_pRunRotationAcc[i].y += m_pRunRotation[i].y * fTimeDelta;
			m_pRunRotationAcc[i].z += m_pRunRotation[i].z * fTimeDelta;

			_vector vRot = XMQuaternionRotationRollPitchYaw(m_pRunRotationAcc[i].x,
				m_pRunRotationAcc[i].y, m_pRunRotationAcc[i].z);
			_matrix matRot = XMMatrixRotationQuaternion(vRot);
			XMStoreFloat4(&Instancing[i].vRight,
				XMVector3TransformNormal((XMLoadFloat4(&Instancing[i].vRight)), matRot));
			XMStoreFloat4(&Instancing[i].vUp,
				XMVector3TransformNormal((XMLoadFloat4(&Instancing[i].vUp)), matRot));
			XMStoreFloat4(&Instancing[i].vLook,
				XMVector3TransformNormal((XMLoadFloat4(&Instancing[i].vLook)), matRot));

			// 크기 및 회전
		}
		else {
			/*Instancing[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
			Instancing[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
			Instancing[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);*/

			XMStoreFloat4(&Instancing[i].vRight,XMVector3Normalize(XMLoadFloat4(&Instancing[i].vRight)) * fScale);
			XMStoreFloat4(&Instancing[i].vUp, XMVector3Normalize(XMLoadFloat4(&Instancing[i].vUp)) * fScale);
			XMStoreFloat4(&Instancing[i].vLook, XMVector3Normalize(XMLoadFloat4(&Instancing[i].vLook)) * fScale);
		}

		/*vDir = bRuntimeDir == false ?
			CenterToPos(Instancing[i].vPos) : vDir;*/

		/*XMStoreFloat3(&m_pSpeedAcc[i], XMLoadFloat3(&m_pSpeedAcc[i]) + XMLoadFloat3(&m_Instancing_Desc.fPowerSpeed) * fTimeDelta);
		XMStoreFloat4(&Instancing[i].vPos, XMLoadFloat4(&Instancing[i].vPos) +
			XMVector3Normalize(vDir) * XMLoadFloat3(&m_pSpeedAcc[i]) * fTimeDelta);*/
		XMStoreFloat3(&m_pSpeedAcc[i], XMLoadFloat3(&m_pSpeedAcc[i]) + XMLoadFloat3(&m_Instancing_Desc.fPowerSpeed) * fTimeDelta);
		XMStoreFloat4(&Instancing[i].vPos, XMLoadFloat4(&Instancing[i].vPos) +
			XMVector3Normalize(XMLoadFloat3(&m_pDir[i])) * XMLoadFloat3(&m_pSpeedAcc[i]) * fTimeDelta);
		// 위치 -> 한 방향으로만 또는 원래 방향으로만
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
	_vector vDir =  XMVectorSetW(XMLoadFloat3(&m_Instancing_Desc.vDir),0.f);

	uniform_real_distribution<float>	RandomRange(0.001f, m_Instancing_Desc.fRange);

	uniform_real_distribution<float>	RandomSpeedX(m_Instancing_Desc.fSpeed[0].x, m_Instancing_Desc.fSpeed[0].y);
	uniform_real_distribution<float>	RandomSpeedY(m_Instancing_Desc.fSpeed[1].x, m_Instancing_Desc.fSpeed[1].y);
	uniform_real_distribution<float>	RandomSpeedZ(m_Instancing_Desc.fSpeed[2].x, m_Instancing_Desc.fSpeed[2].y);

	uniform_real_distribution<float>	RandomLifeTime(m_Instancing_Desc.fLifeTime.x, m_Instancing_Desc.fLifeTime.y);

	uniform_real_distribution<float>	RandomScale(m_Instancing_Desc.fScale.x, m_Instancing_Desc.fScale.y);

	uniform_real_distribution<float>	RandomAngleX(m_Instancing_Desc.fRotation[0].x, m_Instancing_Desc.fRotation[0].y);
	uniform_real_distribution<float>	RandomAngleY(m_Instancing_Desc.fRotation[1].x, m_Instancing_Desc.fRotation[1].y);
	uniform_real_distribution<float>	RandomAngleZ(m_Instancing_Desc.fRotation[2].x, m_Instancing_Desc.fRotation[2].y);

	uniform_real_distribution<float>	RandomRunAngleX(m_Instancing_Desc.fRunRotation[0].x, m_Instancing_Desc.fRunRotation[0].y);
	uniform_real_distribution<float>	RandomRunAngleY(m_Instancing_Desc.fRunRotation[1].x, m_Instancing_Desc.fRunRotation[1].y);
	uniform_real_distribution<float>	RandomRunAngleZ(m_Instancing_Desc.fRunRotation[2].x, m_Instancing_Desc.fRunRotation[2].y);

	for (_uint i = 0; i < m_iInstanceNum; i++)
	{
		m_pScale[i] = RandomScale(m_RandomNumber);
		pVerpostex[i].vRight = _float4(m_pScale[i], 0.f, 0.f, 0.f);
		pVerpostex[i].vUp = _float4(0.f, m_pScale[i], 0.f, 0.f);
		pVerpostex[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		// 크기

		m_pRunRotation[i] = _float3(XMConvertToRadians(RandomRunAngleX(m_RandomNumber)),
			XMConvertToRadians(RandomRunAngleY(m_RandomNumber)),
			XMConvertToRadians(RandomRunAngleZ(m_RandomNumber)));
		// 실시간 회전

		_vector vRot = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(RandomAngleX(m_RandomNumber)),
			XMConvertToRadians(RandomAngleY(m_RandomNumber)),
			XMConvertToRadians(RandomAngleZ(m_RandomNumber)));
		_matrix matRot = XMMatrixRotationQuaternion(vRot);
		XMStoreFloat4(&pVerpostex[i].vRight,
			XMVector3TransformNormal((XMLoadFloat4(&pVerpostex[i].vRight)), matRot));
		XMStoreFloat4(&pVerpostex[i].vUp,
			XMVector3TransformNormal((XMLoadFloat4(&pVerpostex[i].vUp)), matRot));
		XMStoreFloat4(&pVerpostex[i].vLook,
			XMVector3TransformNormal((XMLoadFloat4(&pVerpostex[i].vLook)), matRot));
		//회전 값

		m_pSpeeds[i] = _float3(RandomSpeedX(m_RandomNumber), RandomSpeedY(m_RandomNumber), RandomSpeedZ(m_RandomNumber));
		m_pSpeedAcc[i] = m_pSpeeds[i];
		// 스피드
		m_pLifeTime[i] = RandomLifeTime(m_RandomNumber);
		// 라이프 타임
		
		m_pRotation[i] = _float3(XMConvertToRadians(RandomAngleX(m_RandomNumber)),
			XMConvertToRadians(RandomAngleY(m_RandomNumber)),
			XMConvertToRadians(RandomAngleZ(m_RandomNumber)));
		vDir = XMVector3Normalize(vDir) * RandomRange(m_RandomNumber);
		vRot = XMQuaternionRotationRollPitchYaw(m_pRotation[i].x,
			m_pRotation[i].y, m_pRotation[i].z);
		matRot = XMMatrixRotationQuaternion(vRot);

		vDir = XMVector3TransformNormal(vDir, matRot);
		XMStoreFloat4(&pVerpostex[i].vPos, XMLoadFloat3(&m_Instancing_Desc.vCenter) + vDir);
		pVerpostex[i].vPos.w = 1.f;
		m_pPos[i] = pVerpostex[i].vPos;
		// 위치

		pVerpostex[i].vColor = m_Instancing_Desc.vColor;
		// 색깔

		if (XMVector3Equal(XMLoadFloat3(&m_Instancing_Desc.vRunDir), XMVectorZero()))
		{
			//vDir = XMLoadFloat3(&m_Instancing_Desc.vRunDir);
			
			XMStoreFloat3( &m_pDir[i],  CenterToPos(m_pPos[i]));

		} // 정해진 방향이 0이면 현재 위치에서 방향 벡터 구하기
		else {
			XMStoreFloat3(&m_pDir[i], XMLoadFloat3(&m_Instancing_Desc.vRunDir) + vDir);

		} // 정해진 방향이 있으면 원래 방향에서 정해진 방향을 더해 방향을 구한다.
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
		// 초기화 당시 받은 상태 행렬을 인스턴스 정점에 저장한다.
	}

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Init_InstanceBuffer()
{
	m_pSpeeds = new _float3[m_iInstanceNum];
	m_pSpeedAcc = new _float3[m_iInstanceNum];
	ZeroMemory(m_pSpeedAcc, sizeof(_float3) * m_iInstanceNum);

	m_pLifeTime = new _float[m_iInstanceNum];
	m_pTimeAcc = new _float[m_iInstanceNum];
	ZeroMemory(m_pTimeAcc, sizeof(_float) * m_iInstanceNum);

	m_pScale = new _float[m_iInstanceNum];
	m_pScaleTimeAcc = new _float[m_iInstanceNum];
	ZeroMemory(m_pScaleTimeAcc, sizeof(_float) * m_iInstanceNum);

	m_pPos = new _float4[m_iInstanceNum];

	m_pRotation = new _float3[m_iInstanceNum];
	m_pRunRotation = new _float3[m_iInstanceNum];
	m_pRunRotationAcc = new _float3[m_iInstanceNum];
	ZeroMemory(m_pRunRotationAcc, sizeof(_float3)* m_iInstanceNum);

	m_pDir = new _float3[m_iInstanceNum];

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iInstanceStride * m_iInstanceNum;	// 동적배열의 크기
	m_Buffer_Desc.Usage = D3D11_USAGE_DYNAMIC;					// 정적버퍼인지 동적버퍼인지 설정/ 지금은 정적
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// 버퍼 종류 설정
	m_Buffer_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// 동적만 해당
	m_Buffer_Desc.MiscFlags = 0;								// 동적만 해당
	m_Buffer_Desc.StructureByteStride = m_iInstanceStride;		// 정점 하나의 크기를 집어 넣음

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	VTXINSTANCING* pVerpostex = new VTXINSTANCING[m_iInstanceNum];		// 버텍스 버퍼 안에 들어 갈 값들을 설정해줌

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

	if (FAILED(Create_Buffer(&m_pInstanceBuffer)))		// 내가 할당한 값들을 버퍼가 할당한 공간에 복사하여 생성
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// 내가 할당한 것은 버퍼가 이미 사용했기에 필요 없으니 해제

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
	m_pTimeAcc[iIndx] = 0.f; // 누적 시간 값
	pInstancing[iIndx].vColor.w = m_Instancing_Desc.vColor.w; // 알파 값

	m_pScaleTimeAcc[iIndx] = 0.f; // 누적 크기 값
	pInstancing[iIndx].vRight = _float4(m_pScale[iIndx], 0.f, 0.f, 0.f);
	pInstancing[iIndx].vUp = _float4(0.f, m_pScale[iIndx], 0.f, 0.f);
	pInstancing[iIndx].vLook = _float4(0.f, 0.f, 1.f, 0.f);
	// 크기

	m_pSpeedAcc[iIndx] = m_pSpeeds[iIndx];
	//스피드 값

	_vector vRot = XMQuaternionRotationRollPitchYaw(m_pRotation[iIndx].x,
		m_pRotation[iIndx].y, m_pRotation[iIndx].z);
	_matrix matRot = XMMatrixRotationQuaternion(vRot);
	XMStoreFloat4(&pInstancing[iIndx].vRight, XMVector3TransformNormal(XMLoadFloat4(&pInstancing[iIndx].vRight), matRot));
	XMStoreFloat4(&pInstancing[iIndx].vUp, XMVector3TransformNormal(XMLoadFloat4(&pInstancing[iIndx].vUp), matRot));
	XMStoreFloat4(&pInstancing[iIndx].vLook, XMVector3TransformNormal(XMLoadFloat4(&pInstancing[iIndx].vLook), matRot));
	// 회전

	m_pRunRotationAcc[iIndx] = _float3(0.f, 0.f, 0.f);

	pInstancing[iIndx].vPos = m_pPos[iIndx];
	//위치
}

void CVIBuffer_Instancing::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pSpeeds);
	Safe_Delete_Array(m_pLifeTime);
	Safe_Delete_Array(m_pRunRotation);
	Safe_Delete_Array(m_pScale);
	Safe_Delete_Array(m_pTimeAcc);
	Safe_Delete_Array(m_pPos);
	Safe_Delete_Array(m_pRotation);
	Safe_Delete_Array(m_pScaleTimeAcc);
	Safe_Delete_Array(m_pSpeedAcc);
	Safe_Delete_Array(m_pDir);
	
	Safe_Delete_Array(m_pRunRotationAcc);

	Safe_Release(m_pInstanceBuffer);
}
