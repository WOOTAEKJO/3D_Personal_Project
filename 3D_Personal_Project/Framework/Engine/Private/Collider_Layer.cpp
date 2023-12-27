#include "..\Public\Collider_Layer.h"
#include "GameObject.h"

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

void CCollider_Layer::Delete_Collider()
{
    for (auto& iter = m_listCollider.begin(); iter != m_listCollider.end();)
    {
        if ((*iter)->Get_Owner()->Get_Dead())
        {
            iter = m_listCollider.erase(iter);
        }
        else {
            iter++;
        }
    }
}

CCollider_Layer* CCollider_Layer::Create()
{
	return new CCollider_Layer;
}

void CCollider_Layer::Free()
{
    __super::Free();
}
