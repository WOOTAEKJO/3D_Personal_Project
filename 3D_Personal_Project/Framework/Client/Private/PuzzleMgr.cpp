#include "stdafx.h"
#include "..\Public\PuzzleMgr.h"

#include "GameObject.h"
#include "GameInstance.h"

#include "Puzzle.h"

#include "Trigger.h"

IMPLEMENT_SINGLETON(CPuzzleMgr)

CPuzzleMgr::CPuzzleMgr()
	:m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CPuzzleMgr::Initialize()
{
	return S_OK;
}

void CPuzzleMgr::Add_Puzzle(CGameObject* pPuzzle)
{
	if (pPuzzle == nullptr)
		return;

	m_vecPuzzle.push_back(dynamic_cast<CPuzzle*>(pPuzzle));

	//m_vecPuzzle.back()->Set_LightColor(Select_Color(m_vecPuzzle.back()->Get_ModelTag()));
	
	m_vecPuzzle.back()->Set_LightColor(Select_Color(m_iPuzzleNum));
	m_vecPuzzle.back()->Set_PuzzleIndx(m_iPuzzleNum);

	++m_iPuzzleNum;
}

void CPuzzleMgr::Tick()
{
	if (m_bFinish)
		return;

	if (m_iMatch[0] == -1 || m_iMatch[1] == -1)
	{
		for (auto& iter : m_vecPuzzle)
		{
			if (!iter->Get_True())
			{
				if (iter->Get_Hit() && m_iMatch[0] != iter->Get_PuzzleIndx())
				{
					if (m_iMatch[0] == -1)
					{
						m_iMatch[0] = iter->Get_PuzzleIndx();
					}
					else if (m_iMatch[1] == -1 && iter->Get_PuzzleIndx() != m_iMatch[0])
						m_iMatch[1] = iter->Get_PuzzleIndx();
				}
			}
		}
	}
	else {
		if (Compare_Indx(m_iMatch[0], m_iMatch[1]))
		{
			for (auto& iter : m_vecPuzzle)
			{
				if (iter->Get_PuzzleIndx() == m_iMatch[0] || iter->Get_PuzzleIndx() == m_iMatch[1])
				{
					iter->Set_True(true);
				}

			}

			if (m_iSuccessNum == 6)
			{
				m_bFinish = true;

				

				CTrigger::TRIGGER_DESC TriggerDesc = {};
				TriggerDesc.strEventName = TEXT("Portal_Boss1");
				TriggerDesc.vPosition = _float4(46.f, 9.f, 34.f, 1.f);
				TriggerDesc.vScale = _float3(1.f, 1.f, 1.f);

				if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_PLATEFORM]
					, GO_TRIGGER_TAG, &TriggerDesc)))
					return;

				return;
			}

			++m_iSuccessNum;

			m_iMatch[0] = -1;
			m_iMatch[1] = -1;
		}
		else {
			for (auto& iter : m_vecPuzzle)
			{
				if (iter->Get_PuzzleIndx() == m_iMatch[0] || iter->Get_PuzzleIndx() == m_iMatch[1])
				{
					iter->Set_Hit(false);
				}
			}

			m_iMatch[0] = -1;
			m_iMatch[1] = -1;

		}
	}
	
}

_float4 CPuzzleMgr::Select_Color(const wstring& strModelTag)
{
	if (CUtility_String::Compare_WString(strModelTag, MODEL_ENVY_TAG))
	{
		return _float4(1.f,0.f,0.f,1.f);
	}
	else if (CUtility_String::Compare_WString(strModelTag, MODEL_GLUTTONY_TAG))
	{
		return _float4(1.f,0.3f,0.f,1.f);
	}
	else if (CUtility_String::Compare_WString(strModelTag, MODEL_GREED_TAG))
	{
		return _float4(1.f,1.f,0.f,1.f);
	}
	else if (CUtility_String::Compare_WString(strModelTag, MODEL_LUXURY_TAG))
	{
		return _float4(0.f,0.3f,0.f,1.f);
	}
	else if (CUtility_String::Compare_WString(strModelTag, MODEL_PRIDE_TAG))
	{
		return _float4(0.f,0.f,1.f,1.f);
	}
	else if (CUtility_String::Compare_WString(strModelTag, MODEL_SLOTH_TAG))
	{
		return _float4(0.f,0.2f,0.6f,1.f);
	}
	else if (CUtility_String::Compare_WString(strModelTag, MODEL_WRATH_TAG))
	{
		return _float4(0.6f,0.f,0.6f,1.f);
	}

	return _float4(1.f,1.f,1.f,1.f);
}

_float4 CPuzzleMgr::Select_Color(_uint iIndx)
{
	if (iIndx == 0 || iIndx == 13)
	{
		return _float4(1.f, 0.f, 0.f, 1.f);
	}
	else if (iIndx == 1 || iIndx == 12)
	{
		return _float4(1.f, 0.3f, 0.f, 1.f);
	}
	else if (iIndx == 2 || iIndx == 11)
	{
		return _float4(1.f, 1.f, 0.f, 1.f);
	}
	else if (iIndx == 3 || iIndx == 10)
	{
		return _float4(0.f, 0.3f, 0.f, 1.f);
	}
	else if (iIndx == 4 || iIndx == 9)
	{
		return _float4(0.f, 0.f, 1.f, 1.f);
	}
	else if (iIndx == 5 || iIndx == 8)
	{
		return _float4(0.f, 0.2f, 0.6f, 1.f);
	}
	else if (iIndx == 6 || iIndx == 7)
	{
		return _float4(0.6f, 0.f, 0.6f, 1.f);
	}

	return _float4(1.f,1.f,1.f,1.f);
}

_bool CPuzzleMgr::Compare_Indx(_uint iSour, _uint iDest)
{
	if (iSour == 0 && iDest == 13)
	{
		return true;
	}
	else if ((iSour == 1 && iDest == 12) || (iSour == 12 && iDest == 1))
	{
		return true;
	}
	else if ((iSour == 2 && iDest == 11) || (iSour ==11 && iDest == 2))
	{
		return true;
	}
	else if ((iSour == 3 && iDest == 10) || (iSour == 10 && iDest == 3))
	{
		return true;
	}
	else if ((iSour == 4 && iDest == 9) || (iSour == 9 && iDest == 4))
	{
		return true;
	}
	else if ((iSour == 5 && iDest == 8) || (iSour == 8 && iDest == 5))
	{
		return true;
	}
	else if ((iSour == 6 && iDest == 7) || (iSour == 7 && iDest == 6))
	{
		return true;
	}

	return false;
}

void CPuzzleMgr::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);

}
