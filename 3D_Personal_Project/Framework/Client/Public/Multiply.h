#pragma once
#include "Bullet.h"

BEGIN(Engine)

class CModel;

END

BEGIN(Client)

class CMultiply final : public CBullet
{
private:
	CMultiply(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMultiply(const CMultiply& rhs);
	virtual	~CMultiply() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	void	Toward(_float fTimeDelta);

private:
	CBone* m_pSocketBone = { nullptr };

private:
	CModel*		m_pModelCom = { nullptr };

private:
	CGameObject* m_pEffect = { nullptr };
	_bool		m_bTrans = { false };
	_float		m_fTransTimeAcc = { 0.f };

private:
	HRESULT	Bind_ShaderResources();
	HRESULT	Ready_Component();

public:
	static CMultiply* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END