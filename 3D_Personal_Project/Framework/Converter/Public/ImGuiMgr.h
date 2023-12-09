#pragma once
#include "Converter_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Converter)

class CImGuiMgr final : public CBase
{
	DECLARE_SINGLETON(CImGuiMgr)

private:
	CImGuiMgr();
	virtual	~CImGuiMgr() = default;

public:
	HRESULT		Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void		Tick();
	HRESULT		Render();

	void		File_Render();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CGameInstance*			m_pGameInstance = { nullptr };

private:
	_bool					m_bFileDialog = { false };

private:
	HRESULT		Binarization();

public:
	virtual	void	Free() override;

};

END