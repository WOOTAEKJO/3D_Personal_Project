#include "stdafx.h"
#include "..\Public\Spooketon.h"

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

HRESULT CSpooketon::Ready_Tree()
{
	CAICom::AIDESC AIDesc = {};

	CNode* pNodes[] = {
		/*CBuilder().Leaf<CTestAction>().Build(),
		CBuilder().Composite<CSequence>().Leaf<CTestAction>().End().Build(),
		CBuilder().Decorator<CSucceeder>().Leaf<CTestAction>().End().Build()*/
	};

	wstring	pString[] = {
		TEXT("AI1"),TEXT("AI2"),TEXT("AI3")
	};

	AIDesc.ppNodes = pNodes;
	AIDesc.pstrNodeTags = pString;
	AIDesc.iNodeNum = 3;

	/* For.Com_AI*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_AI"),
		TEXT("Com_AI"), reinterpret_cast<CComponent**>(&m_pAICom), &AIDesc)))
		return E_FAIL;

	m_pAICom->Set_Tree(TEXT("AI1"));

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
