#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CShader::CShader(const CShader& rhs)
	:CComponent(rhs),
	m_pEffect(rhs.m_pEffect),
	m_Technique(rhs.m_Technique),
	m_vecInputLayout(rhs.m_vecInputLayout)
{
	for (auto& iter : m_vecInputLayout)
		Safe_AddRef(iter);

	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_ProtoType(const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElement, const _uint& iElementNum)
{
	_uint iHlslFlag = 0;

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	/*
		- 디버그일 때 컴파일 최적화를 하지 않는다.
	*/
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
	/*
		- 디버그가 아닐 때 최적화를 가장 기본적인 단계로 한다.
	*/
#endif

	/*여기서 쉐이더 파일 경로를 가져와서 effect 변수를 만들어 준다.*/
	if (FAILED(D3DX11CompileEffectFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;
	// effect 객체를 만들어준다.

	ID3DX11EffectTechnique*	pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (pTechnique == nullptr)
		return E_FAIL;
	pTechnique->GetDesc(&m_Technique);
	// technique에 정보를 받아와서 구조체 변수에 저장
	
	for (_uint i = 0; i < m_Technique.Passes; i++) {
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);
		if (pPass == nullptr)
			return E_FAIL;

		D3DX11_PASS_DESC	PassDesc;

		pPass->GetDesc(&PassDesc);

		ID3D11InputLayout* pInputLayout = nullptr;

		if (FAILED(m_pDevice->CreateInputLayout(pElement, iElementNum, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;

		m_vecInputLayout.push_back(pInputLayout);
	} // pass들의 정보들을 받아와서 Inputlayout객체를 생성해 벡터에 저장

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_Technique.Passes)
		return E_FAIL;

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (pTechnique == nullptr)
		return E_FAIL;

	ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(iPassIndex);
	if (pPass == nullptr)
		return E_FAIL;

	pPass->Apply(0, m_pContext);
	
	m_pContext->IASetInputLayout(m_vecInputLayout[iPassIndex]);

	return S_OK;
}

HRESULT CShader::Bind_Matrix(const _char* pMatrixName, const _float4x4* pMatrix)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pMatrixName);
	if (pVariable == nullptr)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (pMatrixVariable == nullptr)
		return E_FAIL;

	pMatrixVariable->SetMatrix((_float*)pMatrix);

	return S_OK;
}

HRESULT CShader::Bind_Matrixes(const _char* pMatrixName, const _float4x4* pMatrix, const _uint& iMatrixNum)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pMatrixName);
	if (pVariable == nullptr)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (pMatrixVariable == nullptr)
		return E_FAIL;

	pMatrixVariable->SetMatrixArray((_float*)pMatrix,0, iMatrixNum);

	return S_OK;
}

HRESULT CShader::Bind_SRV(const _char* pTextureName, ID3D11ShaderResourceView* pSRV)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pTextureName);
	if (pVariable == nullptr)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pShaderVariable = pVariable->AsShaderResource();
	if (pShaderVariable == nullptr)
		return E_FAIL;

	pShaderVariable->SetResource(pSRV);

	return S_OK;
}

HRESULT CShader::Bind_SRVS(const _char* pTextureName, ID3D11ShaderResourceView** pSRV, const _uint& iSRVNum)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pTextureName);
	if (pVariable == nullptr)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pShaderVariable = pVariable->AsShaderResource();
	if (pShaderVariable == nullptr)
		return E_FAIL;

	pShaderVariable->SetResourceArray(pSRV, 0, iSRVNum);

	return S_OK;
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElement, const _uint& iElementNum)
{
	CShader* pInstance = new CShader(pDevice, pContext);


	if (FAILED(pInstance->Initialize_ProtoType(strShaderFilePath, pElement, iElementNum))) {
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Cloned : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	for (auto& iter : m_vecInputLayout)
		Safe_Release(iter);

	Safe_Release(m_pEffect);
}
