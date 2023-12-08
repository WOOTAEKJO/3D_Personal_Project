#include "..\Public\Model.h"

#include "Mesh.h"
#include "Bone.h"
#include "Texture.h"
#include "Animation.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel& rhs)
	: CComponent(rhs),
	m_eType(rhs.m_eType),
	m_iMeshesNum(rhs.m_iMeshesNum),
	m_iMaterialsNum(rhs.m_iMaterialsNum),
	m_vecMesh(rhs.m_vecMesh),
	m_vecMaterial(rhs.m_vecMaterial),
	m_vecBones(rhs.m_vecBones),
	m_matPivot(rhs.m_matPivot),
	m_iAnimationNum(rhs.m_iAnimationNum),
	m_iCurrentAnimationIndex(rhs.m_iCurrentAnimationIndex),
	m_vecAnimation(rhs.m_vecAnimation)
{
	for (auto& iter1 : m_vecMaterial)
	{
		for (auto& iter2 : iter1.pMtrlTexture)
			Safe_AddRef(iter2);
	}

	for (auto& iter : m_vecMesh)
		Safe_AddRef(iter);

	for (auto& iter : m_vecBones)
		Safe_AddRef(iter);

	for (auto& iter : m_vecAnimation)
		Safe_AddRef(iter);
}

HRESULT CModel::Initialize_ProtoType(TYPE eType, const string& strModelFilePath, _fmatrix	matPivot)
{
	m_eType = eType;

	_uint iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	if (m_eType == TYPE::TYPE_NONANIM)
		iFlag |= aiProcess_PreTransformVertices;

	m_pAiScene = m_Importer.ReadFile(strModelFilePath, iFlag);
	// aiProcess_PreTransformVertices 애니메이션이 있는 매쉬를 로드하면 애니메이션과 관련된 매쉬가 없어질 수 있다.
	if (m_pAiScene == nullptr)
		return E_FAIL;

	XMStoreFloat4x4(&m_matPivot, matPivot);

	if (FAILED(Ready_Bones(m_pAiScene->mRootNode, -1)))
		return E_FAIL;

	if (FAILED(Ready_Meshes(matPivot)))
		return E_FAIL;

	if (FAILED(Ready_Materials(strModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	if (iMeshIndex >= m_iMeshesNum)
		return E_FAIL;

	m_vecMesh[iMeshIndex]->Bind_Buffer();
	m_vecMesh[iMeshIndex]->Render();

	return S_OK;
}

void CModel::Play_Animation(_float fTimeDelta, _bool bLoop)
{
	if (m_iCurrentAnimationIndex >= m_iAnimationNum)
		return;

	m_vecAnimation[m_iCurrentAnimationIndex]->Invalidate_TransformationMatrix(fTimeDelta, bLoop,m_vecBones);

	for (auto& iter : m_vecBones)
	{
		iter->Invalidate_MatCombined(m_vecBones, XMLoadFloat4x4(&m_matPivot));
	}
}

_bool CModel::Compute_MousePos(_float3* pOut, _matrix matWorld)
{
	if (m_vecMesh.empty())
		return false;

	for (auto& iter : m_vecMesh)
	{
		if (iter->Compute_MousePos(pOut, matWorld))
			return true;
	}

	return false;
}

HRESULT CModel::Bind_ShaderResources(CShader* pShader, const _char* pName, _uint iMeshIndex, aiTextureType eType)
{
	if (pShader==nullptr || iMeshIndex >= m_iMeshesNum)
		return E_FAIL;

	_uint	iMaterialIndex = m_vecMesh[iMeshIndex]->Get_MaterialIndex();

	return m_vecMaterial[iMaterialIndex].pMtrlTexture[eType]->Bind_ShaderResource(pShader, pName);
}

HRESULT CModel::Bind_Blend(CShader* pShader, const _char* pName, _uint iMeshIndex)
{
	return m_vecMesh[iMeshIndex]->Bind_Blend(pShader, pName,m_vecBones);
}

HRESULT CModel::Ready_Meshes(_fmatrix	matPivot)
{
	m_iMeshesNum = m_pAiScene->mNumMeshes;

	m_vecMesh.reserve(m_iMeshesNum);

	for (size_t i = 0; i < m_iMeshesNum; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, m_pAiScene->mMeshes[i], matPivot,m_vecBones);

		if (pMesh == nullptr)
			return E_FAIL;

		m_vecMesh.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const string& strModelFilePath)
{
	m_iMaterialsNum = m_pAiScene->mNumMaterials;

	for (size_t i = 0; i < m_iMaterialsNum; i++)
	{
		aiMaterial* pMaterial = m_pAiScene->mMaterials[i];

		MATERIAL_DESC	Material_Desc = {};

		for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			
			_char szDdrive[MAX_PATH] = "";
			_char szDirectory[MAX_PATH] = "";
			
			_splitpath_s(strModelFilePath.c_str(), szDdrive, MAX_PATH, szDirectory, MAX_PATH,
				nullptr, 0, nullptr, 0);

			aiString	szGetPath;
			if (FAILED(pMaterial->GetTexture((aiTextureType)j, 0, &szGetPath)))
				continue;

			_char	szFileName[MAX_PATH] = "";
			_char	szExc[MAX_PATH] = "";

			_splitpath_s(szGetPath.data, nullptr, 0, nullptr, 0,
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
				return E_FAIL;
		}

		m_vecMaterial.push_back(Material_Desc);
	}

	return S_OK;
}

HRESULT CModel::Ready_Bones(aiNode* pNode, _int iParentIndex)
{
	CBone* pBone = CBone::Create(pNode, iParentIndex);
	if (pBone == nullptr)
		return E_FAIL;

	m_vecBones.push_back(pBone);

	_int iParIndx = m_vecBones.size() - 1;

	for (_uint i = 0; i < pNode->mNumChildren; i++)
	{
		Ready_Bones(pNode->mChildren[i], iParIndx);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animation()
{
	m_iAnimationNum = m_pAiScene->mNumAnimations;

	for (_uint i = 0; i < m_iAnimationNum; i++)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAiScene->mAnimations[i],m_vecBones);
		if (pAnimation == nullptr)
			return E_FAIL;

		m_vecAnimation.push_back(pAnimation);
	}

	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const string& strModelFilePath, _fmatrix	matPivot)
{
	CModel* pInstance = new CModel(pDevice, pContext);

 	if (FAILED(pInstance->Initialize_ProtoType(eType,strModelFilePath, matPivot))) {
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

	for (auto& iter1 : m_vecMaterial)
	{
		for (auto& iter2 : iter1.pMtrlTexture)
			Safe_Release(iter2);
	}

	for (auto* iter : m_vecBones)
		Safe_Release(iter);
	m_vecBones.clear();

	for (auto* iter : m_vecMesh)
		Safe_Release(iter);
	m_vecMesh.clear();

	for (auto* iter : m_vecBones)
		Safe_Release(iter);
	m_vecBones.clear();

	for (auto& iter : m_vecAnimation)
		Safe_Release(iter);
	m_vecAnimation.clear();

	if(!m_bClone)
		m_Importer.FreeScene();

}
