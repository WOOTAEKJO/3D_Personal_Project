#include "stdafx.h"
#include "..\Public\OwlTalk3.h"
#include "TargetCamera.h"

#include "Owl.h"
#include "Player.h"

COwlTalk3::COwlTalk3()
{

}

HRESULT COwlTalk3::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	Ready_Font();

	return S_OK;
}

void COwlTalk3::Enter()
{
	SetUp_Camera();

	PRODUCTION_ACTOR_DESC OwlInfo1 = {};
	OwlInfo1.fSensitivity = 0.6f;
	//OwlInfo1.vOffset = _float3(0.5f, -0.1f, 0.1f);
	OwlInfo1.vOffset = Compute_Offset(TEXT("Owl"), 0.1f, -0.1f, 0.5f);
	OwlInfo1.iAnimIndex = 2;
	OwlInfo1.strActorTag = TEXT("Owl");

	PRODUCTION_ACTOR_DESC OwlInfo2 = {};
	OwlInfo2.fSensitivity = 0.7f;
	OwlInfo2.vOffset = OwlInfo1.vOffset;
	OwlInfo2.strActorTag = TEXT("Owl");

	PRODUCTION_ACTOR_DESC OwlInfo3 = {};
	OwlInfo3.fSensitivity = 0.7f;
	OwlInfo3.vOffset = OwlInfo1.vOffset;
	OwlInfo3.strActorTag = TEXT("Owl");

	PRODUCTION_ACTOR_DESC PlayerInfo = {};
	PlayerInfo.fSensitivity = 0.7f;
	//PlayerInfo.vOffset = _float3(-0.4f, -0.3f, 0.3f);
	PlayerInfo.vOffset = Compute_Offset(TEXT("Player"), 0.3f, -0.3f, -0.4f);
	PlayerInfo.strActorTag = TEXT("Player");

	m_vecActorOrder.push_back(OwlInfo1);
	m_vecActorOrder.push_back(OwlInfo2);
	m_vecActorOrder.push_back(OwlInfo3);
	m_vecActorOrder.push_back(PlayerInfo);
	
	Camera_Target_Change(true);
	RenderUI();
}

void COwlTalk3::Tick()
{
	Is_Font_Render();

	if (m_pGameInstance->Key_Down(DIK_UPARROW))
	{
		Camera_Target_Change();
		RenderUI();
	}
}

void COwlTalk3::Render()
{
	RenderFont();
}

void COwlTalk3::Exite()
{
	//dynamic_cast<CCharacter*>()
	Find_Actor(TEXT("Owl"))->Get_Component<CStateMachine>()->Set_State(COwl::STATE::IDLE);

	Camera_Reset();
}

HRESULT COwlTalk3::Ready_UI()
{
	_float2 vChatPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 5.f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 7.f)) };
	_float2 vMarkPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 9.5f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 2.7f)) };

	if (FAILED(Add_GROUP(TEXT("Owl1"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_OWL_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Owl1"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Player1"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_JACK_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Player1"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Owl2"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_OWL_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Owl2"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Owl3"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_OWL_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Owl3"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	m_vecUIOrder.push_back(TEXT("Owl1"));
	m_vecUIOrder.push_back(TEXT("Owl2"));
	m_vecUIOrder.push_back(TEXT("Owl3"));
	m_vecUIOrder.push_back(TEXT("Player1"));

	return S_OK;
}

void COwlTalk3::Ready_Font()
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
	Desc.strText = TEXT("고생했어");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("포탈이 열렸을 거야");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("어서 허수아비한테 가봐");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("고마워");
	m_vecFont.push_back(Desc);
}

COwlTalk3* COwlTalk3::Create()
{
	COwlTalk3* pInstance = new COwlTalk3();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Cloned : COwlTalk3");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void COwlTalk3::Free()
{
	__super::Free();
}
