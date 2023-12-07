#include "..\Public\Channel.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(aiNodeAnim* paiChannel)
{
	strcpy_s(m_szName, paiChannel->mNodeName.data);
	
	m_iKeyFrameNum = max(paiChannel->mNumScalingKeys, paiChannel->mNumRotationKeys);
	m_iKeyFrameNum = max(paiChannel->mNumPositionKeys, m_iKeyFrameNum);

	_float3 vPos;
	_float3 vScale;
	_float4 vRot;

	for (_uint i = 0; i < m_iKeyFrameNum; i++)
	{
		KEYFRAME	KeyFrame = {};

		if (i < paiChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &paiChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTrackPosition = paiChannel->mScalingKeys[i].mTime;
		}
		if (i < paiChannel->mNumRotationKeys)
		{
			vRot.x = paiChannel->mRotationKeys[i].mValue.x;
			vRot.y = paiChannel->mRotationKeys[i].mValue.y;
			vRot.z = paiChannel->mRotationKeys[i].mValue.z;
			vRot.w = paiChannel->mRotationKeys[i].mValue.w;
			KeyFrame.fTrackPosition = paiChannel->mRotationKeys[i].mTime;
		}
		if (i < paiChannel->mNumPositionKeys)
		{
			memcpy(&vPos, &paiChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTrackPosition = paiChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vPosition = vPos;
		KeyFrame.vRotation = vRot;
		KeyFrame.vScale = vScale;

		m_vecKeyFrame.push_back(KeyFrame);
	}

	return S_OK;
}

void CChannel::Invalidate_TransformationMatrix(_float fCurrentTrackPosition)
{
	_vector	vScale, vRot, vPos;

	KEYFRAME	LastKeyFrame = m_vecKeyFrame.back();

	if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRot = XMLoadFloat4(&LastKeyFrame.vRotation);
		vPos = XMLoadFloat3(&LastKeyFrame.vPosition);
	}
	else {

		if (fCurrentTrackPosition >= m_vecKeyFrame[m_iCurrentKeyFrameIndex + 1].fTrackPosition)
			++m_iCurrentKeyFrameIndex;

		_float3 vSourScale, vDestScale;
		_float4 vSourRot, vDestRot;
		_float3 vSourPos, vDestPos;

		vSourScale = m_vecKeyFrame[m_iCurrentKeyFrameIndex].vScale;
		vSourRot = m_vecKeyFrame[m_iCurrentKeyFrameIndex].vRotation;
		vSourPos = m_vecKeyFrame[m_iCurrentKeyFrameIndex].vPosition;

		vDestScale = m_vecKeyFrame[m_iCurrentKeyFrameIndex+1].vScale;
		vDestRot = m_vecKeyFrame[m_iCurrentKeyFrameIndex+1].vRotation;
		vDestPos = m_vecKeyFrame[m_iCurrentKeyFrameIndex+1].vPosition;

		_float fRatio = (fCurrentTrackPosition - m_vecKeyFrame[m_iCurrentKeyFrameIndex].fTrackPosition)/
			(m_vecKeyFrame[m_iCurrentKeyFrameIndex + 1].fTrackPosition - m_vecKeyFrame[m_iCurrentKeyFrameIndex].fTrackPosition);

		vScale = XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), fRatio);
		vRot = XMQuaternionSlerp(XMLoadFloat4(&vSourRot), XMLoadFloat4(&vDestRot), fRatio);
		vPos = XMVectorLerp(XMLoadFloat3(&vSourPos), XMLoadFloat3(&vDestPos), fRatio);
	}

	_matrix matTransformation = XMMatrixAffineTransformation(vScale,
		XMVectorSet(0.f, 0.f, 0.f, 1.f), vRot, vPos);
}

CChannel* CChannel::Create(aiNodeAnim* paiChannel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(paiChannel))) {
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	__super::Free();
}
