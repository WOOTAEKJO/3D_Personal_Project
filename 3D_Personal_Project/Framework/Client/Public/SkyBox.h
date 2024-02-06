#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CVIBuffer_Cube;
class CShader;
class CTexture;

END

BEGIN(Client)

class CSkyBox final : public CGameObject
{

private:
	CSkyBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkyBox(const CSkyBox& rhs);
	virtual	~CSkyBox() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader*		m_pShaderCom = { nullptr };
	CVIBuffer_Cube* m_pBufferCom = { nullptr };
	CTexture*		m_pTextureCom = { nullptr };

private:
	_float2						m_vFogPosition = { 0.f,0.f };
	_float4						m_vFogColor = { 1.f,1.f,1.f,1.f };

private:
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Component() ;

public:
	static CSkyBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END