#include "stdafx.h"
#include "..\Public\Ending2.h"
#include "TargetCamera.h"

#include "Phantom.h"

#include "Player.h"

CEnding2::CEnding2()
{

}

HRESULT CEnding2::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	Ready_Font();

	return S_OK;
}

void CEnding2::Enter()
{
	SetUp_Camera();

	dynamic_cast<CPlayer*>(Find_Actor(TEXT("Player")))->Get_BodyModel()->Set_AnimationIndex(0);

	PRODUCTION_ACTOR_DESC PlayerInfo = {};
	PlayerInfo.fSensitivity = 0.7f;
	PlayerInfo.vOffset = Compute_Offset(TEXT("Player"), 0.2f, -0.3f, -0.5f);
	PlayerInfo.strActorTag = TEXT("Player");

	PRODUCTION_ACTOR_DESC PlayerInfo2 = {};
	PlayerInfo2.fSensitivity = 0.7f;
	PlayerInfo2.vOffset = PlayerInfo.vOffset;
	PlayerInfo2.strActorTag = TEXT("Player");

	PRODUCTION_ACTOR_DESC PlayerInfo3 = {};
	PlayerInfo3.fSensitivity = 0.7f;
	PlayerInfo3.vOffset = PlayerInfo.vOffset;
	PlayerInfo3.strActorTag = TEXT("Player");

	m_vecActorOrder.push_back(PlayerInfo);
	m_vecActorOrder.push_back(PlayerInfo2);
	m_vecActorOrder.push_back(PlayerInfo3);

	Camera_Target_Change(true);
	RenderUI();

	m_pGameInstance->Play_Sound(L"BGM", L"Ending.ogg", CHANNELID::SOUND_BGM, 0.7f, true);
}

void CEnding2::Tick()
{
	Is_Font_Render();

	if (m_pGameInstance->Key_Down(DIK_UPARROW))
	{
		Camera_Target_Change();
		RenderUI();
	}
}

void CEnding2::Render()
{
	RenderFont();
}

void CEnding2::Exite()
{

	Camera_Reset();
}

HRESULT CEnding2::Ready_UI()
{
	_float2 vChatPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 5.f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 7.f)) };
	_float2 vMarkPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 9.5f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 2.7f)) };

	if (FAILED(Add_GROUP(TEXT("Player1"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_JACK_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Player1"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Player2"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_JACK_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Player2"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Player3"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_JACK_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Player3"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	m_vecUIOrder.push_back(TEXT("Player1"));
	m_vecUIOrder.push_back(TEXT("Player2"));
	m_vecUIOrder.push_back(TEXT("Player3"));

	return S_OK;
}

void CEnding2::Ready_Font()
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
	Desc.strText = TEXT("다들 고생했어요");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("앞으로도 화이팅입니다");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("END");
	m_vecFont.push_back(Desc);
}

CEnding2* CEnding2::Create()
{
	CEnding2* pInstance = new CEnding2();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Cloned : CEnding2");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEnding2::Free()
{
	__super::Free();
}
