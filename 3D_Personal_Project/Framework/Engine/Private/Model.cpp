#include "..\Public\Model.h"
#include "Mesh.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel& rhs)
	: CComponent(rhs),
	m_iMeshesNum(rhs.m_iMeshesNum),
	m_vecMesh(rhs.m_vecMesh)
{
	for (auto& iter : m_vecMesh)
		Safe_AddRef(iter);
}

HRESULT CModel::Initialize_ProtoType(const string& strModelFilePath, _fmatrix	matPivot)
{
	m_pAiScene = m_Importer.ReadFile(strModelFilePath, aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast);

	if (m_pAiScene == nullptr)
		return E_FAIL;

	if (FAILED(Ready_Meshes(matPivot)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render()
{
	for (auto& iter : m_vecMesh) {
		if (iter != nullptr) {
			iter->Bind_Buffer();
			iter->Render();
		}
	}

	return S_OK;
}

HRESULT CModel::Ready_Meshes(_fmatrix	matPivot)
{
	m_iMeshesNum = m_pAiScene->mNumMeshes;

	m_vecMesh.reserve(m_iMeshesNum);

	for (_uint i = 0; i < m_iMeshesNum; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_pAiScene->mMeshes[i], matPivot);

		if (pMesh == nullptr)
			return E_FAIL;

		m_vecMesh.push_back(pMesh);
	}

	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& strModelFilePath, _fmatrix	matPivot)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(strModelFilePath, matPivot))) {
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto* iter : m_vecMesh)
		Safe_Release(iter);
	m_vecMesh.clear();

	if(!m_bClone)
		m_Importer.FreeScene();
}
