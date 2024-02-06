#pragma once
#include "Client_Production.h"

BEGIN(Client)

class CCrowTalk final : public CClient_Production
{
private:
	CCrowTalk();
	virtual	~CCrowTalk() = default;

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
	static CCrowTalk* Create();
	virtual	void	Free();
};

END