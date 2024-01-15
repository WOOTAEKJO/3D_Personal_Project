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
	m_matPivot(rhs.m_matPivot),
	m_iAnimationNum(rhs.m_iAnimationNum),
	m_iCurrentAnimationIndex(rhs.m_iCurrentAnimationIndex)
{
	for (auto& iter1 : m_vecMaterial)
	{
		for (auto& iter2 : iter1.pMtrlTexture)
			Safe_AddRef(iter2);
	}

	for (auto& iter : m_vecMesh)
		Safe_AddRef(iter);

	for (auto& iter : rhs.m_vecBones)
		m_vecBones.push_back(iter->Clone());

	for (auto& iter : rhs.m_vecAnimation)
		m_vecAnimation.push_back(iter->Clone());
}

HRESULT CModel::Initialize_ProtoType(TYPE eType, const string& strModelFilePath, _fmatrix	matPivot)
{
	m_eType = eType;

	XMStoreFloat4x4(&m_matPivot, matPivot);

	CMeshData::MESHDATADESC MeshData = {};

	CMeshData* pMeshData = CMeshData::Create(MeshData);

	if (FAILED(pMeshData->Load_Data(strModelFilePath.c_str())))
		return E_FAIL;

	if (FAILED(pMeshData->Data_Get(MeshData)))
		return E_FAIL;

	if (FAILED(Ready_Bones(MeshData)))
		return E_FAIL;

	if (FAILED(Ready_Meshes(MeshData)))
		return E_FAIL;

	if (FAILED(Ready_Materials(MeshData, strModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animation(MeshData)))
		return E_FAIL;

	Safe_Release(pMeshData);

	m_vecAnimation;
	m_vecBones;
	m_vecMaterial;
	m_vecMesh;

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

	if(m_bChnageAnim){
		
		//vector<CChannel*> vecCurChannel = m_vecAnimation[m_iCurrentAnimationIndex]->Get_Channels();
		vector<KEYFRAME> vecPrevKeyFrame = m_vecAnimation[m_iCurrentAnimationIndex]->Get_PrevKeyFrame();

		if (m_vecAnimation[m_iNextAnimationIndex]->Invalidate_Interval_TransformationMatrix(fTimeDelta,0.2f, m_vecBones, vecPrevKeyFrame))
		{
			m_bChnageAnim = false;
			//m_vecAnimation[m_iCurrentAnimationIndex]->Reset_InterverTime();
			m_iCurrentAnimationIndex = m_iNextAnimationIndex;
		}

	}
	else {
		m_vecAnimation[m_iCurrentAnimationIndex]->Invalidate_TransformationMatrix(fTimeDelta, bLoop, m_vecBones);
	}

	for (auto& iter : m_vecBones)
	{
		iter->Invalidate_MatCombined(m_vecBones, XMLoadFloat4x4(&m_matPivot));
	}
}

CBone* CModel::Get_Bone(const _char* strBoneName)
{
	auto iter = find_if(m_vecBones.begin(), m_vecBones.end(), [&](CBone* pBone) {
		
		if (!strcmp(pBone->Get_BoneName(), strBoneName))
			return true;

		return false;
	});

	if (iter == m_vecBones.end())
		return nullptr;

	return *iter;
}

_float* CModel::Get_AnimExtraSpeed()
{
	if (m_vecAnimation[m_iCurrentAnimationIndex] == nullptr)
		return nullptr;

	return m_vecAnimation[m_iCurrentAnimationIndex]->Get_ExtraSpeed();
}

_bool CModel::Compute_MousePos(_float3* pOut, _matrix matWorld)
{
	if (m_vecMesh.empty())
		return false;

	_float fDist = 0.f;

	for (auto& iter : m_vecMesh)
	{
		if (iter->Compute_MousePos(pOut, matWorld, &fDist))
			return true;
	}

	return false;
}

_bool CModel::Compute_MousePos_Dist(_float3* pOut, _matrix matWorld, _float* fDist)
{
	if (m_vecMesh.empty())
		return false;

	_float fMinDist = 10000.f;
	//_float fDist = 0.f;
	_float3 vPos = {};
	_bool bCheck = false;

	for (auto& iter : m_vecMesh)
	{
		if (iter->Compute_MousePos(&vPos, matWorld,fDist))
		{
			if (*fDist < fMinDist)
			{
				bCheck = true;
				fMinDist = *fDist;
				*pOut = vPos;
			}
		}
	}

	return bCheck;
}

HRESULT CModel::Bind_ShaderResources(CShader* pShader, const _char* pName, _uint iMeshIndex, TEXTURETYPE eType)
{
	if (pShader==nullptr || iMeshIndex >= m_iMeshesNum)
		return E_FAIL;

	_uint	iMaterialIndex = m_vecMesh[iMeshIndex]->Get_MaterialIndex();

	if (m_vecMaterial[iMaterialIndex].pMtrlTexture[eType] == nullptr)
		return S_OK;

	return m_vecMaterial[iMaterialIndex].pMtrlTexture[eType]->Bind_ShaderResource(pShader, pName);
}

HRESULT CModel::Bind_Blend(CShader* pShader, const _char* pName, _uint iMeshIndex, _int iNonBindInd)
{
	return m_vecMesh[iMeshIndex]->Bind_Blend(pShader, pName,m_vecBones, iNonBindInd);
}

_bool CModel::Is_Animation_Finished()
{
	_bool bCheck = m_vecAnimation[m_iCurrentAnimationIndex]->Is_Finished();

	if (bCheck)
	{
		m_vecAnimation[m_iCurrentAnimationIndex]->Set_ReStart();
		//m_vecAnimation[m_iCurrentAnimationIndex]->Reset_Animation();

		return true;
	}

	return false;
}

_bool CModel::Is_CurAnim_Arrival_TrackPosition(_float fTime)
{
	return m_vecAnimation[m_iCurrentAnimationIndex]->Is_Arrival_TrackPosition(fTime);
}

_float CModel::CurAnim_Get_Duration()
{
	return m_vecAnimation[m_iCurrentAnimationIndex]->Get_Duration();
}

void CModel::Write_Json(json& Out_Json)
{
	_uint iSize = m_vecAnimation.size();

	for (_uint i = 0; i < iSize; i++)
	{
		m_vecAnimation[i]->Write_Json(Out_Json["Animation"][i]);
	}
}

void CModel::Load_FromJson(const json& In_Json)
{
	_uint iSize = m_vecAnimation.size();
	for (_uint i = 0; i < iSize; i++)
	{
		m_vecAnimation[i]->Load_FromJson(In_Json["Animation"][i]);
	}
}

HRESULT CModel::Ready_Meshes(CMeshData::MESHDATADESC MeshData)
{	
	m_iMeshesNum = MeshData.iMeshNum;

	m_vecMesh.reserve(m_iMeshesNum);

	for (_uint i = 0; i < m_iMeshesNum; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, MeshData.vecMesh[i], XMLoadFloat4x4(&m_matPivot), m_vecBones);
		if (pMesh == nullptr)
			return E_FAIL;
		
		m_vecMesh.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(CMeshData::MESHDATADESC MeshData, const string& strModelFilePath)
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

HRESULT CModel::Ready_Bones(CMeshData::MESHDATADESC MeshData)
{
	_uint iBoneNum = MeshData.iAnimBoneNum;

	for (_uint i = 0; i < iBoneNum; i++)
	{
		CBone* pBone = CBone::Create(MeshData.vecAnimBone[i]);
		if (pBone == nullptr)
			return E_FAIL;

		m_vecBones.push_back(pBone);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animation(CMeshData::MESHDATADESC MeshData)
{
	m_iAnimationNum = MeshData.iAnimAnimationNum;

	for (_uint i = 0; i < m_iAnimationNum; i++)
	{
		CAnimation* pAnimation = CAnimation::Create(MeshData.vecAnimAnimation[i], m_vecBones);
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

}
