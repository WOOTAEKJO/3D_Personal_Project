#pragma once
#include "Base.h"

BEGIN(Engine)

class CMeshData final : public CBase
{
public:
	enum MODEL_TYPE {NONANIM, ANIM, TERRAIN, TYPE_END};

	typedef	struct tagMesh_Data_Desc
	{
		vector<VTXPOSTEX> 	vecPosVertices;
		vector<VTXNORTEX>	vecNorVertices;
		vector<VTXTBN>		vecTBNVertices;
		vector<VTXMESH>		vecMeshVertices;
		vector<VTXANIMMESH>	vecAnimVertices;

		vector<_uint3>		vecIndices;

		_uint				iNumVertices;
		_uint				iNumFaces;
		_uint				iNumBones;

		MODEL_TYPE			eModel_Type = TYPE_END;
	}MESHDATADESC;

private:
	CMeshData();
	virtual	~CMeshData() = default;

public:
	HRESULT	Initialize(MESHDATADESC MeshDataDesc);
	HRESULT	Save_Data(const char* strPath);
	HRESULT	Load_Data(const char* strPath);

public:
	HRESULT	Data_Get(MESHDATADESC& MeshDataDesc);

private:
	vector<VTXPOSTEX> 	m_vecPosVertices;
	vector<VTXNORTEX>	m_vecNorVertices;
	vector<VTXTBN>		m_vecTBNVertices;
	vector<VTXMESH>		m_vecMeshVertices;
	vector<VTXANIMMESH>	m_vecAnimVertices;

	vector<_uint3>		m_vecIndices;

private:
	_uint				m_iNumVertices;
	_uint				m_iNumFaces;
	_uint				m_iNumBones;

	MODEL_TYPE			m_eModel_Type = { TYPE_END };

private:
	HRESULT	Set_Data(MESHDATADESC MeshDataDesc);
	
	
public:
	static CMeshData* Create(MESHDATADESC MeshDataDesc);
	virtual	void	Free() override;
};

END