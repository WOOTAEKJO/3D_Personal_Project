#pragma once
#include "Client_Production.h"

BEGIN(Client)

class CBoss2Talk final : public CClient_Production
{
private:
	CBoss2Talk();
	virtual	~CBoss2Talk() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual	void	Enter() override;
	virtual	void	Tick() override;
	virtual	void	Render() override;
	virtual	void	Exite() override;

private:

public:
	static CBoss2Talk* Create();
	virtual	void	Free();
};

END