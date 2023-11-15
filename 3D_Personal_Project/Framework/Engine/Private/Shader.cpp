#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice, pContext)
{
}

CShader::CShader(const CShader& rhs)
	:CComponent(rhs),
	m_pEffect(rhs.m_pEffect)
{
	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_ProtoType(const wstring& strShaderFilePath)
{
	/*여기서 쉐이더 파일 경로를 가져와서 effect 변수를 만들어 준다.*/

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strShaderFilePath)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_ProtoType(strShaderFilePath))) {
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg))) {
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	Safe_Release(m_pEffect);
}
