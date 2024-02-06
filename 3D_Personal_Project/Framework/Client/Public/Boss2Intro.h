#pragma once
#include "Client_Production.h"

BEGIN(Client)

class CBoss2Intro final : public CClient_Production
{
private:
	CBoss2Intro();
	virtual	~CBoss2Intro() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual	void	Enter() override;
	virtual	void	Tick() override;
	virtual	void	Render() override;
	virtual	void	Exite() override;

private:

public:
	static CBoss2Intro* Create();
	virtual	void	Free();
};

END