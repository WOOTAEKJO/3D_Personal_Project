#pragma once
#include "Client_Production.h"

BEGIN(Client)

class CEnding2 final : public CClient_Production
{
private:
	CEnding2();
	virtual	~CEnding2() = default;

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
	static CEnding2* Create();
	virtual	void	Free();
};

END