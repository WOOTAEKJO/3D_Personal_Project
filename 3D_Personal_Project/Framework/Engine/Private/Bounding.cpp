#include "..\Public\Bounding.h"

CBounding::CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice),m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_bool CBounding::Colision(TYPE eType,CCollider* pTargetCollider)
{

	switch (eType)
	{
	case Engine::CBounding::TYPE_AABB:
		break;
	case Engine::CBounding::TYPE_OBB:
		break;
	case Engine::CBounding::TYPE_SPHERE:
		break;
	}

	return _bool();
}

void CBounding::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
