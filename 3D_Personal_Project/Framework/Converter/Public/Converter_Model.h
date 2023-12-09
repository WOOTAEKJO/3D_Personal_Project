#pragma once
#include "Converter_Defines.h"
#include "VIBuffer.h"

BEGIN(Converter)

class CConverter_Model final : public CVIBuffer
{
public:
	enum TYPE {TYPE_NONANIM, TYPE_ANIM, TYPE_END};
private:
	CConverter_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CConverter_Model(const CConverter_Model& rhs);
	virtual	~CConverter_Model() = default;

public:
	virtual	HRESULT	Initialize_ProtoType(TYPE eType, const string& strModelFilePath);
	virtual	HRESULT	Initialize(void* pArg) override;

public:
	virtual HRESULT	Set_Buffer(const _char* strPath) override;

private:
	const aiScene*			m_pAiScene = { nullptr };
	Assimp::Importer		m_Importer;

private:
	TYPE					m_eType = { TYPE_END };

	_bool					m_bReady = false;

private:
	_uint					m_iMashesNum = { 0 };
	vector<MESH>			m_vecMesh;

	_uint					m_iMaterialsNum = { 0 };
	vector<MATERIAL>		m_vecMaterial;

	_uint					m_iAnimationsNum = { 0 };
	vector<ANIMATION>		m_vecAnimation;

	_uint					m_iBonesNum = { 0 };
	vector<BONE>			m_vecBone;

private:
	HRESULT	Ready_Meshes();
	HRESULT	Ready_Materials(const string& strModelFilePath);
	HRESULT	Ready_Bones(aiNode* pNode, _int iParentIndex);
	HRESULT	Ready_Animation();

public:
	static	CConverter_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,TYPE eType, const string& strModelFilePath);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END