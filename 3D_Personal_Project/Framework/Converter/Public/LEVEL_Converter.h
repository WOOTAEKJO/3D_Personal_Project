#pragma once
#include "Level.h"

BEGIN(Converter)

class CLEVEL_Converter final : public CLevel
{
private:
	CLEVEL_Converter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLEVEL_Converter() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	HRESULT	Ready_Layer_BackGround(const wstring & strLayerTag);

public:
	static CLEVEL_Converter* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual void Free() override;
};

END