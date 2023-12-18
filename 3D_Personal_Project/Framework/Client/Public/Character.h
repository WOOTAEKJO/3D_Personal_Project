#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CModel;
class CShader;
class CNavigation;

END

BEGIN(Client)

class CCharacter abstract : public CGameObject
{
protected:
	CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacter(const CCharacter& rhs);
	virtual	~CCharacter() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CModel*		Get_ModelCom()	{ return m_pModelCom; }
	CTransform* Get_Transform() { return m_pTransformCom; }
	CNavigation* Get_Navigation() { return m_pNavigationCom; }

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };

protected:
	wstring		m_strModelTag;

protected:
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Ready_Component();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END