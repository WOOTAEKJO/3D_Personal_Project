#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{

public:
	enum LOADTYPE {TYPE_HEIGHT, TYPE_BINARY,TYPE_END};
private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual	~CVIBuffer_Terrain() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(LOADTYPE eType, const wstring & strFilePath);
	virtual	HRESULT	Initialize(void* pArg) override;
	virtual HRESULT	Init_Buffer(CMeshData::MESHDATADESC tMeshData);

public:
	HRESULT	Init_HegithTexture(const wstring & strHeightMapFilePath);

private:
	_uint	m_iNumVerticesX = { 0 };
	_uint	m_iNumVerticesZ = { 0 };
	_uint   m_iNumFaces = { 0 };

public:
	static	CVIBuffer_Terrain* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LOADTYPE eType, const wstring & strFilePath);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;

};

END