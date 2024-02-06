#pragma once
#include "Client_Production.h"

BEGIN(Client)

class CEnding final : public CClient_Production
{
private:
	CEnding();
	virtual	~CEnding() = default;

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
	static CEnding* Create();
	virtual	void	Free();
};

END