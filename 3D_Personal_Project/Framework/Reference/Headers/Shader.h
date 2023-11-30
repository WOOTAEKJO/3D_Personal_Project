#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
public:
	enum RENDERSTATE {RS_SOLID, RS_WIREFRAME, RS_END};

private:
	CShader(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& rhs);
	virtual	~CShader() = default;
		
public:
	HRESULT	Initialize_ProtoType(const wstring& strShaderFilePath,const D3D11_INPUT_ELEMENT_DESC * pElement, const _uint & iElementNum);
	virtual	HRESULT	Initialize(void* pArg) override;
public:
	HRESULT	Begin(_uint iPassIndex);
	HRESULT	Bind_Matrix(const _char* pMatrixName, const _float4x4* pMatrix);
	HRESULT	Bind_Matrixes(const _char * pMatrixName, const _float4x4 * pMatrix, const _uint & iMatrixNum);
	HRESULT	Bind_SRV(const _char * pTextureName, ID3D11ShaderResourceView * pSRV);
	HRESULT	Bind_SRVS(const _char * pTextureName, ID3D11ShaderResourceView** pSRV, const _uint& iSRVNum);
	HRESULT	Bind_RawValue(const _char * pValueName, const void* pData, _uint iSize);

public:
	HRESULT	Set_RenderState(RENDERSTATE eType);

private:
	ID3DX11Effect*		m_pEffect = { nullptr };
	/*
		- 쉐이더 하나를 연결해서 이 변수로 쉐이더를 사용
	*/
	D3DX11_TECHNIQUE_DESC		m_Technique = {};	// technique에 정보를 저장할 구조체 변수
	vector<ID3D11InputLayout*>	m_vecInputLayout;
	// pass를 이용해서 만든 InputLayout들을 보관할 벡터

private:
	ID3D11RasterizerState* m_pWireFrame = { nullptr };
	ID3D11RasterizerState* m_pSolidFrame = { nullptr };
	ID3D11RasterizerState* m_pFrame[RS_END];

private:
	HRESULT	 Ready_RenderState();

public:
	static	CShader*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElement, const _uint& iElementNum);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void		Free() override;
};

END