#include "..\Public\Channel.h"
#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(CHANNEL Channel, const CModel::BONES& vecBones)
{
	strcpy_s(m_szName, Channel.szName.c_str());

	m_iKeyFrameNum = Channel.vecKeyFrame.size();

	m_vecKeyFrame = Channel.vecKeyFrame;

	_uint iBoneIndex = { 0 };

	auto iter = find_if(vecBones.begin(), vecBones.end(), [&](CBone* pBone) {
		
		if (!strcmp(m_szName, pBone->Get_BoneName()))
			return true;
	
		++iBoneIndex;

		return false;
	});

	if (iter == vecBones.end())
		return E_FAIL;

	m_iBoneIndex = iBoneIndex;

	return S_OK;
}

void CChannel::Invalidate_TransformationMatrix(_float fCurrentTrackPosition, const CModel::BONES& vecBones, _uint* iCurrentKeyFrameIndex)
{
	if (fCurrentTrackPosition == 0.f)
		*iCurrentKeyFrameIndex = 0;

	_vector	vScale, vRot, vPos;

	KEYFRAME	LastKeyFrame = m_vecKeyFrame.back();

	if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRot = XMLoadFloat4(&LastKeyFrame.vRotation);
		vPos = XMLoadFloat3(&LastKeyFrame.vPosition);
	}
	else {

		while(fCurrentTrackPosition >= m_vecKeyFrame[*iCurrentKeyFrameIndex + 1].fTrackPosition)
			++* iCurrentKeyFrameIndex;

		_float3 vSourScale, vDestScale;
		_float4 vSourRot, vDestRot;
		_float3 vSourPos, vDestPos;

		vSourScale = m_vecKeyFrame[*iCurrentKeyFrameIndex].vScale;
		vSourRot = m_vecKeyFrame[*iCurrentKeyFrameIndex].vRotation;
		vSourPos = m_vecKeyFrame[*iCurrentKeyFrameIndex].vPosition;

		vDestScale = m_vecKeyFrame[*iCurrentKeyFrameIndex +1].vScale;
		vDestRot = m_vecKeyFrame[*iCurrentKeyFrameIndex +1].vRotation;
		vDestPos = m_vecKeyFrame[*iCurrentKeyFrameIndex +1].vPosition;

		_float fRatio = (fCurrentTrackPosition - m_vecKeyFrame[*iCurrentKeyFrameIndex].fTrackPosition)/
			(m_vecKeyFrame[*iCurrentKeyFrameIndex + 1].fTrackPosition - m_vecKeyFrame[*iCurrentKeyFrameIndex].fTrackPosition);

		vScale = XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), fRatio);
		vRot = XMQuaternionSlerp(XMLoadFloat4(&vSourRot), XMLoadFloat4(&vDestRot), fRatio);
		vPos = XMVectorLerp(XMLoadFloat3(&vSourPos), XMLoadFloat3(&vDestPos), fRatio);
	}

	_matrix matTransformation = XMMatrixAffineTransformation(vScale,
		XMVectorSet(0.f, 0.f, 0.f, 1.f), vRot, vPos);

	vecBones[m_iBoneIndex]->Set_TransformationMatrix(matTransformation);
}

CChannel* CChannel::Create(CHANNEL Channel, const CModel::BONES& vecBones)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(Channel, vecBones))) {
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	__super::Free();
}
