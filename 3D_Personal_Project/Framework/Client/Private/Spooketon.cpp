#include "stdafx.h"
#include "..\Public\Spooketon.h"

#include "NorMonster_IDLE.h"

CSpooketon::CSpooketon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CSpooketon::CSpooketon(const CSpooketon& rhs)
	:CMonster(rhs)
{
}

HRESULT CSpooketon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpooketon::Initialize(void* pArg)
{
	m_strModelTag = ANIMMODEL_SPOOKETON_TAG;

	CGameObject::GAMEOBJECT_DESC GameObject_Desc = {};

	GameObject_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	GameObject_Desc.fSpeedPerSec = 5.f;

	if (FAILED(CMonster::Initialize(&GameObject_Desc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, XMVectorSet(1.f, 0.f, 1.f, 1.f));

	return S_OK;
}

void CSpooketon::Priority_Tick(_float fTimeDelta)
{
	CMonster::Priority_Tick(fTimeDelta);
}

void CSpooketon::Tick(_float fTimeDelta)
{
	CMonster::Tick(fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta, true);
}

void CSpooketon::Late_Tick(_float fTimeDelta)
{
	CMonster::Late_Tick(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CSpooketon::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(CMonster::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpooketon::Bind_ShaderResources()
{
	if (FAILED(CMonster::Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpooketon::Ready_Component()
{
	if (FAILED(CMonster::Ready_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSpooketon::Ready_State()
{
	if (FAILED(m_pStateMachineCom->Add_State(STATE::IDLE, CNorMonster_IDLE::Create(this)))) return E_FAIL;

	if (FAILED(m_pStateMachineCom->Init_State(STATE::IDLE)))
		return E_FAIL;

	return S_OK;
}

CSpooketon* CSpooketon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSpooketon* pInstance = new CSpooketon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSpooketon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSpooketon::Clone(void* pArg)
{
	CSpooketon* pInstance = new CSpooketon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSpooketon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSpooketon::Free()
{
	__super::Free();
}
