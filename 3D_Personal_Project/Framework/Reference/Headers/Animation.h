#pragma once
#include "Model.h"

BEGIN(Engine)

class CChannel;

class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual	~CAnimation() = default;

public:
	HRESULT	Initialize(aiAnimation* paiAnimation, const CModel::BONES& vecBones);
	void	Invalidate_TransformationMatrix(_float fTimeDelta, _bool bLoop,const CModel::BONES& vecBones);

private:
	_char				m_szName[MAX_PATH] = "";
	_float				m_fDuration = { 0.f };
	_float				m_fTicksPerSecond = { 0.f };
	_float				m_fTrackPosition = { 0.f };

	_uint				m_iChannelNum = {0};
	vector<CChannel*>	m_vecChannel;

	_bool				m_bFinished = { false };

public:
	static	CAnimation* Create(aiAnimation* paiAnimation, const CModel::BONES& vecBones);
	virtual	void	Free() override;
};

END