#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CVIBuffer_DTerrain;
class CShader;
class CTexture;

END

BEGIN(Client)

class CTerrain_Demo final : public CGameObject
{
public:
	typedef struct tagTerrainDemoValue
	{
		_float fRadius;
		_float fHeight;
		_float fSharpness;

	}TERRAINDEMOVALUE;

private:
	CTerrain_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain_Demo(const CTerrain_Demo& rhs);
	virtual	~CTerrain_Demo() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT	Create_DynamicBuffer(_uint iVerticesXNum = 3, _uint iVerticesZNum = 3);
	HRESULT	Delete_DynamicBuffer();
	HRESULT	Set_Control_Variable(void* pArg);

private:
	CVIBuffer_DTerrain* m_pVIBufferCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

private:
	_float	m_fHeight = { 0.f };
	_float	m_fRadius = { 0.f };
	_float	m_fSharpness = { 0.f };

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CTerrain_Demo* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual	CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END