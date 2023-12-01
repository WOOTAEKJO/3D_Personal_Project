#pragma once
#include "Component.h"

BEGIN(Engine)

class CMesh;

class ENGINE_DLL CModel final : public	CComponent
{
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual	~CModel() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(const string & strModelFilePath, _fmatrix	matPivot);
	virtual	HRESULT	Initialize(void* pArg);
	virtual	HRESULT	Render();

private:
	const aiScene*		m_pAiScene = { nullptr };
	Assimp::Importer	m_Importer;
	
private:
	_uint		m_iMeshesNum = { 0 };
	vector<CMesh*>	m_vecMesh;

private:
	HRESULT	Ready_Meshes(_fmatrix	matPivot);

public:
	static	CModel* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const string& strModelFilePath, _fmatrix	matPivot);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END