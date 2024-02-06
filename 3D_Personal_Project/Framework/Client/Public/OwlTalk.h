#pragma once
#include "Client_Production.h"

BEGIN(Client)

class COwlTalk final : public CClient_Production
{
private:
	COwlTalk();
	virtual	~COwlTalk() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual	void	Enter() override;
	virtual	void	Tick() override;
	virtual	void	Render() override;
	virtual	void	Exite() override;

private:

private:
	virtual	HRESULT Ready_UI() override;
	virtual	void	Ready_Font() override;

public:
	static COwlTalk* Create();
	virtual	void	Free();
};

END