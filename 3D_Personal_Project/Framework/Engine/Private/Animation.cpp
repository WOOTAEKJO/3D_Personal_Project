#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(aiAnimation* paiAnimation)
{
	strcpy_s(m_szName, paiAnimation->mName.data);

	m_fDuration = paiAnimation->mDuration;
	m_fTicksPerSecond = paiAnimation->mTicksPerSecond;
	
	m_iChannelNum = paiAnimation->mNumChannels;

	for (_uint i = 0; i < m_iChannelNum; i++)
	{
		CChannel* pChannel = CChannel::Create(paiAnimation->mChannels[i]);
		if (pChannel == nullptr)
			return E_FAIL;

		m_vecChannel.push_back(pChannel);
	}

	return S_OK;
}

void CAnimation::Invalidate_TransformationMatrix(_float fTimeDelta)
{
	m_fTrackPosition += m_fTicksPerSecond * fTimeDelta;

	for (_uint i = 0; i < m_iChannelNum; i++)
	{
		m_vecChannel[i]->Invalidate_TransformationMatrix(m_fTrackPosition);
	}
}

CAnimation* CAnimation::Create(aiAnimation* paiAnimation)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(paiAnimation))) {
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	__super::Free();
}
