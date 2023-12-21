#include "..\Public\Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(BONE Bone)
{
	m_iParentIndex = Bone.iParentIndex;

	strcpy_s(m_szName, Bone.szName.c_str());

	m_matOffset = Bone.matOffsetMatrix;

	m_matTransformation = Bone.matTransformation;

	XMStoreFloat4x4(&m_matCombinedTransformation, XMMatrixIdentity());

	return  S_OK;
}

wstring CBone::Get_BoneName_wstr()
{
	_tchar szFullName[MAX_PATH] = TEXT("");
	const _char* szTmp = m_szName;
	MultiByteToWideChar(CP_ACP, 0, szTmp, strlen(szTmp), szFullName, MAX_PATH);

	return szFullName;
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

CBone* CBone::Create(BONE Bone)
{
	CBone* pInstance = new CBone();

	if (FAILED(pInstance->Initialize(Bone))) {
		MSG_BOX("Failed to Created : CBone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::Clone()
{
	return new CBone(*this);
}

void CBone::Free()
{
	__super::Free();
}
