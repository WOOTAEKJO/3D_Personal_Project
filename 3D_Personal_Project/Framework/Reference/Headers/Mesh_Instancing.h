#pragma once
#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CMesh_Instancing final : public CVIBuffer_Instancing
{
public:
	typedef struct tagMesh_Instance_Desc
	{
		vector<_float4x4>		vecInstanceVertex;

	}MESH_INSTANCE_DESC;

private:
	CMesh_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh_Instancing(const CMesh_Instancing& rhs);
	virtual	~CMesh_Instancing() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(MESH MeshData, _fmatrix matPivot);
	virtual	HRESULT	Initialize(void* pArg);

public:
	_bool	Compute_MousePos(_float3* pOut, _matrix matWorld,_float* fDist);
	_uint	Get_MaterialIndex() const { return m_iMaterialIndex; }

private:
	char	m_szName[MAX_PATH] = {};
	_uint	m_iMaterialIndex = { 0 };

private:
	vector<_float3>			m_vecVertexInfo;
	vector<_uint3>			m_vecIndexInfo;

public:
	static	CMesh_Instancing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MESH MeshData, _fmatrix matPivot);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END