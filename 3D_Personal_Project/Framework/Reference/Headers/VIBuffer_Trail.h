#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CTransform;

class ENGINE_DLL CVIBuffer_Trail final : public CVIBuffer
{
public:
	typedef struct tagTrail_Desc
	{
		_float3		vPos_0;
		_float3		vPos_1;
		_uint		iMaxCnt;
		_uint		iLerpPointNum;

	}VIBUFFER_TRAIL_DESC;

private:
	CVIBuffer_Trail(ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
	CVIBuffer_Trail(const CVIBuffer_Trail& rhs);
	virtual	~CVIBuffer_Trail() = default;
public:
	virtual	HRESULT	Initialize_ProtoType() override;
	virtual	HRESULT	Initialize(void* pArg) override;

	void	Update(_fmatrix matOwner);
	void	Reset(_fmatrix matOwner);

private:
	VIBUFFER_TRAIL_DESC m_Trail_Desc = {};
	_uint				m_iVtxCount = { 0 };

	//_uint m_iLerpPointNum = { 100 };
	_int m_iCatMullRomIndex[4] = {};

public:
	static	CVIBuffer_Trail* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent*		Clone(void* pArg) override;
	virtual	void			Free() override;
};

END
