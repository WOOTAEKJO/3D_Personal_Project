#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CModel;
class CShader;
class CBone;

END

BEGIN(Client)

class CPlayer_Weapon_Spear final : public CGameObject
{	
public:
	typedef struct tagPlayer_Weapon_Spear_Desc
	{
		vector<CBone*>		pBones;
		CTransform* pParentsTransform = nullptr;

	}PLAYERSPEAR_DESC;
private:
	CPlayer_Weapon_Spear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Weapon_Spear(const CPlayer_Weapon_Spear& rhs);
	virtual	~CPlayer_Weapon_Spear() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Load_FromJson(const json& In_Json) override;

private:
	CModel*		m_pModelCom = { nullptr };
	CShader*	m_pShaderCom = { nullptr };

private:
	CBone*		m_pSocketBone = { nullptr };
	_uint		m_iSocketBoneIndex = { 0 };

	CTransform* m_pParentsTransform = { nullptr };
	_float4x4	m_matWorldMat = {};

private:
	wstring		m_strModelTag;

private:
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Component();
	HRESULT	Load_Data(const _char* strFilePath);

public:
	static	CPlayer_Weapon_Spear* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END