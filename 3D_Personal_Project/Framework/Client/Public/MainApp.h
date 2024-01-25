#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMainApp final  : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(_float fTimeDelta);
	HRESULT Render();

private:
	/* IDirect3DDevice9 == LPDIRECT3DDEVICE9 */
	ID3D11Device*			m_pDevice = { nullptr };		// �Ҵ� ���
	ID3D11DeviceContext*	m_pContext = { nullptr };		// ������ ���(�׸��� ���) ���
	CGameInstance*			m_pGameInstance = { nullptr };	// ���� �� Ŭ������ �� ������ ���ؼ��� ���� ����
private:
	//ImGuiIO* io = nullptr;

#ifdef _DEBUG
private:
	_tchar					m_szFPS[MAX_PATH] = TEXT("");
	_float					m_fTimeAcc = { 0.f };
	_uint					m_iNumRender = { 0 };

#endif

private:
	HRESULT Open_Level(LEVEL eStartLevelID);
	
private:
	HRESULT	Ready_ProtoType_Component_ForStaticLevel();
	HRESULT	Ready_Font();

public:
	static CMainApp* Create();
	virtual void Free();
};

END