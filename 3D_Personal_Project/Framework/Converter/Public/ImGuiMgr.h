#pragma once
#include "Converter_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Converter)

class CMesh_Demo;

class CImGuiMgr final : public CBase
{
	DECLARE_SINGLETON(CImGuiMgr)

public:
	enum MODELTYPE {TYPE_NONANIM, TYPE_ANIM,TYPE_END};
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
	vector<CMesh_Demo*>	m_vecMesh_Demo[TYPE_END];

private:
	_bool					m_bFileDialog = { false };

	_uint					m_iCurrentIndex = { 0 };
	MODELTYPE				m_eCurrentType = { TYPE_END };

private:
	HRESULT		Binarization(const _char* strFilePath);

	HRESULT		Init_Model();
	HRESULT		Create_Model(MODELTYPE eType, const wstring& strTag);

	wstring	Split_Wstring(wstring strFull, _tchar cSeperator);

public:
	virtual	void	Free() override;

};

END