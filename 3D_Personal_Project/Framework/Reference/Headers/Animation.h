#pragma once
#include "Model.h"

BEGIN(Engine)

class CChannel;

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual	~CAnimation() = default;

public:
	HRESULT	Initialize(ANIMATION Animation, const CModel::BONES& vecBones);
	void	Invalidate_TransformationMatrix(_float fTimeDelta, _bool bLoop,const CModel::BONES& vecBones);
	_bool	Invalidate_Interval_TransformationMatrix(_float fTimeDelta,_float fIntervalDuration, const CModel::BONES& vecBones, vector<CChannel*>& vecChannel);

public:
	vector<CChannel*>&	Get_Channels() { return m_vecChannel; }

private:
	_char				m_szName[MAX_PATH] = "";
	_float				m_fDuration = { 0.f };
	_float				m_fTicksPerSecond = { 0.f };
	_float				m_fTrackPosition = { 0.f };

	_uint				m_iChannelNum = {0};
	vector<CChannel*>	m_vecChannel;
	vector<_uint>		m_vecCurrentKeyFrameIndex;

	_bool				m_bFinished = { false };

private:
	_float				m_fInterverTime = { 0.f };

public:
	static	CAnimation* Create(ANIMATION Animation, const CModel::BONES& vecBones);
	CAnimation* Clone();
	virtual	void	Free() override;
};

END