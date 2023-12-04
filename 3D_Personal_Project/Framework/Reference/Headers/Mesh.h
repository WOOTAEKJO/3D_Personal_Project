#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual	~CMesh() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(const aiMesh* pMesh, _fmatrix matPivot);
	virtual	HRESULT	Initialize(void* pArg);

public:
	_bool	Compute_MousePos(_float3* pOut, _matrix matWorld);
	_uint	Get_MaterialIndex() const { return m_iMaterialIndex; }

private:
	char	m_cName[MAX_PATH] = {};
	_uint	m_iMaterialIndex = { 0 };

private:
	vector<VTXTBN>	m_vecVertexInfo;
	vector<_uint3>	m_vecIndexInfo;

public:
	static	CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pMesh,_fmatrix matPivot);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END