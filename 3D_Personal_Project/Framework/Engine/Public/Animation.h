#pragma once
#include "Model.h"

BEGIN(Engine)

class CChannel;

class ENGINE_DLL CAnimation final : public CBase
{
public:
	enum SPEED_TYPE {TYPE_EXTRASPEED,TYPE_INTERVER_EXTRASPEED,TYPE_END};

private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual	~CAnimation() = default;

public:
	HRESULT	Initialize(ANIMATION Animation, const CModel::BONES& vecBones);
	void	Invalidate_TransformationMatrix(_float fTimeDelta, _bool bLoop,const CModel::BONES& vecBones);
	_bool	Invalidate_Interval_TransformationMatrix(_float fTimeDelta,_float fIntervalDuration, const CModel::BONES& vecBones, vector<KEYFRAME>& vecPrevKeyFrame);

	void	Reset_Animation();
public:
	vector<CChannel*>&	Get_Channels() { return m_vecChannel; }
	vector<KEYFRAME>& Get_PrevKeyFrame() { return m_vecPrevKeyFrame; }

	_bool		Is_Finished() { return m_bFinished; }
	void		Set_ReStart();
	void		Reset_InterverTime() { m_fInterverTime = 0.f; }
	wstring		Get_Name();

	_float*		Get_ExtraSpeed() { return m_fExtraSpeed; }
	//_float*		Get_InterverExtraSpeed() { return &m_fInterverExtraSpeed; }

	_bool		Is_Arrival_TrackPosition(_float fTime);
	_float		Get_Duration() { return m_fDuration; }

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

private:
	_char				m_szName[MAX_PATH] = "";
	_float				m_fDuration = { 0.f };
	_float				m_fTicksPerSecond = { 0.f };
	_float				m_fTrackPosition = { 0.f };

	_uint				m_iChannelNum = {0};
	vector<CChannel*>	m_vecChannel;
	vector<_uint>		m_vecCurrentKeyFrameIndex;

	_bool				m_bFinished = { false };

	vector<KEYFRAME>	m_vecPrevKeyFrame;

private:
	_float				m_fInterverTime = { 0.f };
	//_float				m_fInterverExtraSpeed = { 1.f };

private:
	_float				m_fExtraSpeed[TYPE_END] = { 1.f,1.f };

public:
	static	CAnimation* Create(ANIMATION Animation, const CModel::BONES& vecBones);
	CAnimation* Clone();
	virtual	void	Free() override;
};

END