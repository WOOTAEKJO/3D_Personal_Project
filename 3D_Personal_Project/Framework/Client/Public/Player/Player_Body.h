#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CModel;
class CShader;

END

BEGIN(Client)

class CPlayer_Body final : public CGameObject
{	
public:
	typedef struct tagPlayer_Body_Desc
	{
		CTransform* pParentsTransform = nullptr;

	}PLAYERBODY_DESC;
private:
	CPlayer_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Body(const CPlayer_Body& rhs);
	virtual	~CPlayer_Body() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;
	virtual HRESULT Render_Blur() override;

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

	HRESULT	Load_Data(const _char* strFilePath);

public:
	void Set_AnimationIndex(_uint iAnimIndex);

private:
	CModel*		m_pModelCom = { nullptr };
	CShader*	m_pShaderCom = { nullptr };

private:
	CTransform* m_pParentsTransform = { nullptr };
	_float4x4	m_matWorldMat = {};

private:
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Component();

public:
	static	CPlayer_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END