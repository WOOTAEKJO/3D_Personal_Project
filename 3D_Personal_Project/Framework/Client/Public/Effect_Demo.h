#pragma once
#include "Demo.h"

BEGIN(Engine)

class CVIBuffer_Rect;

END

BEGIN(Client)

class CEffect_Demo final : public CDemo
{
private:
	CEffect_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Demo(const CEffect_Demo& rhs);
	virtual	~CEffect_Demo() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Set_Control_Variable(void* pArg) override;

public:
	_bool	Get_Picked();

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

public:
	void	Set_SocketBone(CBone* pBone);
	void	Set_ParentsTransform(CTransform* pTransform) { m_pParentsTransform = pTransform; }

private:
	CShader*		m_pShaderCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pBufferCom = { nullptr };

private:
	wstring			m_strTextureTag;

private:
	CTransform* m_pParentsTransform = { nullptr };
	CBone*		m_pSocketBone = { nullptr };
	_float4x4	m_matWorldMat;

private:
	_bool		m_bSprite = { false };
	_float		m_fFrame = { 0.f };
	_float		m_fMaxFrame = { 0.f };
	
private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;

public:
	static CEffect_Demo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END