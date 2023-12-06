#pragma once
#include "Model.h"

BEGIN(Engine)

class CBone final : public	CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	HRESULT	Initialize(aiNode* pNode, _int iParentIndex);

public:
	const _char*	Get_BoneName() const { return m_szName; }
	_matrix	Get_CombinedTransformationMatrix() const { 
		return XMLoadFloat4x4(&m_matCombinedTransformation); }

	void	Invalidate_MatCombined(CModel::BONES& pBones, _fmatrix matPivot);

private:
	_char	m_szName[MAX_PATH] = "";
	_int	m_iParentIndex = { 0 };
	
	_float4x4	m_matTransformation;			// 뼈의 상태 행렬을 저장
	_float4x4	m_matCombinedTransformation;	// 뼈의 상태 행렬 * 부모의 m_matCombindTransformation

public:
	static	CBone* Create(aiNode* pNode, _int iParentIndex);
	virtual	void	Free() override;
};

END