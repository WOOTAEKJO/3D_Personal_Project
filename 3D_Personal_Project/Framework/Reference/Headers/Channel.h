#pragma once
#include "Model.h"

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual	~CChannel() = default;

public:
	HRESULT	Initialize(CHANNEL Channel, const CModel::BONES& vecBones);
	void	Invalidate_TransformationMatrix(_float fCurrentTrackPosition, const CModel::BONES& vecBones,_uint* iCurrentKeyFrameIndex);
	void	Invalidate_Interval_TransformationMatrix(_float fCurrentTrackPosition, _float fIntervalDuration, const CModel::BONES& vecBones, KEYFRAME PrevKeyFrame, _uint* iCurrentKeyFrameIndex);

public:
	KEYFRAME	Get_PrevKeyFram() { return m_PrevKeyFrame; }

private:
	_char				m_szName[MAX_PATH] = "";
	_uint				m_iKeyFrameNum = { 0 };

	_uint				m_iBoneIndex = { 0 };

	vector<KEYFRAME>	m_vecKeyFrame;

	KEYFRAME			m_PrevKeyFrame;

public:
	static CChannel* Create(CHANNEL Channel, const CModel::BONES& vecBones);
	virtual	void	Free() override;
};

END