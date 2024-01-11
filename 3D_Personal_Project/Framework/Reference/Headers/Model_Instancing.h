#pragma once
#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class CMesh;

class ENGINE_DLL CModel_Instancing final : public CVIBuffer_Instancing
{
public:
	typedef struct tagMesh_Instance_Desc
	{
		vector<_float4x4>		vecInstanceVertex;

	}MESH_INSTANCE_DESC;

private:
	CModel_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel_Instancing(const CModel_Instancing& rhs);
	virtual	~CModel_Instancing() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(const string& strModelFilePath, _fmatrix matPivot);
	virtual	HRESULT	Initialize(void* pArg);
	virtual HRESULT	Render(_uint iMeshIndx);

private:
	char	m_szName[MAX_PATH] = {};
	_uint	m_iMaterialIndex = { 0 };

private:
	_uint					m_iMeshesNum = { 0 };
	vector<CMesh*>			m_vecMesh;

	_uint					m_iMaterialsNum = { 0 };
	vector<MATERIAL_DESC>	m_vecMaterial;

private:
	_float4x4				m_matPivot;

private:
	HRESULT	Ready_Meshes(CMeshData::MESHDATADESC MeshData);
	HRESULT	Ready_Materials(CMeshData::MESHDATADESC MeshData, const string& strModelFilePath);

public:
	static	CModel_Instancing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& strModelFilePath, _fmatrix matPivot);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END