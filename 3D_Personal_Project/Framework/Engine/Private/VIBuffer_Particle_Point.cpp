#include "..\Public\VIBuffer_Particle_Point.h"
#include "GameInstance.h"

CVIBuffer_Particle_Point::CVIBuffer_Particle_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer_Instancing(pDevice, pContext)
{
}

CVIBuffer_Particle_Point::CVIBuffer_Particle_Point(const CVIBuffer_Particle_Point& rhs)
	:CVIBuffer_Instancing(rhs)
{
}

HRESULT CVIBuffer_Particle_Point::Initialize_ProtoType()
{

	/*if (FAILED(Init_Buffer()))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	INSTANCING_DESC* pPoint_Desc = (INSTANCING_DESC*)pArg;

	m_Instancing_Desc.vCenter = pPoint_Desc->vCenter;
	m_Instancing_Desc.fLifeTime = pPoint_Desc->fLifeTime;
	m_Instancing_Desc.fRange = pPoint_Desc->fRange;

	m_Instancing_Desc.fSpeed[0] = pPoint_Desc->fSpeed[0];
	m_Instancing_Desc.fSpeed[1] = pPoint_Desc->fSpeed[1];
	m_Instancing_Desc.fSpeed[2] = pPoint_Desc->fSpeed[2];
	m_Instancing_Desc.fPowerSpeed = pPoint_Desc->fPowerSpeed;
	// ���ǵ�� ���ǵ� ���ӵ� �Ŀ�

	m_Instancing_Desc.fScale = pPoint_Desc->fScale;
	m_Instancing_Desc.fScaleControl = pPoint_Desc->fScaleControl;
	// ũ�� ���� ũ�� �ӵ� ��Ʈ�� ��

	m_Instancing_Desc.vColor = pPoint_Desc->vColor;
	// ����

	m_Instancing_Desc.vDir = pPoint_Desc->vDir; // ������ �� ����ϴ� ���� ����
	m_Instancing_Desc.vRunDir = pPoint_Desc->vRunDir; // �ǽð����� ������ �� ����ϴ� ���� ����

	m_Instancing_Desc.fRotation[0] = pPoint_Desc->fRotation[0];
	m_Instancing_Desc.fRotation[1] = pPoint_Desc->fRotation[1];
	m_Instancing_Desc.fRotation[2] = pPoint_Desc->fRotation[2];
	// ���� ȸ�� xyz ���� ��

	m_Instancing_Desc.fRunRotation[0] = pPoint_Desc->fRunRotation[0];
	m_Instancing_Desc.fRunRotation[1] = pPoint_Desc->fRunRotation[1];
	m_Instancing_Desc.fRunRotation[2] = pPoint_Desc->fRunRotation[2];
	// �ǽð� ȸ�� xyz ���� ��

	m_Instancing_Desc.bLoop = pPoint_Desc->bLoop;
	// ���� ������ ��������

	m_Instancing_Desc.iInstanceNum = pPoint_Desc->iInstanceNum;
	// �ν��Ͻ� ����

	m_Instancing_Desc.eColorType = pPoint_Desc->eColorType;
	
	if (FAILED(Init_Buffer())) return E_FAIL;
	if (FAILED(__super::Init_InstanceBuffer())) return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Save_Particle(const _char* strFilePath)
{
	ofstream fout;

	fout.open(strFilePath, std::ofstream::binary);

	if (fout.is_open())
	{
		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.iInstanceNum), sizeof(_int));
		// �ν��Ͻ� ����
		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.vCenter), sizeof(_float3));
		// ���� ��
		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fRange), sizeof(_float));
		// ���� ��

		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
		{
			fout.write(reinterpret_cast<const char*>(&m_pSpeeds[i]), sizeof(_float3));
		}
		// ���� ���ǵ� ��

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fPowerSpeed), sizeof(_float3));
		// ���ǵ� �Ŀ� ��

		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
		{
			fout.write(reinterpret_cast<const char*>(&m_pScale[i]), sizeof(_float));
		}
		// ���� ũ�� ��

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fScaleControl), sizeof(_float));
		// ũ�� ��Ʈ�� ��

		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
		{
			fout.write(reinterpret_cast<const char*>(&m_pLifeTime[i]), sizeof(_float));
		}
		// ���� ������Ÿ�� ��

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.vColor), sizeof(_float4));
		// ����

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.vRunDir), sizeof(_float3));
		// �ǽð� ���� ��

		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
		{
			fout.write(reinterpret_cast<const char*>(&m_pRunRotation[i]), sizeof(_float3));
		}
		// �ǽð� ȸ�� ��

		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
		{
			fout.write(reinterpret_cast<const char*>(&m_pPos[i]), sizeof(_float4));
		}
		// ó�� ��ġ ��

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.bLoop), sizeof(_bool));
		// ���ѷ��� �Ǵ�

		string strTag = CUtility_String::WString_To_string(m_Instancing_Desc.strTextureTag);
		size_t istrSize = strTag.size();
		fout.write(reinterpret_cast<const char*>(&istrSize), sizeof(size_t));
		fout.write(strTag.c_str(), istrSize);
		// �ؽ��� �ױ�

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.eColorType), sizeof(m_Instancing_Desc.eColorType));
		// �ؽ��� �÷� Ÿ��

	}
	else
		return E_FAIL;

	fout.close();

	return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Load_Particle(const _char* strFilePath)
{
	ifstream fIn;

	fIn.open(strFilePath, std::ios::binary);

	if (fIn.is_open())
	{
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.iInstanceNum), sizeof(_int));
		// �ν��Ͻ� ����
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.vCenter), sizeof(_float3));
		//����
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fRange), sizeof(_float));
		//����
		for(_uint i = 0;i< m_Instancing_Desc.iInstanceNum;i++)
			fIn.read(reinterpret_cast<char*>(&m_pSpeeds[i]), sizeof(_float3));
		// ������ ���ǵ�
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fPowerSpeed), sizeof(_float3));
		// ���ǵ� �Ŀ�
		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
			fIn.read(reinterpret_cast<char*>(&m_pScale[i]), sizeof(_float));
		// ������ ũ��
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fScaleControl), sizeof(_float));
		// ũ�� ��Ʈ�� ��
		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
			fIn.read(reinterpret_cast<char*>(&m_pLifeTime[i]), sizeof(_float));
		// ������ ������ Ÿ��
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.vColor), sizeof(_float4));
		// ����
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.vRunDir), sizeof(_float3));
		// �ǽð� ����
		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
			fIn.read(reinterpret_cast<char*>(&m_pRunRotation[i]), sizeof(_float3));
		// ������ �ǽð� ȸ�� ����
		for (_uint i = 0; i < m_Instancing_Desc.iInstanceNum; i++)
			fIn.read(reinterpret_cast<char*>(&m_pPos[i]), sizeof(_float4));
		// ������ ó�� ��ġ ��
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.bLoop), sizeof(_bool));
		// ���ѷ��� �Ǵ�
		size_t TagSize;
		fIn.read(reinterpret_cast<char*>(&TagSize), sizeof(size_t));
		string strTag;
		strTag.resize(TagSize);
		fIn.read(&strTag[0], TagSize);
		m_Instancing_Desc.strTextureTag = CUtility_String::string_To_Wstring(strTag);
		// �ؽ��� �ױ� ��
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.eColorType), sizeof(m_Instancing_Desc.eColorType));
		// �ؽ��� �÷� Ÿ��
	}
	else
		return E_FAIL;

	fIn.close();

	return S_OK;
}

HRESULT CVIBuffer_Particle_Point::Init_Buffer()
{
	m_eInstanceType = INSTANCING_TYPE::TYPE_PARTICLE;

	m_iVertexBuffersNum = 2;
	m_iVertexNum = 1;
	m_iVertexStride = sizeof(VTXPOINT);

	//m_iInstanceNum = iInstanceNum;
	m_iInstanceNum = m_Instancing_Desc.iInstanceNum;
	m_iInstanceStride = sizeof(VTXINSTANCING);
	m_iIndexCountPerInstance = 1;

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

	m_iIndexNum = m_iInstanceNum;
	m_iIndexStride = 2;
	m_eIndexForMat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iVertexNum;	// �����迭�� ũ��
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;					// ������������ ������������ ����/ ������ ����
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// ���� ���� ����
	m_Buffer_Desc.CPUAccessFlags = 0;							// ������ �ش�
	m_Buffer_Desc.MiscFlags = 0;								// ������ �ش�
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;		// ���� �ϳ��� ũ�⸦ ���� ����

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	VTXPOINT* pVerpostex = new VTXPOINT;		// ���ؽ� ���� �ȿ� ��� �� ������ ��������

	pVerpostex->fPosition = _float3(0.f, 0.f, 0.f);
	pVerpostex->fPSize = _float2(1.f, 1.f);

	m_SubResource_Data.pSysMem = pVerpostex;

	if (FAILED(Create_Buffer(&m_pVB)))		// ���� �Ҵ��� ������ ���۰� �Ҵ��� ������ �����Ͽ� ����
		return E_FAIL;

	Safe_Delete_Array(pVerpostex);			// ���� �Ҵ��� ���� ���۰� �̹� ����߱⿡ �ʿ� ������ ����

	ZeroMemory(&m_Buffer_Desc, sizeof(m_Buffer_Desc));

	m_Buffer_Desc.ByteWidth = m_iIndexStride * m_iIndexNum;
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_Buffer_Desc.CPUAccessFlags = 0;
	m_Buffer_Desc.MiscFlags = 0;
	m_Buffer_Desc.StructureByteStride = 0;

	ZeroMemory(&m_SubResource_Data, sizeof(m_SubResource_Data));

	_ushort* pIndex = new _ushort[m_iIndexNum];

	ZeroMemory(pIndex, sizeof(_ushort) * m_iIndexNum);

	m_SubResource_Data.pSysMem = pIndex;

	if (FAILED(Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndex);

	return S_OK;
}

CVIBuffer_Particle_Point* CVIBuffer_Particle_Point::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Particle_Point* pInstance = new CVIBuffer_Particle_Point(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType())) {
		MSG_BOX("Failed to Created : CVIBuffer_Particle_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Particle_Point::Clone(void* pArg)
{
	CVIBuffer_Particle_Point* pInstance = new CVIBuffer_Particle_Point(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CVIBuffer_Particle_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Particle_Point::Free()
{
	__super::Free();
}
