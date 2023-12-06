#include "..\Public\Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(aiNode* pNode, _int iParentIndex)
{
	m_iParentIndex = iParentIndex;

	strcpy_s(m_szName, pNode->mName.data);

	memcpy(&m_matTransformation, &pNode->mTransformation,sizeof(_float4x4));
	XMStoreFloat4x4(&m_matTransformation,XMMatrixTranspose(XMLoadFloat4x4(&m_matTransformation)));

	XMStoreFloat4x4(&m_matCombinedTransformation, XMMatrixIdentity());

	return  S_OK;
}

void CBone::Invalidate_MatCombined(class CModel::BONES& pBones, _fmatrix matPivot)
{
	if (-1 == m_iParentIndex)
		XMStoreFloat4x4(&m_matCombinedTransformation, XMLoadFloat4x4(&m_matTransformation) * matPivot);
	else
	{
		XMStoreFloat4x4(&m_matCombinedTransformation,
			XMLoadFloat4x4(&m_matTransformation) * XMLoadFloat4x4(&pBones[m_iParentIndex]->m_matCombinedTransformation));
	}
}

CBone* CBone::Create(aiNode* pNode, _int iParentIndex)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pNode, iParentIndex))) {
		MSG_BOX("Failed to Created : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBone::Free()
{
	__super::Free();
}
