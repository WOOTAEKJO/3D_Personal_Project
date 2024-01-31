#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CCollider;
class CLight;
class CModel;
class CShader;

END

BEGIN(Client)

class CPuzzle final : public CGameObject
{
public:

	typedef struct tagPuzzle_Desc
	{
		wstring		strModelTag;
		_float4		vLightColor;

	}PUZZLE_DESC;

private:
	CPuzzle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPuzzle(const CPuzzle& rhs);
	virtual	~CPuzzle() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void	Down(_float fTimeDelta);

public:
	virtual void	OnCollisionEnter(CCollider* pCollider, _uint iColID);
	virtual void	OnCollisionStay(CCollider* pCollider, _uint iColID);
	virtual void	OnCollisionExit(CCollider* pCollider, _uint iColID);

public:
	void	Set_PuzzleIndx(_int iIdnx) { m_iPuzzleIndx = iIdnx; }
	_int	Get_PuzzleIndx() { return m_iPuzzleIndx; }

	void	Set_LightColor(_float4 vColor) { m_vLightColor = vColor; }

	wstring	Get_ModelTag() { return m_strModelTag; }

	void	Set_True(_bool bTrue) { m_bTrue = bTrue; }
	_bool	Get_True() { return m_bTrue; }

	void	Set_Hit(_bool bHit) { m_bHit = bHit; }
	_bool	Get_Hit() { return m_bHit; }

private:
	CCollider*	m_pColliderCom = { nullptr };
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };

private:
	CLight* m_pLight = { nullptr };

private:
	wstring		m_strModelTag;
	_float4		m_vLightColor;
	_float4		m_vResetColor = { 1.f,1.f,1.f,1.f };

	_bool		m_bHit = { false };
	_float		m_fTimeAcc = { 0.f };

private:
	_int		m_iPuzzleIndx = { -1 };
	_bool		m_bTrue = { false };
	_bool		m_bFinish = { false };
	_bool		m_bRelent = { false };

private:
	HRESULT Bind_ShaderResources();
	HRESULT	Ready_Component();
	HRESULT	Init_Point_Light();

private:
	void	Reset_Hit(_float fTimeDelta);

public:
	static CPuzzle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	CGameObject*	Clone(void* pArg) override;
	virtual	void	Free() override;
};

END