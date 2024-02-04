#include "stdafx.h"
#include "..\Public\Boss1Talk.h"
#include "TargetCamera.h"

#include "HelicoScarrow.h"

CBoss1Talk::CBoss1Talk()
{

}

HRESULT CBoss1Talk::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	Ready_Font();

	return S_OK;
}

void CBoss1Talk::Enter()
{
	SetUp_Camera();

	PRODUCTION_ACTOR_DESC Boss1Info1 = {};
	Boss1Info1.fSensitivity = 0.2f;
	Boss1Info1.vOffset = Compute_Offset(TEXT("Boss1"), 0.4f, -0.3f, -0.5f);
	//Boss1Info1.iAnimIndex = 2;
	Boss1Info1.strActorTag = TEXT("Boss1");

	PRODUCTION_ACTOR_DESC Boss1Info2 = {};
	Boss1Info2.fSensitivity = 0.7f;
	Boss1Info2.vOffset = Boss1Info1.vOffset;
	Boss1Info2.strActorTag = TEXT("Boss1");

	PRODUCTION_ACTOR_DESC PlayerInfo = {};
	PlayerInfo.fSensitivity = 0.7f;
	PlayerInfo.vOffset = Compute_Offset(TEXT("Player"), 0.3f, -0.3f, -0.4f);
	PlayerInfo.strActorTag = TEXT("Player");

	PRODUCTION_ACTOR_DESC PlayerInfo2 = {};
	PlayerInfo2.fSensitivity = 0.7f;
	PlayerInfo2.vOffset = PlayerInfo.vOffset;
	PlayerInfo2.strActorTag = TEXT("Player");

	m_vecActorOrder.push_back(Boss1Info1);
	m_vecActorOrder.push_back(PlayerInfo);
	m_vecActorOrder.push_back(Boss1Info2);
	m_vecActorOrder.push_back(PlayerInfo2);

	Camera_Target_Change(true);
	RenderUI();

	m_pGameInstance->Play_Sound(L"BGM", L"Boss1Intro.ogg", CHANNELID::SOUND_BGM, 0.7f, true);
}

void CBoss1Talk::Tick()
{
	Is_Font_Render();

	if (m_pGameInstance->Key_Down(DIK_UPARROW))
	{
		Camera_Target_Change();
		RenderUI();
	}
}

void CBoss1Talk::Render()
{
	RenderFont();
}

void CBoss1Talk::Exite()
{
	dynamic_cast<CHelicoScarrow*>(Find_Actor(TEXT("Boss1")))->Open_Status_Desc()->bTalk = false;

	Camera_Reset();

	m_pGameInstance->Play_Sound(L"BGM", L"Boss1Fight.ogg", CHANNELID::SOUND_BGM, 0.7f, true);
}

HRESULT CBoss1Talk::Ready_UI()
{
	_float2 vChatPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 5.f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 7.f)) };
	_float2 vMarkPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 9.5f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 2.7f)) };

	if (FAILED(Add_GROUP(TEXT("Boss1"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_SCARECROW_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Boss1"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Player1"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_JACK_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Player1"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Boss2"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_SCARECROW_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Boss2"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Player2"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_JACK_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Player2"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	m_vecUIOrder.push_back(TEXT("Boss1"));
	m_vecUIOrder.push_back(TEXT("Player1"));
	m_vecUIOrder.push_back(TEXT("Boss2"));
	m_vecUIOrder.push_back(TEXT("Player2"));

	return S_OK;
}

void CBoss1Talk::Ready_Font()
{
	_float2 vChatPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 5.f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 7.f)) };
	_float2 vChatSize = { 550.f, 150.f };
	_float2 vFontPos = { vChatPos.x - (vChatSize.x * 0.4f), vChatPos.y - (vChatSize.y * 0.3f) };

	PRODUCTION_DIALOG_DESC Desc = {};

	Desc.iFontTag = FONT_TYPE::FONT_139EX;
	Desc.vPosition = vFontPos;
	Desc.fScale = 0.8f;
	Desc.vColor = _float4(0.f, 0.f, 0.f, 1.f);
	Desc.vOrigin = _float2(0.f, 0.f);
	Desc.fRotation = 0.f;
	Desc.strText = TEXT("호박..?");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("허수아비야 무슨 일이야");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("도와줘..");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("내가 구해줄게");
	m_vecFont.push_back(Desc);
}

CBoss1Talk* CBoss1Talk::Create()
{
	CBoss1Talk* pInstance = new CBoss1Talk();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Cloned : CBoss1Talk");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss1Talk::Free()
{
	__super::Free();
}
