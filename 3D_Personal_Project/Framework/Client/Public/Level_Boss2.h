#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Boss2 final : public CLevel
{
private:
	CLevel_Boss2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Boss2() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	HRESULT	Ready_Layer_BackGround(const wstring& strLayerTag);
	HRESULT	Ready_Layer_Player(const wstring& strLayerTag);
	HRESULT	Ready_Layer_Plateform(const wstring& strLayerTag);
	HRESULT	Ready_Layer_Camera(const wstring& strLayerTag);
	HRESULT	Ready_Layer_Monster(const wstring& strLayerTag);
	
public:
	static CLevel_Boss2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END