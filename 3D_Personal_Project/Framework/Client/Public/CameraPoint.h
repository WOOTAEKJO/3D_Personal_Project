#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CCollider;

END

BEGIN(Client)

class CCameraPoint final : public CGameObject
{
public:
	typedef struct tagCameraPoint_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		_float4		vAtPos;
		_float4		vPosition;
		_float3     vScale = _float3(1.f,1.f,1.f);

	}CAMERAPOINT_DESC;

public:
	enum TEXTURE { TYPE_DIFFUSE, TYPE_MASK, TYPE_END};
private:
	CCameraPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCameraPoint(const CCameraPoint& rhs);
	virtual	~CCameraPoint() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID);
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID);
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID);

private:
	CCollider* m_pColliderCom = { nullptr };

private:
	HRESULT	Ready_Component();

public:
	static CCameraPoint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END