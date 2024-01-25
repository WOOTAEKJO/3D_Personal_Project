#include "..\Public\AlphaObject.h"

CAlphaObject::CAlphaObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CAlphaObject::CAlphaObject(const CAlphaObject& rhs)
	:CGameObject(rhs)
{
}

HRESULT CAlphaObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAlphaObject::Initialize(void* pArg)
{
	if(FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CAlphaObject::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CAlphaObject::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CAlphaObject::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CAlphaObject::Render()
{
	return S_OK;
}

void CAlphaObject::Compute_CamDistance()
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POS);
	_vector vCamPos = m_pGameInstance->Get_CameraState_Mat(CPipeLine::CAMERASTATE::CAM_POS);

	m_fCamDistance =  XMVectorGetX(XMVector3Length(vCamPos - vPos));
}

void CAlphaObject::Free()
{
	__super::Free();
}
