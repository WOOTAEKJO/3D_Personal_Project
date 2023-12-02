#pragma once
#include "Client_Defines.h"
#include "GameObject_Test.h"

BEGIN(Engine)

class CVIBuffer_Cube;
class CShader;
class CTexture;

END

BEGIN(Client)

class CMark final : public CGameObject
{
private:
	CMark(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMark(const CMark& rhs);
	virtual	~CMark() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_Cube*		m_pBufferCom = { nullptr };
	CShader*			m_pShaderCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CMark* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END