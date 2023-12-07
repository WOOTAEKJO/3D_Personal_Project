#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CMeshData;

class ENGINE_DLL CVIBuffer_DTerrain final : public	CVIBuffer
{
public:
	typedef	struct tagVIBuffer_DTerrain_Desc
	{
		_uint iVerticesXNum = 0;
		_uint iVerticesZNum = 0;

	}DTERRAINDESC;
private:
	CVIBuffer_DTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_DTerrain(const CVIBuffer_DTerrain& rhs);
	virtual	~CVIBuffer_DTerrain() = default;

public:
	virtual	HRESULT	Initialize_ProtoType() override;
	virtual	HRESULT	Initialize(void* pArg) override;

public:
	void	Update_Buffer(_fvector fMousePos, _float fRadious, _float fHeight, _float fSharpness);
	void	Compute_MousePos(_float3* pOut, _matrix matWorld);
	virtual HRESULT	Set_Buffer(const _char* strPath) override;
	virtual HRESULT	Init_Buffer(CMeshData::MESHDATADESC tMeshData) override;

public:
	HRESULT	Init_Terrain(DTERRAINDESC* pDTerrainDesc);

private:
	vector<VTXMESH>	m_vecVertexInfo;
	vector<_uint3>	m_vecIndexInfo;

private:
	_uint	m_iNumVerticesX = { 0 };
	_uint	m_iNumVerticesZ = { 0 };
	_uint   m_iNumFaces = { 0 };

public:
	static	CVIBuffer_DTerrain* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CComponent* Clone(void* pArg) ;
	virtual	void	Free() override;
};

END