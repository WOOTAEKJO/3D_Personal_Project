#include "stdafx.h"
#include "..\Public\Terrain_Demo.h"

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
	m_pNavigationCom->Update(m_pTransformCom->Get_WorldMatrix_Float4x4());
}

void CTerrain_Demo::Tick(_float fTimeDelta)
{
	if (m_pVIBufferCom == nullptr)
		return;

	CImGuiMgr::GetInstance()->Tick();
	
}

void CTerrain_Demo::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
	if (FAILED(m_pGameInstance->Add_DebugRender(m_pNavigationCom)))
		return;

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
		if (FAILED(Add_Component(LEVEL_TOOL, BUFFER_DTERRAIN_TAG,
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
		if (FAILED(Add_Component(LEVEL_TOOL, BUFFER_DTERRAIN_TAG,
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
	
	if (FAILED(Add_Component<CShader>(SHADER_TERRAIN_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(TEX_TERRAIN_TAG, &m_pTextureCom[TYPE_DIFFUSE]))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(TEX_TERRAIN_MASK_TAG, &m_pTextureCom[TYPE_MASK],nullptr,1))) return E_FAIL;
	if (FAILED(Add_Component<CTexture>(TEX_TERRAIN_BRUSH_TAG, &m_pTextureCom[TYPE_BRUSH], nullptr,2))) return E_FAIL;
	if (FAILED(Add_Component<CNavigation>(COM_NAVIGATION_DEMO_TAG, &m_pNavigationCom))) return E_FAIL;

	///* For.Com_Texture*/ 
	//if (FAILED(Add_Component(LEVEL_TOOL, TEX_TERRAIN_TAG,
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_DIFFUSE]))))
	//	return E_FAIL;

	///* For.Com_Mask*/ 
	//if (FAILED(Add_Component(LEVEL_TOOL, TEX_TERRAIN_MASK_TAG,
	//	TEXT("Com_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_MASK]))))
	//	return E_FAIL;

	///* For.Com_Brush*/ 
	//if (FAILED(Add_Component(LEVEL_TOOL, TEX_TERRAIN_BRUSH_TAG,
	//	TEXT("Com_Brush"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BRUSH]))))
	//	return E_FAIL;

	///* For.Com_Navigation*/
	//if (FAILED(Add_Component(LEVEL_TOOL, COM_NAVIGATION_DEMO_TAG,
	//	TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom))))
	//	return E_FAIL;

	return S_OK;
}

_bool CTerrain_Demo::Update_Mouse(_float4* fPickPoint, _bool bPressing)
{
	if (m_pVIBufferCom == nullptr)
		return false;

	_float3	vMousePos = {};

	m_pGameInstance->Update_Mouse();
	m_pVIBufferCom->Compute_MousePos(&vMousePos, m_pTransformCom->Get_WorldMatrix_Matrix());

	if (vMousePos.x == 0)
		return false; 

	XMStoreFloat4(fPickPoint, XMVector3TransformCoord(XMLoadFloat3(&vMousePos), m_pTransformCom->Get_WorldMatrix_Matrix()));
	m_vMouseWorldPos = *fPickPoint;

	if (bPressing)
	{
		return true;
	}
	else {
		if (m_pGameInstance->Mouse_Down(DIM_LB)) {
			return true;
		}
	}

	

	return false;
}

void CTerrain_Demo::Update_HeightMap(_fvector vPickPos, _float fRadius, _float fHeight, _float fSharpness)
{
	if (m_pVIBufferCom == nullptr)
		return;

	m_pVIBufferCom->Update_Buffer(vPickPos, fRadius, fHeight, fSharpness);
}

HRESULT CTerrain_Demo::Save_Terrain(const _char* strPath)
{
	if (m_pVIBufferCom == nullptr)
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Set_Buffer(strPath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Demo::Load_Terrain(const _char* strPath)
{
	if (m_pVIBufferCom == nullptr) {
		
		if (FAILED(Add_Component(LEVEL_TOOL, BUFFER_DTERRAIN_TAG,
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
			return E_FAIL;
	}
	else {
		if (FAILED(Delete_Component(TEXT("Com_VIBuffer"))))
			return E_FAIL;

		Safe_Release(m_pVIBufferCom);

		if (FAILED(Add_Component(LEVEL_TOOL, BUFFER_DTERRAIN_TAG,
			TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Load_Data_Mesh(m_pVIBufferCom, strPath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Demo::Add_Navigation_Cell(_float3* pPoints, _uint* iCellIndex, _uint iCellType)
{
	if (m_pNavigationCom == nullptr)
		return E_FAIL;

    return m_pNavigationCom->Add_Cell(pPoints, iCellIndex, iCellType);
}

HRESULT CTerrain_Demo::Save_Navigation(const _char* strPath)
{
	if (m_pNavigationCom == nullptr)
		return E_FAIL;

	if (FAILED(m_pNavigationCom->Save_Navigation(strPath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain_Demo::Load_Navigation(const _char* strPath)
{
	if (m_pNavigationCom == nullptr)
		return E_FAIL;

	if (FAILED(m_pNavigationCom->File_Load(strPath)))
		return E_FAIL;

	return S_OK;
}

void CTerrain_Demo::Update_Navigation_Cell(_uint iCellIndex, FLOAT3X3 vPositions)
{
	if (m_pNavigationCom == nullptr)
		return;

	m_pNavigationCom->Update_Buffer(iCellIndex, vPositions);
}

void CTerrain_Demo::All_Delete_Cell()
{
	if (m_pNavigationCom == nullptr)
		return;

	m_pNavigationCom->All_Delete_Cell();
}

void CTerrain_Demo::Selected_Delete_Cell(_uint iCellIndex)
{
	if (m_pNavigationCom == nullptr)
		return;

	m_pNavigationCom->Delete_Cell(iCellIndex);
}

void CTerrain_Demo::Set_Cell_Type(_uint iCellType, _uint iIndex)
{
	if (m_pNavigationCom == nullptr)
		return;

	m_pNavigationCom->Set_Cell_Type(iCellType, iIndex);
}

void CTerrain_Demo::Add_Neighbor(_uint iSourCellIndx, _float3* vSourPoints, _uint iDestCellIndx, _float3* vDestPoints)
{
	if (m_pNavigationCom == nullptr)
		return;

	m_pNavigationCom->Add_Neighbor(iSourCellIndx, vSourPoints, iDestCellIndx, vDestPoints);
}

_bool CTerrain_Demo::Picked_Cell(_uint* iCellIndex)
{
	if (m_pNavigationCom == nullptr)
		return false;

	return m_pNavigationCom->Compute_MousePos(iCellIndex);
}

vector<CCell*> CTerrain_Demo::Get_Navigation_Cells()
{
	return m_pNavigationCom->Get_Navigation_Cells();
}

void CTerrain_Demo::Set_Add(_bool bCheck)
{
	if (m_pVIBufferCom == nullptr)
		return;

	m_pVIBufferCom->Set_Add(bCheck);
}

void CTerrain_Demo::Init_Neighbor()
{
	if (m_pNavigationCom == nullptr)
		return;

	m_pNavigationCom->Init_Neighbor();
}

CTerrain_Demo* CTerrain_Demo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain_Demo* pInstance = new CTerrain_Demo(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
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

	/* 원형객체를 초기화한다.  */
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

	for (_uint i = 0; i < (_uint)TEXTURE::TYPE_END; i++)
	{
		Safe_Release(m_pTextureCom[i]);
	}
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
