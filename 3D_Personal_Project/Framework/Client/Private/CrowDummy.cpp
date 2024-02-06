#include "stdafx.h"
#include "CrowDummy.h"

#include "GameInstance.h"

#include "Bone.h"
#include "Player.h"

#include "Monster.h"

#include "Utility_Effect.h"
#include "Effect_Trail.h"

CCrowDummy::CCrowDummy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CNPC(pDevice, pContext)
{
}

CCrowDummy::CCrowDummy(const CCrowDummy& rhs)
	: CNPC(rhs)
{
}

HRESULT CCrowDummy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCrowDummy::Initialize(void* pArg)
{
	CCrowDummy::CHARACTER_DESC Crow_Desc = {};
	Crow_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Crow_Desc.fSpeedPerSec = 5.f;
	Crow_Desc.strModelTag = ANIMMODEL_CROW_TAG;

	if (FAILED(CNPC::Initialize(&Crow_Desc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(0.16f, 0.16f, 0.16f);

	m_pModelCom->Set_AnimationIndex(0);

	return S_OK;
}

void CCrowDummy::Priority_Tick(_float fTimeDelta)
{
	
	CNPC::Priority_Tick(fTimeDelta);
}

void CCrowDummy::Tick(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta, true);
	CNPC::Tick(fTimeDelta);
}

void CCrowDummy::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	CNPC::Late_Tick(fTimeDelta);
	
}

HRESULT CCrowDummy::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (size_t i = 0; i < iNumMeshs; i++)
	{
		if (FAILED(m_pModelCom->Bind_Blend(m_pShaderCom, "g_BlendMatrix", i)))
			return E_FAIL;

		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(2);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CCrowDummy::Bind_ShaderResources()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_matWorld", &m_pTransformCom->Get_WorldMatrix_Float4x4() )))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;

	_uint2 iIndx;
	if (m_eCurrentTypeAnimIndex == (_int)CNPC::STATE::FOLLOW || m_eCurrentTypeAnimIndex == (_int)CNPC::STATE::ATTACK)
		iIndx = { 1000,1000 };
	else
		iIndx = { 6,9 };
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_iDiscardIndx", &iIndx, sizeof(_uint2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_bHited", &m_bHit_Effect, sizeof(_bool))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCrowDummy::Ready_Component()
{
	/*CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iCurrentIndex = m_pPlayer->Get_Component<CNavigation>()->Get_CurrentIndex();
	if (FAILED(Add_Component<CNavigation>(m_pGameInstance->Get_CurNavigationTag(), &m_pNavigationCom, &NavigationDesc))) return E_FAIL;*/

	if (FAILED(Add_Component<CShader>(SHADER_ANIMMESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(ANIMMODEL_CROW_TAG, &m_pModelCom))) return E_FAIL;

	return S_OK;
}

CCrowDummy* CCrowDummy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCrowDummy* pInstance = new CCrowDummy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCrowDummy");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CCrowDummy::Clone(void* pArg)
{
	CCrowDummy* pInstance = new CCrowDummy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCrowDummy");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCrowDummy::Free()
{
	__super::Free();
}
