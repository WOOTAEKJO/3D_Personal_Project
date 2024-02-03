#include "stdafx.h"
#include "..\Public\CrowTalk.h"
#include "TargetCamera.h"

#include "Crow.h"

CCrowTalk::CCrowTalk()
{

}

HRESULT CCrowTalk::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	Ready_Font();

	return S_OK;
}

void CCrowTalk::Enter()
{
	SetUp_Camera();

	PRODUCTION_ACTOR_DESC CrowInfo1 = {};
	CrowInfo1.fSensitivity = 0.1f;
	CrowInfo1.vOffset = Compute_Offset(TEXT("Crow"), 0.1f, -0.2f, -0.5f);
	CrowInfo1.iAnimIndex = 2;
	CrowInfo1.strActorTag = TEXT("Crow");

	PRODUCTION_ACTOR_DESC CrowInfo2 = {};
	CrowInfo2.fSensitivity = 0.7f;
	CrowInfo2.vOffset = CrowInfo1.vOffset;
	CrowInfo2.strActorTag = TEXT("Crow");

	PRODUCTION_ACTOR_DESC PlayerInfo = {};
	PlayerInfo.fSensitivity = 0.7f;
	PlayerInfo.vOffset = Compute_Offset(TEXT("Player"), 0.3f, -0.3f, -0.4f);
	PlayerInfo.strActorTag = TEXT("Player");

	PRODUCTION_ACTOR_DESC CrowInfo3 = {};
	CrowInfo3.fSensitivity = 0.7f;
	CrowInfo3.vOffset = CrowInfo1.vOffset;
	CrowInfo3.strActorTag = TEXT("Crow");

	PRODUCTION_ACTOR_DESC PlayerInfo2 = {};
	PlayerInfo2.fSensitivity = 0.7f;
	PlayerInfo2.vOffset = PlayerInfo.vOffset;
	PlayerInfo2.strActorTag = TEXT("Player");

	m_vecActorOrder.push_back(CrowInfo1);
	m_vecActorOrder.push_back(PlayerInfo);
	m_vecActorOrder.push_back(CrowInfo2);
	m_vecActorOrder.push_back(CrowInfo3);
	m_vecActorOrder.push_back(PlayerInfo2);

	Camera_Target_Change(true);
	RenderUI();

}

void CCrowTalk::Tick()
{
	Is_Font_Render();

	if (m_pGameInstance->Key_Down(DIK_UPARROW))
	{
		Camera_Target_Change();
		RenderUI();
	}
}

void CCrowTalk::Render()
{
	RenderFont();
}

void CCrowTalk::Exite()
{
	Find_Actor(TEXT("Crow"))->Get_Component<CStateMachine>()->Set_State(CCrow::STATE::FOLLOW);

	Camera_Reset();
}

HRESULT CCrowTalk::Ready_UI()
{
	_float2 vChatPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 5.f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 7.f)) };
	_float2 vMarkPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 9.5f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 2.7f)) };

	if (FAILED(Add_GROUP(TEXT("Crow1"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_CROW_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Crow1"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Player1"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_JACK_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Player1"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Crow2"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_CROW_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Crow2"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Crow3"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_CROW_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Crow3"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Player2"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_JACK_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Player2"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	m_vecUIOrder.push_back(TEXT("Crow1"));
	m_vecUIOrder.push_back(TEXT("Player1"));
	m_vecUIOrder.push_back(TEXT("Crow2"));
	m_vecUIOrder.push_back(TEXT("Crow3"));
	m_vecUIOrder.push_back(TEXT("Player2"));

	return S_OK;
}

void CCrowTalk::Ready_Font()
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
	Desc.strText = TEXT("무슨 일이니?");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("허수아비를 보았니?");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("안쪽으로 갔어");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("내가 도와줄까?");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("고마워");
	m_vecFont.push_back(Desc);
}

CCrowTalk* CCrowTalk::Create()
{
	CCrowTalk* pInstance = new CCrowTalk();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Cloned : CCrowTalk");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCrowTalk::Free()
{
	__super::Free();
}
