#pragma once
#include "Base.h"

BEGIN(Engine)

class CBone final : public	CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	HRESULT	Initialize(aiNode* pNode, _int iParentIndex);

private:
	_char	m_szName[MAX_PATH] = "";
	_int	m_iParentIndex = { 0 };
	
	_float4x4	m_matTransformation;		// ���� ���� ����� ����
	_float4x4	m_matCombindTransformation;	// ���� ���� ��� * �θ��� m_matCombindTransformation

public:
	static	CBone* Create(aiNode* pNode, _int iParentIndex);
	virtual	void	Free() override;
};

END