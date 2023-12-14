#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CMeshData final : public CBase
{
public:
	enum MODEL_TYPE {NONANIM, ANIM, TERRAIN, TYPE_END};

	typedef	struct tagMesh_Data_Desc
	{
		vector<VTXPOSTEX> 	vecPosVertices;
		vector<VTXNORTEX>	vecNorVertices;
		vector<VTXTBN>		vecTBNVertices;
		vector<VTXMESH>		vecMeshVertices;

		vector<MESH>		vecMesh;
		vector<MATERIAL>	vecMaterial;
		vector<ANIMATION>	vecAnimAnimation;
		vector<BONE>		vecAnimBone;

		vector<_uint3>		vecIndices;

		_uint				iMeshNum ;
		_uint				iMaterialNum ;
		_uint				iAnimAnimationNum ;
		_uint				iAnimBoneNum ;

		_uint				iNumVertices;
		_uint				iNumFaces;

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
	
	vector<_uint3>		m_vecIndices;
	
private: /* For. Anim*/
	
	vector<MESH>			m_vecMesh;
	vector<MATERIAL>		m_vecMaterial;
	vector<ANIMATION>		m_vecAnimAnimation;
	vector<BONE>			m_vecAnimBone;

	_uint					m_iMeshNum = { 0 };
	_uint					m_iMaterialNum = { 0 };
	_uint					m_iAnimAnimationNum = { 0 };
	_uint					m_iAnimBoneNum = { 0 };

private:
	_uint				m_iNumVertices;
	_uint				m_iNumFaces;

	MODEL_TYPE			m_eModel_Type = { TYPE_END };

private:
	HRESULT	Set_Data(MESHDATADESC MeshDataDesc);
	
public:
	static CMeshData* Create(MESHDATADESC MeshDataDesc);
	virtual	void	Free() override;
};

END