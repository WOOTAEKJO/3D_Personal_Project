#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& rhs);
	virtual	~CShader() = default;
		
public:
	HRESULT	Initialize_ProtoType(const wstring& strShaderFilePath);
	virtual	HRESULT	Initialize(void* pArg) override;

private:
	ID3DX11Effect*		m_pEffect = { nullptr };
	/*
		- 쉐이더 하나를 연결해서 이 변수로 쉐이더를 사용
	*/

public:
	static	CShader*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const wstring& strShaderFilePath);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void		Free() override;
};

END