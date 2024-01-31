#pragma once
#include "Client_Defines.h"
#include "Base.h"
//#include "Puzzle.h"

BEGIN(Engine)

class CGameObject;
class CGameInstance;

END

BEGIN(Client)

class CPuzzle;

class CPuzzleMgr final : public CBase
{
	DECLARE_SINGLETON(CPuzzleMgr)
private:
	CPuzzleMgr();
	virtual ~CPuzzleMgr() = default;

public:
	HRESULT	Initialize();
	void	Add_Puzzle(CGameObject* pPuzzle);
	void	Tick();

private:
	CGameInstance*	m_pGameInstance = { nullptr };

private:
	vector<CPuzzle*>	m_vecPuzzle;
	_uint				m_iPuzzleNum = { 0 };

private:
	_int				m_iMatch[2] = { -1,-1 };
	_uint				m_iSuccessNum = { 0 };
	_bool				m_bFinish = { false };

private:
	_float4	Select_Color(const wstring& strModelTag);
	_float4 Select_Color(_uint iIndx);
	_bool	Compare_Indx(_uint iSour, _uint iDest);

public:
	virtual	void	Free() override;

};

END