#pragma once
#include "Base.h"

BEGIN(Engine)

class CChannel;

class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual	~CAnimation() = default;

public:
	HRESULT	Initialize(aiAnimation* paiAnimation);
	void	Invalidate_TransformationMatrix(_float fTimeDelta);

private:
	_char				m_szName[MAX_PATH] = "";
	_float				m_fDuration = { 0.f };
	_float				m_fTicksPerSecond = { 0.f };
	_float				m_fTrackPosition = { 0.f };

	_uint				m_iChannelNum = {0};
	vector<CChannel*>	m_vecChannel;


public:
	static	CAnimation* Create(aiAnimation* paiAnimation);
	virtual	void	Free() override;
};

END