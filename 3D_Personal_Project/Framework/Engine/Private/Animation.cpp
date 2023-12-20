#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& rhs)
	:m_fDuration(rhs.m_fDuration),m_fTicksPerSecond(rhs.m_fTicksPerSecond),m_fTrackPosition(rhs.m_fTrackPosition),
	m_iChannelNum(rhs.m_iChannelNum),m_vecChannel(rhs.m_vecChannel),m_vecCurrentKeyFrameIndex(rhs.m_vecCurrentKeyFrameIndex),
	m_bFinished(rhs.m_bFinished),m_vecPrevKeyFrame(rhs.m_vecPrevKeyFrame)
{
	strcpy_s(m_szName, rhs.m_szName);
	
	for (auto& iter : m_vecChannel)
		Safe_AddRef(iter);

}

HRESULT CAnimation::Initialize(ANIMATION Animation, const CModel::BONES& vecBones)
{
	strcpy_s(m_szName, Animation.szName.c_str());

	m_fDuration = Animation.fDuration;
	m_fTicksPerSecond = Animation.fTicksPerSecond;
	
	m_iChannelNum = Animation.vecChannel.size();

	m_vecCurrentKeyFrameIndex.resize(m_iChannelNum);

	m_vecPrevKeyFrame.resize(m_iChannelNum);

	for (_uint i = 0; i < m_iChannelNum; i++)
	{
		CChannel* pChannel = CChannel::Create(Animation.vecChannel[i], vecBones);
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
		m_vecChannel[i]->Invalidate_TransformationMatrix(m_fTrackPosition, vecBones,&m_vecCurrentKeyFrameIndex[i], &m_vecPrevKeyFrame[i]);
	}
}

_bool CAnimation::Invalidate_Interval_TransformationMatrix(_float fTimeDelta, _float fIntervalDuration, const CModel::BONES& vecBones, vector<KEYFRAME>& vecPrevKeyFrame)
{
	if (m_fInterverTime == 0.f)
		Reset_Animation();

	m_fInterverTime += fTimeDelta;

	if (m_fInterverTime >= fIntervalDuration) {
		m_fInterverTime = 0.f;
		return true;
	}

	_uint iPrevSize = vecPrevKeyFrame.size();

	for (_uint i = 0; i < m_iChannelNum; i++)
	{
		if (iPrevSize <= i)
			continue;

		m_vecChannel[i]->Invalidate_Interval_TransformationMatrix(m_fInterverTime, fIntervalDuration, vecBones, vecPrevKeyFrame[i], &m_vecCurrentKeyFrameIndex[i]);
	}

	return false;
}

void CAnimation::Reset_Animation()
{
	m_fTrackPosition = 0.f;
	for (auto& iter : m_vecCurrentKeyFrameIndex)
	{
		iter = 0;
	}
	m_bFinished = false;
}

wstring CAnimation::Get_Name()
{
	_tchar szFullName[MAX_PATH] = TEXT("");
	const _char* szTmp = m_szName;
	MultiByteToWideChar(CP_ACP, 0, szTmp, strlen(szTmp), szFullName, MAX_PATH);

	return szFullName;
}

CAnimation* CAnimation::Create(ANIMATION Animation, const CModel::BONES& vecBones)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(Animation, vecBones))) {
		MSG_BOX("Failed to Created : CAnimation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

void CAnimation::Free()
{
	__super::Free();

	for (auto& iter : m_vecChannel)
		Safe_Release(iter);
	m_vecChannel.clear();
}
