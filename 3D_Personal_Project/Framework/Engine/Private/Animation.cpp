#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(aiAnimation* paiAnimation, const CModel::BONES& vecBones)
{
	strcpy_s(m_szName, paiAnimation->mName.data);

	m_fDuration = paiAnimation->mDuration;
	m_fTicksPerSecond = paiAnimation->mTicksPerSecond;
	
	m_iChannelNum = paiAnimation->mNumChannels;

	for (_uint i = 0; i < m_iChannelNum; i++)
	{
		CChannel* pChannel = CChannel::Create(paiAnimation->mChannels[i], vecBones);
		if (pChannel == nullptr)
			return E_FAIL;

		m_vecChannel.push_back(pChannel);
	}

	return S_OK;
}

void CAnimation::Invalidate_TransformationMatrix(_float fTimeDelta, _bool bLoop, const CModel::BONES& vecBones)
{
	m_fTrackPosition += m_fTicksPerSecond * fTimeDelta;

	if (m_fTrackPosition >= m_fDuration)
	{
		m_bFinished = true;
		m_fTrackPosition = m_fDuration;

		if (bLoop)
		{
			m_bFinished = false;
			m_fTrackPosition = 0.f;
		}
	}

	for (_uint i = 0; i < m_iChannelNum; i++)
	{
		m_vecChannel[i]->Invalidate_TransformationMatrix(m_fTrackPosition, vecBones);
	}
}

CAnimation* CAnimation::Create(aiAnimation* paiAnimation, const CModel::BONES& vecBones)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(paiAnimation, vecBones))) {
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	__super::Free();

	for (auto& iter : m_vecChannel)
		Safe_Release(iter);
	m_vecChannel.clear();
}
