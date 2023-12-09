#pragma once
#include "Converter_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Converter)

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
private:
	HRESULT Open_Level(LEVEL eStartLevelID);
	
private:
	HRESULT	Ready_ProtoType_Component_ForStaticLevel();

public:
	static CMainApp* Create();
	virtual void Free();
};

END