#include "stdafx.h"
#include "..\Public\DataMgr.h"

#include "GameInstance.h"
#include "GameObject.h"

#include "Plateform.h"
#include "Character.h"

IMPLEMENT_SINGLETON(CDataMgr)

CDataMgr::CDataMgr()
    :m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CDataMgr::Initialize()
{


    return S_OK;
}

HRESULT CDataMgr::Level_Object_Load(const _char* strFilePath)
{
	ifstream fIn;

	fIn.open(strFilePath, std::ios::binary);

	if (fIn.is_open())
	{
		_uint iSize;
		fIn.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));

		for (_uint i = 0; i < iSize; i++) {

			_bool bAnim;
			fIn.read(reinterpret_cast<char*>(&bAnim), sizeof(_bool));

			size_t LayerNameSize = {};
			fIn.read(reinterpret_cast<char*>(&LayerNameSize), sizeof(size_t));
			string strLayerName;
			strLayerName.resize(LayerNameSize);
			fIn.read(&strLayerName[0], LayerNameSize);

			size_t ProTagNameSize = {};
			fIn.read(reinterpret_cast<char*>(&ProTagNameSize), sizeof(size_t));
			string strProTagName;
			strProTagName.resize(ProTagNameSize);
			fIn.read(&strProTagName[0], ProTagNameSize);

			size_t ModelNameSize = {};
			fIn.read(reinterpret_cast<char*>(&ModelNameSize), sizeof(size_t));
			string strModelName;
			strModelName.resize(ModelNameSize);
			fIn.read(&strModelName[0], ModelNameSize);

			_int iNaviIndex;
			fIn.read(reinterpret_cast<char*>(&iNaviIndex), sizeof(_int));

			_float4x4 matWorld;
			fIn.read(reinterpret_cast<char*>(&matWorld), sizeof(_float4x4));

			CGameObject* pObject_Demo = nullptr;

			if (bAnim) {
				CCharacter::CHARACTER_DESC Character_Desc = {};

				Character_Desc.fRotationPerSec = XMConvertToRadians(90.f);
				Character_Desc.fSpeedPerSec = 5.f;
				//CUtility_String::string_To_Wstring(strModelName);
				Character_Desc.strModelTag = wstring(strModelName.begin(), strModelName.end());

				if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level()
					, CUtility_String::string_To_Wstring(strLayerName),
					CUtility_String::string_To_Wstring(strProTagName),
					&Character_Desc, reinterpret_cast<CGameObject**>(&pObject_Demo))))
					return E_FAIL;
			}
			else {
				CPlateform::PLATEFORM_DESC Plateform_Desc = {};
				Plateform_Desc.strModelTag = CUtility_String::string_To_Wstring(strModelName);
				Plateform_Desc.vPos = _float4(0.f, 0.f, 0.f, 1.f);

				if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level()
					, CUtility_String::string_To_Wstring(strLayerName),
					CUtility_String::string_To_Wstring(strProTagName),
					&Plateform_Desc, reinterpret_cast<CGameObject**>(&pObject_Demo))))
					return E_FAIL;
			}

			pObject_Demo->Get_Component<CTransform>()->Set_WorldMatrix(matWorld);

			if (iNaviIndex != -1)
			{
				pObject_Demo->Get_Component<CNavigation>()->Set_CurrentIndex(iNaviIndex);
			}
		}
	}
	else
		return E_FAIL;

	fIn.close();

    return S_OK;
}

void CDataMgr::Free()
{
    __super::Free();
    Safe_Release(m_pGameInstance);
}
