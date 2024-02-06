#pragma once
#include "Client_Defines.h"
#include "AlphaObject.h"

BEGIN(Engine)

class CVIBuffer_Particle_Point;
class CShader;
class CTexture;

END

BEGIN(Client)

class CParticle abstract : public CAlphaObject
{
public:
	typedef struct tagParticleInfo : public CGameObject::GAMEOBJECT_DESC
	{
		wstring			strParticleTag;
		CGameObject*	pOwner = nullptr;
		_float			fLifeTime = 0.f;
		vector<CBone*>		pBones;
		_bool				bChild = false;
		_bool				bBlur = true;

	}PARTICLEINFO;

protected:
	CParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle(const CParticle& rhs);
	virtual	~CParticle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Load_FromJson(const json& In_Json) override;

protected:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Particle_Point* m_pBufferCom = { nullptr };

protected:
	CGameObject* m_pOwner = { nullptr };

protected:
	_bool			m_bUpdate = { false };

protected:
	wstring		m_strParticleTag;

protected:
	CTransform* m_pOwnerTransform = { nullptr };
	CBone*		m_pSocketBone = { nullptr };
	_uint		m_iSocketBoneIndex = { 0 };
	_float4x4	m_matWorldMat;

protected:
	_float		m_fTimeAcc = { 0.f };
	_float		m_fLifeTime = { 0.f };

protected:
	_bool		m_bChild = { false };
	_bool		m_bBlur = { true };

protected:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

	void	Judge_Dead(_float fTimeDelta);

public:
	virtual	CGameObject* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END