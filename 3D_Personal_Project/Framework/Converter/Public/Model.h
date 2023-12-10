#pragma once
#include "Component.h"

BEGIN(Engine)

class CMesh;
class CAnimation;
class CBone;

class ENGINE_DLL CModel final : public	CComponent
{
public:
	enum TYPE {TYPE_NONANIM, TYPE_ANIM,TYPE_END};
	typedef vector<CBone*> BONES;
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual	~CModel() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(TYPE eType, const string & strModelFilePath, _fmatrix	matPivot);
	virtual	HRESULT	Initialize(void* pArg);
	virtual	HRESULT	Render(_uint iMeshIndex);
	void	Play_Animation(_float fTimeDelta, _bool bLoop);

public:
	_uint	Get_MeshesNum() const { return m_iMeshesNum; }

public:
	_bool	Compute_MousePos(_float3 * pOut, _matrix matWorld);
	HRESULT	Bind_ShaderResources(class CShader* pShader, const _char* pName,_uint iMeshIndex, aiTextureType eType);
	HRESULT	Bind_Blend(class CShader* pShader, const _char* pName, _uint iMeshIndex);

private:
	const aiScene*		m_pAiScene = { nullptr };
	Assimp::Importer	m_Importer;
	
private:
	_uint					m_iMeshesNum = { 0 };
	vector<CMesh*>			m_vecMesh;

	_uint					m_iMaterialsNum = { 0 };
	vector<MATERIAL_DESC>	m_vecMaterial;

	BONES	m_vecBones;

	_uint					m_iAnimationNum = { 0 };
	_uint					m_iCurrentAnimationIndex = { 1 };
	vector<CAnimation*>		m_vecAnimation;

private:
	TYPE					m_eType = { TYPE_END };

	_float4x4				m_matPivot;

private:
	HRESULT	Ready_Meshes(_fmatrix	matPivot);
	HRESULT	Ready_Materials(const string & strModelFilePath);
	HRESULT	Ready_Bones(aiNode* pNode, _int iParentIndex);
	HRESULT	Ready_Animation();

public:
	static	CModel* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,TYPE eType, const string& strModelFilePath, _fmatrix	matPivot);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END