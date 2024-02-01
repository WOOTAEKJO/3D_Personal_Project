#pragma once
#include "Client_Production.h"

BEGIN(Client)

class COwlTalk2 final : public CClient_Production
{
private:
	COwlTalk2();
	virtual	~COwlTalk2() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual	void	Enter() override;
	virtual	void	Tick() override;
	virtual	void	Render() override;
	virtual	void	Exite() override;

private:
	virtual	HRESULT Ready_UI() override;
	virtual	void	Ready_Font() override;

public:
	static COwlTalk2* Create();
	virtual	void	Free();
};

END