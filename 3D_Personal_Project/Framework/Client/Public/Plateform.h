#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CModel;
class CShader;

END

BEGIN(Client)

class CPlateform final : public CGameObject
{
public:
	typedef struct tagPlateform_Desc
	{
		wstring		strModelTag;
		_float4		vPos;

	}PLATEFORM_DESC;
private:
	CPlateform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlateform(const CPlateform& rhs);
	virtual	~CPlateform() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Tick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Write_Json(json& Out_Json) override;
	virtual void Load_FromJson(const json& In_Json) override;

public:
	void		Set_ModelTag(const wstring& strModelTag) { m_strModelTag = strModelTag; }
	wstring		Get_ModelTag() { return m_strModelTag; }

	void		Set_TriggerNum(_int iNum) { m_iTriggerNum = iNum; }
	_int		Get_TriggerNum() { return m_iTriggerNum; }

	void		Set_Fall() { m_bFall = true; }

private:
	CShader*	m_pShaderCom = { nullptr };
	CModel*		m_pModelCom = { nullptr };

private:
	wstring		m_strModelTag;
	_float4		m_vObjectPos = {};	

private: // 보스 바닥 떨구기를 구현하기 위한 임시 코드 
	_int		m_iTriggerNum = { 0 };
	_bool		m_bFall = { false };

	random_device	m_RandomDevice;
	mt19937_64		m_RandomNumber;

	_float			m_fTimeAcc = { 0.f };

	_float3	m_fAngle = { 0.f,0.f,30.f };
	_float  m_fAmplitude = { 1.07f };
	_float3	m_fAngularVelocity = {};

private:
	HRESULT Bind_ShaderResources();
	HRESULT Ready_Component() ;

private:
	void	Fall(_float fTimeDelta);


public:
	static CPlateform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual	void	Free() override;
};

END