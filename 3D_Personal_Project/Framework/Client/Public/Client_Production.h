#pragma once
#include "Client_Defines.h"
#include "Production.h"

BEGIN(Engine)

END

BEGIN(Client)

class CTargetCamera;

class CClient_Production abstract : public CProduction
{
public:
	typedef struct tagActorInfo
	{
		_float3 vOffset;
		_float fSensitivity;
		wstring strActorTag;
		_int	iAnimIndex = { -1 };

	}PRODUCTION_ACTOR_DESC;

protected:
	CClient_Production();
	virtual	~CClient_Production() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual	void	Enter() override;
	virtual	void	Tick() override;
	virtual	void	Render() override;
	virtual	void	Exite() override;

protected:
	CTargetCamera*	m_pCamera = { nullptr };

protected:
	vector<PRODUCTION_ACTOR_DESC> m_vecActorOrder;
	_uint							m_iCurrentOrderIndx = { 0 };

protected:
	_bool	SetUp_Camera();
	void	Camera_Target_Change(_bool bSpring = false);
	void	Camera_Reset();

public:
	virtual	void	Free();
};

END