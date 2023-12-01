#include "stdafx.h"
#include "..\Public\Terrain_Demo.h"
#include "GameInstance.h"

#include "../Public/ImGuiMgr.h"

#include "Terrain_Window.h"

CTerrain_Demo::CTerrain_Demo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CDemo(pDevice, pContext)
{
}

CTerrain_Demo::CTerrain_Demo(const CTerrain_Demo& rhs)
	: CDemo(rhs)
{
}

HRESULT CTerrain_Demo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain_Demo::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CTerrain_Demo::Priority_Tick(_float fTimeDelta)
{
}

void CTerrain_Demo::Tick(_float fTimeDelta)
{
	if (m_pVIBufferCom == nullptr)
		return;

	CImGuiMgr::GetInstance()->Tick();
	
	Update_Mouse();

	CTerrain_Window::TERRAINWINDOWDESC TerrainWindowDesc;
	TerrainWindowDesc.bPicked = m_bPicked;
	TerrainWindowDesc.vPickPos = m_vMouseWorldPos;
	CImGuiMgr::GetInstance()->Window_Set_Variable(CImGuiMgr::MODE_TERRAIN, &TerrainWindowDesc);
}

void CTerrain_Demo::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	m_bPicked = false;
}

HRESULT CTerrain_Demo::Render()
{

	if (m_pVIBufferCom != nullptr) {

		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Bind_Buffer();

		if (m_bWireFrame)
			m_pShaderCom->Set_RenderState(CShader::RENDERSTATE::RS_WIREFRAME);
		else
			m_pShaderCom->Set_RenderState(CShader::RENDERSTATE::RS_SOLID);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CTerrain_Demo::Create_DynamicBuffer(_uint iVerticesXNum, _uint iVerticesZNum)
{
	if (m_pVIBufferCom == nullptr) {
		CVIBuffer_DTerrain::DTERRAINDESC tDTerrainDesc;

		tDTerrainDesc.iVerticesXNum = iVerticesXNum;
		tDTerrainDesc.iVerticesZNum = iVerticesZNum;

		/* For.Com_VIBuffer*/
		if (FAILED(Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_VIBuffer_DTerrain"),
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &tDTerrainDesc)))
			return E_FAIL;
	}
	else {
		if (FAILED(Delete_Component(TEXT("Com_VIBuffer"))))
			return E_FAIL;

		Safe_Release(m_pVIBufferCom);

		CVIBuffer_DTerrain::DTERRAINDESC tDTerrainDesc;

		tDTerrainDesc.iVerticesXNum = iVerticesXNum;
		tDTerrainDesc.iVerticesZNum = iVerticesZNum;

		/* For.Com_VIBuffer*/
		if (FAILED(Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_VIBuffer_DTerrain"),
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &tDTerrainDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CTerrain_Demo::Set_Control_Variable(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	TERRAINDEMOVALUE* pVariable = (TERRAINDEMOVALUE*)pArg;

	m_fRadius = pVariable->fRadius;
	m_fHeight = pVariable->fHeight;
	m_fSharpness = pVariable->fSharpness;
	m_bWireFrame = pVariable->bWireFrame;

	return S_OK;
}

HRESULT CTerrain_Demo::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_BRUSH]->Bind_ShaderResource(m_pShaderCom, "g_BrushTexture")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_CamWorldPos", &m_pGameInstance->Get_Camera_Pos(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bWireFrame", &m_bWireFrame, sizeof(bool))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBrushPos", &m_vMouseWorldPos, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fBrushRange", &m_fRadius, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Demo::Ready_Component()
{
	
	/* For.Com_Shader*/
	if (FAILED(Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VTXTBN"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture*/
	if (FAILED(Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_DIFFUSE]))))
		return E_FAIL;

	/* For.Com_Mask*/
	if (FAILED(Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Terrain_Mask"),
		TEXT("Com_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_MASK]))))
		return E_FAIL;

	/* For.Com_Brush*/
	if (FAILED(Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Terrain_Brush"),
		TEXT("Com_Brush"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BRUSH]))))
		return E_FAIL;

	return S_OK;
}

void CTerrain_Demo::Update_Mouse()
{
	_float3	vMousePos = {};

	m_pGameInstance->Update_Mouse();
	m_pVIBufferCom->Compute_MousePos(&vMousePos, m_pTransformCom->Get_WorldMatrix_Matrix());

	if (vMousePos.x == 0)
		return;

	if (m_pGameInstance->Mouse_Down(DIM_LB)) {

		//m_pVIBufferCom->Update_Buffer(XMLoadFloat3(&vMousePos), m_fRadius, m_fHeight, m_fSharpness);
		//Create_Mark();
		m_bPicked = true;
	}

	XMStoreFloat4(&m_vMouseWorldPos, XMVector3TransformCoord(XMLoadFloat3(&vMousePos), m_pTransformCom->Get_WorldMatrix_Matrix()));
}

HRESULT CTerrain_Demo::Create_Mark()
{
	if (m_pMark == nullptr) {
		if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL, TEXT("Tool"), TEXT("Prototype_GameObject_GameObject_Mark"),
			nullptr, reinterpret_cast<CGameObject**>(&m_pMark))))
			return E_FAIL;
	}
	else {

	}

	return S_OK;
}

CTerrain_Demo* CTerrain_Demo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain_Demo* pInstance = new CTerrain_Demo(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CTerrain_Demo::Clone(void* pArg)
{
	CTerrain_Demo* pInstance = new CTerrain_Demo(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain_Demo");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTerrain_Demo::Free()
{
	__super::Free();

	for (_uint i = 0; i < TYPE_END; i++)
	{
		Safe_Release(m_pTextureCom[i]);
	}
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMark);
}
