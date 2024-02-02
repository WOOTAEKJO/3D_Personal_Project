#include "stdafx.h"
#include "..\Public\OwlTalk.h"
#include "TargetCamera.h"

#include "Owl.h"
#include "Player.h"

#include "UI_ChatBox.h"

COwlTalk::COwlTalk()
{

}

HRESULT COwlTalk::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	Ready_Font();

	return S_OK;
}

void COwlTalk::Enter()
{
	SetUp_Camera();

	PRODUCTION_ACTOR_DESC OwlInfo1 = {};
	OwlInfo1.fSensitivity = 0.2f;
	//OwlInfo1.vOffset = _float3(0.5f, -0.1f, 0.1f);
	OwlInfo1.vOffset = Compute_Offset(TEXT("Owl"), 0.1f, -0.1f, 0.5f);
	OwlInfo1.iAnimIndex = 2;
	OwlInfo1.strActorTag = TEXT("Owl");

	PRODUCTION_ACTOR_DESC OwlInfo2 = {};
	OwlInfo2.fSensitivity = 0.7f;
	OwlInfo2.vOffset = OwlInfo1.vOffset;
	OwlInfo2.strActorTag = TEXT("Owl");

	PRODUCTION_ACTOR_DESC PlayerInfo = {};
	PlayerInfo.fSensitivity = 0.7f;
	//PlayerInfo.vOffset = _float3(-0.4f, -0.3f, 0.3f);
	PlayerInfo.vOffset = Compute_Offset(TEXT("Player"), 0.3f, -0.3f, -0.4f);
	PlayerInfo.strActorTag = TEXT("Player");

	m_vecActorOrder.push_back(OwlInfo1);
	m_vecActorOrder.push_back(PlayerInfo);
	m_vecActorOrder.push_back(OwlInfo2);

	Camera_Target_Change(true);
	RenderUI();
}

void COwlTalk::Tick()
{
	if (m_pGameInstance->Key_Down(DIK_UPARROW))
	{
		Camera_Target_Change();
		RenderUI();
		
	}

}

void COwlTalk::Render()
{
	RenderFont();
}

void COwlTalk::Exite()
{
	//dynamic_cast<CCharacter*>()
	Find_Actor(TEXT("Owl"))->Get_Component<CStateMachine>()->Set_State(COwl::STATE::IDLE);

	Camera_Reset();
}

HRESULT COwlTalk::Ready_UI()
{
	_float2 vChatPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 5.f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 7.f)) };
	_float2 vMarkPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 9.5f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 2.7f)) };

	if (FAILED(Add_GROUP(TEXT("Owl1"), Add_UI(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_OWL_TAG, GO_UICHATBOX_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Owl1"), Add_UI(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, GO_UICHATBOX_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Player1"), Add_UI(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_JACK_TAG, GO_UICHATBOX_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Player1"), Add_UI(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, GO_UICHATBOX_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Owl2"), Add_UI(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_OWL_TAG, GO_UICHATBOX_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Owl2"), Add_UI(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, GO_UICHATBOX_TAG)))) return E_FAIL;

	m_vecUIOrder.push_back(TEXT("Owl1"));
	m_vecUIOrder.push_back(TEXT("Player1"));
	m_vecUIOrder.push_back(TEXT("Owl2"));

	return S_OK;
}

void COwlTalk::Ready_Font()
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
	Desc.strText = TEXT("안녕 호박아?");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("허수아비를 보았니?");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("까마귀를 찾아가봐");
	m_vecFont.push_back(Desc);
}

COwlTalk* COwlTalk::Create()
{
	COwlTalk* pInstance = new COwlTalk();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Cloned : COwlTalk");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void COwlTalk::Free()
{
	__super::Free();
}
