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
	ID3D11Device*			m_pDevice = { nullptr };		// 할당 담당
	ID3D11DeviceContext*	m_pContext = { nullptr };		// 나머지 기능(그리기 등등) 담당
	CGameInstance*			m_pGameInstance = { nullptr };	// 엔진 쪽 클래스는 이 변수를 통해서만 접근 가능
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