#include "stdafx.h"
#include "..\Public\DataMgr.h"

#include "GameInstance.h"
#include "GameObject.h"

#include "Plateform.h"
#include "Plateform_Instancing.h"
#include "Character.h"

#include "PuzzleMgr.h"

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
				/*if(!wcscmp(CUtility_String::string_To_Wstring(strProTagName).c_str(), GO_PLATEFORM_TRAP_TAG))
					int a = 0;*/
				
				CPlateform::PLATEFORM_DESC Plateform_Desc = {};
				Plateform_Desc.strModelTag = CUtility_String::string_To_Wstring(strModelName);
				Plateform_Desc.vPos = _float4(0.f, 0.f, 0.f, 1.f);

				if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level()
					, CUtility_String::string_To_Wstring(strLayerName),
					CUtility_String::string_To_Wstring(strProTagName),
					&Plateform_Desc, reinterpret_cast<CGameObject**>(&pObject_Demo))))
					return E_FAIL;

				if (!wcscmp(CUtility_String::string_To_Wstring(strProTagName).c_str(), GO_PUZZLE_TAG))
				{
					CPuzzleMgr::GetInstance()->Add_Puzzle(pObject_Demo);
				}
			}

			pObject_Demo->Get_Component<CTransform>()->Set_WorldMatrix(matWorld);

			if (0 <= iNaviIndex)
			{
				pObject_Demo->Get_Component<CNavigation>()->Set_CurrentIndex(iNaviIndex);
			}
			else if (iNaviIndex == -3 || iNaviIndex == -4)
			{
				dynamic_cast<CPlateform*>(pObject_Demo)->Set_TriggerNum(iNaviIndex);
				// 임시니깐 다음부터 이렇게 만들지 말자
				// 시간 없다
			}
		}
	}
	else
		return E_FAIL;

	fIn.close();

    return S_OK;
}

HRESULT CDataMgr::Level_Object_Instancing_Load(const _char* strFilePath)
{
	ifstream fIn;

	fIn.open(strFilePath, std::ios::binary);

	if (fIn.is_open())
	{
		_uint iSize;
		fIn.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));

		for (_uint i = 0; i < iSize; i++) {

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

			vector <_float4x4> vecIsntanceVertex;
			_uint iInstanceVertexSize = 0;
			fIn.read(reinterpret_cast<char*>(&iInstanceVertexSize), sizeof(_uint));
			vecIsntanceVertex.reserve(iInstanceVertexSize);

			for (_uint j = 0; j < iInstanceVertexSize; j++)
			{
				_float4x4 matVertex;
				fIn.read(reinterpret_cast<char*>(&matVertex), sizeof(_float4x4));
				vecIsntanceVertex.push_back(matVertex);
			}

			CGameObject* pObject_Demo = nullptr;

			CPlateform_Instancing::PLATEFORM_INSTANCING_DESC Plateform_Desc = {};
			Plateform_Desc.strModelTag = CUtility_String::string_To_Wstring(strModelName);
			Plateform_Desc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			Plateform_Desc.vecInstanceVertex = vecIsntanceVertex;



			if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level()
				, CUtility_String::string_To_Wstring(strLayerName),
				CUtility_String::string_To_Wstring(strProTagName),
				&Plateform_Desc, reinterpret_cast<CGameObject**>(&pObject_Demo))))
				return E_FAIL;

			pObject_Demo->Get_Component<CTransform>()->Set_WorldMatrix(matWorld);

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
