#include "..\Public\Model_Instancing.h"

#include "GameInstance.h"
#include "Mesh.h"

CModel_Instancing::CModel_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer_Instancing(pDevice, pContext)
{
}

CModel_Instancing::CModel_Instancing(const CModel_Instancing& rhs)
	: CVIBuffer_Instancing(rhs)
{
}

HRESULT CModel_Instancing::Initialize_ProtoType(const string& strModelFilePath,_fmatrix matPivot)
{
	m_eInstanceType = INSTANCING_TYPE::TYPE_MESH;

	XMStoreFloat4x4(&m_matPivot, matPivot);

	CMeshData::MESHDATADESC MeshData = {};

	CMeshData* pMeshData = CMeshData::Create(MeshData);

	if (FAILED(pMeshData->Load_Data(strModelFilePath.c_str())))
		return E_FAIL;

	if (FAILED(pMeshData->Data_Get(MeshData)))
		return E_FAIL;

	if (FAILED(Ready_Meshes(MeshData)))
		return E_FAIL;

	if (FAILED(Ready_Materials(MeshData, strModelFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel_Instancing::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	MESH_INSTANCE_DESC* pInstanceDesc = ((MESH_INSTANCE_DESC*)pArg);

	m_vecInstanceVertex = pInstanceDesc->vecInstanceVertex;

	m_iInstanceNum = pInstanceDesc->vecInstanceVertex.size();
	m_iInstanceStride = sizeof(VTXINSTANCING);
	/*m_iInstanceStride = sizeof(VTXINSTANCING);
	for (auto& iter : m_vecMesh)
	{
		m_iIndexCountPerInstance += iter->Get_IndexInfo().size();
	}*/

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel_Instancing::Render(_uint iMeshIndx)
{
	/*if (nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;*/

	if (iMeshIndx >= m_vecMesh.size())
		return E_FAIL;

	ID3D11Buffer* pVerTexBuffers[] = {
		m_vecMesh[iMeshIndx]->Get_VertexBuffer(),
		m_pInstanceBuffer
	};	// 버텍스 버퍼들 배열

	_uint			iStrides[] = {
		m_vecMesh[iMeshIndx]->Get_VertexStride(),
		m_iInstanceStride
	};	// 버텍스 버퍼들의 정점 하나의 크기 배열

	_uint			iOffsets[] = {
		0,
		0
	}; // 버텍스 버퍼들의 offset

	m_iVertexBuffersNum = 2;

	/* 버텍스 버퍼를 바인딩*/
	m_pContext->IASetVertexBuffers(0, m_iVertexBuffersNum, pVerTexBuffers, iStrides, iOffsets);

	/* 인덱스 버퍼를 바인딩*/
	m_pContext->IASetIndexBuffer(m_vecMesh[iMeshIndx]->Get_IndexBuffer(),
		m_vecMesh[iMeshIndx]->Get_IndexFormat(), 0);

	/* 정점을 이용해서 어떤식으로 그릴 것인지를 설정*/
	m_pContext->IASetPrimitiveTopology(m_vecMesh[iMeshIndx]->Get_Topology());

	m_pContext->DrawIndexedInstanced(m_vecMesh[iMeshIndx]->Get_IndexNum(),
		m_iInstanceNum, 0, 0, 0);

	return S_OK;
}

HRESULT CModel_Instancing::Ready_Meshes(CMeshData::MESHDATADESC MeshData)
{
	m_iMeshesNum = MeshData.iMeshNum;

	m_vecMesh.reserve(m_iMeshesNum);

	CModel::BONES vecBones;

	for (_uint i = 0; i < m_iMeshesNum; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, CModel::TYPE::TYPE_NONANIM, MeshData.vecMesh[i], XMLoadFloat4x4(&m_matPivot), vecBones);
		if (pMesh == nullptr)
			return E_FAIL;

		m_vecMesh.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel_Instancing::Ready_Materials(CMeshData::MESHDATADESC MeshData, const string& strModelFilePath)
{
	m_iMaterialsNum = MeshData.iMaterialNum;

	for (_uint i = 0; i < m_iMaterialsNum; i++)
	{
		MATERIAL_DESC Material_Desc = {};

		for (_uint j = 1; j < (_uint)TEXTURETYPE::TYPE_UNKNOWN; j++)
		{
			_char szDdrive[MAX_PATH] = "";
			_char szDirectory[MAX_PATH] = "";

			_splitpath_s(strModelFilePath.c_str(), szDdrive, MAX_PATH, szDirectory, MAX_PATH,
				nullptr, 0, nullptr, 0);

			string	szGetPath = MeshData.vecMaterial[i].vecMaterialPath[j];
			if (!strcmp(szGetPath.c_str(), ""))
				continue;

			_char	szFileName[MAX_PATH] = "";
			_char	szExc[MAX_PATH] = "";

			_splitpath_s(szGetPath.c_str(), nullptr, 0, nullptr, 0,
				szFileName, MAX_PATH, szExc, MAX_PATH);

			_char	szTmp[MAX_PATH] = "";

			strcpy_s(szTmp, szDdrive);
			strcat_s(szTmp, szDirectory);
			strcat_s(szTmp, szFileName);
			strcat_s(szTmp, szExc);

			_tchar	szFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szTmp, strlen(szTmp), szFullPath, MAX_PATH);

			Material_Desc.pMtrlTexture[j] = CTexture::Create(m_pDevice, m_pContext, szFullPath, 1);
			if (Material_Desc.pMtrlTexture[j] == nullptr)
			{
				//return E_FAIL;
				continue;
			}

		}
		m_vecMaterial.push_back(Material_Desc);
	}

	return S_OK;
}

CModel_Instancing* CModel_Instancing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	const string& strModelFilePath,_fmatrix matPivot)
{
	CModel_Instancing* pInstance = new CModel_Instancing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(strModelFilePath, matPivot))) {
		MSG_BOX("Failed to Created : CModel_Instancing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel_Instancing::Clone(void* pArg)
{
	CModel_Instancing* pInstance = new CModel_Instancing(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CModel_Instancing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel_Instancing::Free()
{
	__super::Free();

	for (auto& iter1 : m_vecMaterial)
	{
		for (auto& iter2 : iter1.pMtrlTexture)
			Safe_Release(iter2);
	}

	for (auto* iter : m_vecMesh)
		Safe_Release(iter);
	m_vecMesh.clear();
}
