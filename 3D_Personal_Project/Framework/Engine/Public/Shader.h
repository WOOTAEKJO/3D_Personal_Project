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
	HRESULT	Initialize_ProtoType(const wstring& strShaderFilePath,const D3D11_INPUT_ELEMENT_DESC * pElement, const _uint & iElementNum);
	virtual	HRESULT	Initialize(void* pArg) override;
public:
	HRESULT	Begin(_uint iPassIndex);
	HRESULT	Bind_Matrix(const _char* pMatrixName, const _float4x4* pMatrix);

private:
	ID3DX11Effect*		m_pEffect = { nullptr };
	/*
		- ���̴� �ϳ��� �����ؼ� �� ������ ���̴��� ���
	*/
	D3DX11_TECHNIQUE_DESC		m_Technique = {};	// technique�� ������ ������ ����ü ����
	vector<ID3D11InputLayout*>	m_vecInputLayout;
	// pass�� �̿��ؼ� ���� InputLayout���� ������ ����

public:
	static	CShader*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElement, const _uint& iElementNum);
	virtual	CComponent* Clone(void* pArg) override;
	virtual	void		Free() override;
};

END