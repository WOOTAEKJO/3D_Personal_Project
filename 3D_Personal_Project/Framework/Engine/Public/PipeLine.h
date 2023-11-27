#pragma once
#include "Base.h"

BEGIN(Engine)

class CPipeLine final : public CBase
{
public:
	enum TRANSFORMSTATE {VIEW,PROJ,STATE_END};
private:
	CPipeLine();
	virtual	~CPipeLine() = default;

public:
	HRESULT	Initialize();
	void	Tick();

public:
	void	Set_Transform(TRANSFORMSTATE eState, _float4x4 matMatrix);
	void	Set_Transform(TRANSFORMSTATE eState, _fmatrix matMatrix);
	_float4x4 Get_Transform_Float4x4(TRANSFORMSTATE eState);
	_matrix	Get_Transform_Matrix(TRANSFORMSTATE eState);
	_float4x4	Get_Transform_Float4x4_Inverse(TRANSFORMSTATE eState);
	_matrix		Get_Transform_Matrix_Inverse(TRANSFORMSTATE eState);
	_float4		Get_Camera_Pos();
		
private:
	_float4x4	m_matTransform[TRANSFORMSTATE::STATE_END];
	_float4x4	m_matTransformInverse[TRANSFORMSTATE::STATE_END];
	_float4		m_vCameraPos;

public:
	static	CPipeLine* Create();
	virtual	void	Free();
};

END