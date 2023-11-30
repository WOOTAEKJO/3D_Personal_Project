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
	virtual	HRESULT	Initialize_ProtoType(const string & strModelFilePath);
	virtual	HRESULT	Initialize(void* pArg);

private:
	const aiScene*		m_pAiScene = { nullptr };
	Assimp::Importer	m_Importer;
	
private:
	_uint		m_iMeshesNum = { 0 };
	vector<CMesh*>	m_vecMesh;

private:
	HRESULT	Ready_Meshes();

public:
	static	CModel* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const string& strModelFilePath);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END