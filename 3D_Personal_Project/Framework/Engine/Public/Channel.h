#pragma once
#include "Base.h"

BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual	~CChannel() = default;

public:
	HRESULT	Initialize(aiNodeAnim* paiChannel);
	void	Invalidate_TransformationMatrix(_float fCurrentTrackPosition);

private:
	_char				m_szName[MAX_PATH] = "";
	_uint				m_iKeyFrameNum = { 0 };
	_uint				m_iCurrentKeyFrameIndex = { 0 };

	vector<KEYFRAME>	m_vecKeyFrame;

public:
	static CChannel* Create(aiNodeAnim* paiChannel);
	virtual	void	Free() override;
};

END