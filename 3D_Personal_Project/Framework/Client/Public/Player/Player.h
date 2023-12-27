#pragma once
#include "Character.h"

BEGIN(Client)



class CPlayer final : public CCharacter
{
public:
	enum STATE {IDLE,RUN,ATTACK1, ATTACK2, ATTACK3, AIR_ATTACK,JUMP,ROLL,STATE_END};
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual	~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CGameObject* Find_Parts(const wstring& strPartsTag);
	CModel* Get_BodyModel();

public:
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID) override;
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID) override;

private:
	map<const wstring, CGameObject*>	m_mapParts;

private:
	virtual HRESULT Bind_ShaderResources() override;
	virtual HRESULT Ready_Component() override;

private:
	HRESULT	Ready_State();
	HRESULT	Ready_Parts();
	HRESULT	Add_Parts(const wstring& strPrototypeTag, const wstring& strPartsTag, void* pArg = nullptr);

public:
	static	CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END