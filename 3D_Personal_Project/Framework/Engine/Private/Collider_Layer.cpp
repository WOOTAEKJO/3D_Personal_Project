#include "..\Public\Collider_Layer.h"

CCollider_Layer::CCollider_Layer()
{
}

HRESULT CCollider_Layer::Add_Collider(CCollider* pCollider)
{
    if (pCollider == nullptr)
        return E_FAIL;

    m_listCollider.push_back(pCollider);

    return S_OK;
}

CCollider_Layer* CCollider_Layer::Create()
{
	return new CCollider_Layer;
}

void CCollider_Layer::Free()
{
    __super::Free();
}
