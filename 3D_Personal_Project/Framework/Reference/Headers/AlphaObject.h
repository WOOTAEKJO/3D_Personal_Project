#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CAlphaObject : public CGameObject
{
protected:
	CAlphaObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAlphaObject(const CAlphaObject& rhs);
	virtual ~CAlphaObject() = default;

public:
	virtual HRESULT Initialize_Prototype(); // 원본 초기화
	virtual HRESULT Initialize(void* pArg); // 사본 초기화
	virtual void Priority_Tick(_float fTimeDelta);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Compute_CamDistance();

public:
	_float Get_CamDinstance() const {
		return m_fCamDistance;
	}

protected:
	_float		m_fCamDistance = { 0.f };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END