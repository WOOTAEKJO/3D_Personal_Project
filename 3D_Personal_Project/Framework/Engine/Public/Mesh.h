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
	//virtual	HRESULT	Initialize_ProtoType(CModel::TYPE eType, const aiMesh* pMesh, _fmatrix matPivot, CModel::BONES& pBones);
	virtual	HRESULT	Initialize_ProtoType(CModel::TYPE eType, MESH MeshData, _fmatrix matPivot, CModel::BONES& pBones);
	virtual	HRESULT	Initialize(void* pArg);

public:
	_bool	Compute_MousePos(_float3* pOut, _matrix matWorld);
	_uint	Get_MaterialIndex() const { return m_iMaterialIndex; }
	HRESULT	Bind_Blend(class CShader* pShader, const _char* strName, CModel::BONES& pBones);

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
	//static	CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,CModel::TYPE eType, const aiMesh* pMesh,_fmatrix matPivot, CModel::BONES& pBones);
	static	CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eType, MESH MeshData, _fmatrix matPivot, CModel::BONES& pBones);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END