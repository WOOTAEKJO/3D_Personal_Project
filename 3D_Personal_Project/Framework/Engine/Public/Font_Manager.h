#pragma once
#include "Base.h"

BEGIN(Engine)

class CCustomFont;

class CFont_Manager final : public CBase
{
private:
	CFont_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	~CFont_Manager() = default;

public:
	HRESULT Initialize();
	HRESULT	Add_Font(_uint iFontTag,const wstring& strFontFilePath);
	HRESULT	Render(_uint iFontTag,const wstring& strText, _float2 vPosition, _fvector vColor,
		_float fScale, _float2 vOrigin, _float fRotation);

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

private:
	map<_uint, CCustomFont*>	m_mapFont;

private:
	CCustomFont* Find_Font(_uint iFontTag);

public:
	static CFont_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void	Free() override;
};

END