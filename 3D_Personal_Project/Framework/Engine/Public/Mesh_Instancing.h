#pragma once
//#include "VIBuffer.h"
#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class CMesh_Instancing final : public CVIBuffer_Instancing
{
private:
	CMesh_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh_Instancing(const CMesh_Instancing& rhs);
	virtual	~CMesh_Instancing() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(CModel::TYPE eType, MESH MeshData,
		_fmatrix matPivot, CModel::BONES& pBones, MESH_INSTACE_TYPE eBufferType);
	virtual	HRESULT	Initialize(void* pArg);

public:
	_bool	Compute_MousePos(_float3* pOut, _matrix matWorld,_float* fDist);
	_uint	Get_MaterialIndex() const { return m_iMaterialIndex; }
	HRESULT	Bind_Blend(class CShader* pShader, const _char* strName, CModel::BONES& pBones, _int iNonBindInd);

private:
	char	m_szName[MAX_PATH] = {};
	_uint	m_iMaterialIndex = { 0 };

private:
	vector<_float3>			m_vecVertexInfo;
	vector<_uint3>			m_vecIndexInfo;

private:
	_uint					m_iNumBones;
	vector<_uint>			m_vecBoneIndices;
	vector<_float4x4>		m_vecOffsetMatrix;

private:
	HRESULT	Anim_Vertex(MESH MeshData, CModel::BONES& pBones);
	HRESULT	NonAnim_Vertex(MESH MeshData, _fmatrix matPivot);

public:
	static	CMesh_Instancing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		CModel::TYPE eType, MESH MeshData, _fmatrix matPivot, CModel::BONES& pBones, MESH_INSTACE_TYPE eBufferType);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END