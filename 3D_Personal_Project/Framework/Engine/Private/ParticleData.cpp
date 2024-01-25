#include "..\Public\ParticleData.h"
#include "GameInstance.h"

CParticleData::CParticleData()
{
}

HRESULT CParticleData::Initialize()
{
	return S_OK;
}

HRESULT CParticleData::Save_Data(const char* strPath)
{
	ofstream fout;

	fout.open(strPath, std::ofstream::binary);

	if (fout.is_open())
	{
		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.vCenter), sizeof(_float3));
		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fRange), sizeof(_float));

		for (_uint i = 0; i < 3; i++)
			fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fSpeed[i]), sizeof(_float2));
		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fPowerSpeed), sizeof(_float3));

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fScale), sizeof(_float2));
		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fScaleControl), sizeof(_float));

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fLifeTime), sizeof(_float2));

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.vColor), sizeof(_float4));

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.vDir), sizeof(_float3));
		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.vRunDir), sizeof(_float3));

		for(_uint i=0;i<3;i++)
			fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fRotation[i]), sizeof(_float2));
		for (_uint i = 0; i < 3; i++)
			fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.fRunRotation[i]), sizeof(_float2));

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.bLoop), sizeof(_bool));

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.iInstanceNum), sizeof(_int));

		
		string strTag = CUtility_String::WString_To_string(m_Instancing_Desc.strTextureTag);
		size_t istrSize = strTag.size();
		fout.write(reinterpret_cast<const char*>(&istrSize), sizeof(size_t));
		fout.write(strTag.c_str(), istrSize);

		fout.write(reinterpret_cast<const char*>(&m_Instancing_Desc.eColorType), sizeof(m_Instancing_Desc.eColorType));
		
	}
	else
		return E_FAIL;

	fout.close();

	return S_OK;
}

HRESULT CParticleData::Load_Data(const char* strPath)
{
	ifstream fIn;

	fIn.open(strPath, std::ios::binary);

	if (fIn.is_open())
	{

		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.vCenter), sizeof(_float3));
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fRange), sizeof(_float));

		for (_uint i = 0; i < 3; i++)
			fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fSpeed[i]), sizeof(_float2));
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fPowerSpeed), sizeof(_float3));

		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fScale), sizeof(_float2));
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fScaleControl), sizeof(_float));
		
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fLifeTime), sizeof(_float2));
		
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.vColor), sizeof(_float4));
		
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.vDir), sizeof(_float3));
		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.vRunDir), sizeof(_float3));

		for (_uint i = 0; i < 3; i++)
			fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fRotation[i]), sizeof(_float2));
		for (_uint i = 0; i < 3; i++)
			fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.fRunRotation[i]), sizeof(_float2));

		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.bLoop), sizeof(_bool));

		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.iInstanceNum), sizeof(_int));

		size_t istrSize;
		fIn.read(reinterpret_cast<char*>(&istrSize), sizeof(size_t));
		string strTag;
		strTag.resize(istrSize);
		fIn.read(&strTag[0], istrSize);
		m_Instancing_Desc.strTextureTag = CUtility_String::string_To_Wstring(strTag);

		fIn.read(reinterpret_cast<char*>(&m_Instancing_Desc.eColorType), sizeof(m_Instancing_Desc.eColorType));
	}
	else
		return E_FAIL;

	fIn.close();

	return S_OK;
}

CParticleData* CParticleData::Create()
{
	CParticleData* pInstance = new CParticleData();

	if (FAILED(pInstance->Initialize())) {
		MSG_BOX("Failed to Created : CParticleData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticleData::Free()
{
	__super::Free();
}
