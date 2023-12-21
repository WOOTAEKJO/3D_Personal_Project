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
public:
	typedef struct Demo_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		wstring strModelTag;
		_float4		vPos;

	}DEMO_DESC;

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

public:
	wstring	Get_ModelTag() { return m_strModelTag; }

protected:
	wstring		m_strModelTag;
	_float4		m_vObjectPos = {};

protected:
	virtual HRESULT	Bind_ShaderResources() = 0;
	virtual HRESULT	Ready_Component() = 0;

public:
	virtual	CGameObject* Clone(void* pArg) = 0;
	virtual	void	Free() override;
};

END