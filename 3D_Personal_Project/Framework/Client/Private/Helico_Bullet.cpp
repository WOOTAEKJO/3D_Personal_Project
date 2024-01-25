#include "stdafx.h"
#include "..\Public\Helico_Bullet.h"

#include "GameInstance.h"
#include "Character.h"

#include "Utility_Effect.h"

CHelico_Bullet::CHelico_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBullet(pDevice, pContext)
{
}

CHelico_Bullet::CHelico_Bullet(const CHelico_Bullet& rhs)
	: CBullet(rhs)
{
}

HRESULT CHelico_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHelico_Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	BULLET_DESC* BulletDesc = (BULLET_DESC*)pArg;

	if (FAILED(m_pGameInstance->Add_Collision(BulletDesc->eCollider_Layer, m_pColliderCom)))
		return E_FAIL;

	

	return S_OK;
}

void CHelico_Bullet::Priority_Tick(_float fTimeDelta)
{
	Toward(fTimeDelta);
	__super::Priority_Tick(fTimeDelta);
}

void CHelico_Bullet::Tick(_float fTimeDelta)
{
	/*if (m_bParticle)
	{
		CUtility_Effect::Create_Particle_Normal(m_pGameInstance, PARTICLE_BOSS1AIRATTACK_TAG, GO_PARTICLESUB_TAG,
			this, nullptr, 1.f);

		m_bParticle = false;
	}*/

	__super::Tick(fTimeDelta);
}

void CHelico_Bullet::Late_Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc > 0.05f)
	{
		m_fTimeAcc = 0.f;

		_uint iNum = rand() % 5 + 1;

		wstring strTag = TEXT("Prototype_Component_Particle_Boss1AirAttack") + to_wstring(iNum);

		CUtility_Effect::Create_Particle_Normal(m_pGameInstance, strTag, GO_PARTICLESUB_TAG,
			this, nullptr, 0.3f);
	}
		

	/*if (XMVector3NearEqual(XMLoadFloat4(&m_vTargetPos), m_pTransformCom->Get_State(CTransform::STATE::STATE_POS),
		XMVectorSet(0.1f, 0.1f, 0.1f, 0.f)))
		Set_Dead();*/

	if (XMVector3NearEqual(XMVectorSet(0.f, 1.95f, 0.f, 1.f), m_pTransformCom->Get_State(CTransform::STATE::STATE_POS),
		XMVectorSet(100.f, 0.1f, 100.f, 0.f)))
	{
		CUtility_Effect::Create_Damage_Effect(m_pGameInstance, this, 0.2f, _float2(0.15f, 0.15f));
		CUtility_Effect::Create_Particle_Normal(m_pGameInstance, PARTICLE_BOSS1AIRATTACKEND_TAG,
			GO_PARTICLENORMAL_TAG,
			this, nullptr, 0.5f);
		Set_Dead();
	}
		
	/*if(m_pTransformCom->Get_State(CTransform::STATE::STATE_POS).m128_f32[0] <= 2.f)
		Set_Dead();*/

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

	__super::Late_Tick(fTimeDelta);
}

HRESULT CHelico_Bullet::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshs = m_pModelCom->Get_MeshesNum();

	for (_uint i = 0; i < iNumMeshs; i++)
	{
		m_pModelCom->Bind_ShaderResources(m_pShaderCom, "g_DiffuseTexture", i, TEXTURETYPE::TYPE_DIFFUSE);

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	if(FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CHelico_Bullet::Toward(_float fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vLook = XMLoadFloat3(&m_vLook);

	vPos = vPos + vLook * fTimeDelta * m_fSpeed;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POS, vPos);
	LookAt_Y();
}

void CHelico_Bullet::LookAt_Y()
{
	_float3 fScale = m_pTransformCom->Get_Scaled();

	//_vector	vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);

	_vector vLook = XMVector3Normalize(XMLoadFloat3(&m_vLook)) * fScale.y;
	_vector	vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * fScale.x;
	_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * fScale.z;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE::STATE_UP, -vLook);
	m_pTransformCom->Set_State(CTransform::STATE::STATE_LOOK, vUp);
}

HRESULT CHelico_Bullet::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_matWorld")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matView", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_matProj", &m_pGameInstance
		->Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE::PROJ))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CHelico_Bullet::Ready_Component()
{
	if (FAILED(CBullet::Ready_Component()))
		return E_FAIL;

	if (FAILED(Add_Component<CShader>(SHADER_MESH_TAG, &m_pShaderCom))) return E_FAIL;
	if (FAILED(Add_Component<CModel>(MODEL_HELICOBULLET_TAG, &m_pModelCom))) return E_FAIL;

	return S_OK;
}

CHelico_Bullet* CHelico_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHelico_Bullet* pInstance = new CHelico_Bullet(pDevice, pContext);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHelico_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHelico_Bullet::Clone(void* pArg)
{
	CHelico_Bullet* pInstance = new CHelico_Bullet(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHelico_Bullet");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHelico_Bullet::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
