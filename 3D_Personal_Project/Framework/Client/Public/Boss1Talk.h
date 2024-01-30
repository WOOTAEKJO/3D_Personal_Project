#pragma once
#include "Client_Production.h"

BEGIN(Client)

class CBoss1Talk final : public CClient_Production
{
private:
	CBoss1Talk();
	virtual	~CBoss1Talk() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual	void	Enter() override;
	virtual	void	Tick() override;
	virtual	void	Render() override;
	virtual	void	Exite() override;

private:

public:
	static CBoss1Talk* Create();
	virtual	void	Free();
};

END