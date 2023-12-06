#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

BEGIN(Engine)

class CShader;
class CTexture;

END

BEGIN(Client)

class CDemo abstract : public CGameObject
{
protected:
	CDemo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDemo(const CDemo& rhs);
	virtual	~CDemo() = default;

public:
	virtual HRESULT Initialize_Prototype() = 0;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) = 0;
	virtual void Tick(_float fTimeDelta) = 0;
	virtual void Late_Tick(_float fTimeDelta) = 0;
	virtual HRESULT Render() override;
	virtual HRESULT	Set_Control_Variable(void* pArg) = 0;

protected:

protected:
	virtual HRESULT	Bind_ShaderResources() = 0;
	virtual HRESULT	Ready_Component() = 0;

public:
	virtual	CGameObject* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END