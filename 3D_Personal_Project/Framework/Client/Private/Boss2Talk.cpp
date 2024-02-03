#include "stdafx.h"
#include "..\Public\Boss2Talk.h"
#include "TargetCamera.h"

#include "Phantom.h"

#include "Trigger.h"
#include "Utility_Effect.h"

CBoss2Talk::CBoss2Talk()
{

}

HRESULT CBoss2Talk::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	Ready_Font();

	return S_OK;
}

void CBoss2Talk::Enter()
{
	SetUp_Camera();

	PRODUCTION_ACTOR_DESC Boss2Info1 = {};
	Boss2Info1.fSensitivity = 0.2f;
	Boss2Info1.vOffset = Compute_Offset(TEXT("Boss2"), 1.f, -2.5f, -2.f);
	Boss2Info1.strActorTag = TEXT("Boss2");
	Boss2Info1.fLookHeight = 12.f;

	PRODUCTION_ACTOR_DESC Boss2Info2 = {};
	Boss2Info2.fSensitivity = 0.7f;
	Boss2Info2.vOffset = Boss2Info1.vOffset;
	Boss2Info2.strActorTag = TEXT("Boss2");
	Boss2Info2.fLookHeight = Boss2Info1.fLookHeight;

	PRODUCTION_ACTOR_DESC Boss2Info3 = {};
	Boss2Info3.fSensitivity = 0.7f;
	Boss2Info3.vOffset = Boss2Info1.vOffset;
	Boss2Info3.strActorTag = TEXT("Boss2");
	Boss2Info3.fLookHeight = Boss2Info1.fLookHeight;

	PRODUCTION_ACTOR_DESC Boss2Info4 = {};
	Boss2Info4.fSensitivity = 0.7f;
	Boss2Info4.vOffset = Boss2Info1.vOffset;
	Boss2Info4.strActorTag = TEXT("Boss2");
	Boss2Info4.fLookHeight = Boss2Info1.fLookHeight;

	PRODUCTION_ACTOR_DESC PlayerInfo = {};
	PlayerInfo.fSensitivity = 0.7f;
	PlayerInfo.vOffset = Compute_Offset(TEXT("Player"), 0.3f, -0.3f, -0.4f);
	PlayerInfo.strActorTag = TEXT("Player");

	m_vecActorOrder.push_back(Boss2Info1);
	m_vecActorOrder.push_back(Boss2Info2);
	m_vecActorOrder.push_back(Boss2Info3);
	m_vecActorOrder.push_back(PlayerInfo);
	m_vecActorOrder.push_back(Boss2Info4);

	Camera_Target_Change(true);
	RenderUI();
}

void CBoss2Talk::Tick()
{
	Is_Font_Render();

	if (m_pGameInstance->Key_Down(DIK_UPARROW))
	{
		Camera_Target_Change();
		RenderUI();
	}
}

void CBoss2Talk::Render()
{
	RenderFont();
}

void CBoss2Talk::Exite()
{
	Find_Actor(TEXT("Boss2"))->Get_Component<CStateMachine>()->Set_State(CPhantom::STATE::INTROEND);

	CTrigger::TRIGGER_DESC TriggerDesc = {};
	TriggerDesc.strEventName = TEXT("Portal_Boss2");
	TriggerDesc.vPosition = _float4(26.3f, 2.f, 28.4f, 1.f);
	TriggerDesc.vScale = _float3(1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Clone(m_pGameInstance->Get_Current_Level(), g_strLayerName[LAYER::LAYER_PLATEFORM]
		, GO_TRIGGER_TAG, &TriggerDesc)))
		return;

	CUtility_Effect::Create_Particle_Stage(m_pGameInstance, PARTICLE_PORTAL_TAG, _float4(26.3f, 2.f, 28.4f, 1.f),
		nullptr, nullptr);

	Camera_Reset();
}

HRESULT CBoss2Talk::Ready_UI()
{
	_float2 vChatPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 5.f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 7.f)) };
	_float2 vMarkPos = { (_float)(g_iWinSizeX - (g_iWinSizeX / 9.5f)),(_float)(g_iWinSizeY - (g_iWinSizeY / 2.7f)) };

	if (FAILED(Add_GROUP(TEXT("Boss1"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_PHANTOM_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Boss1"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Player1"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_JACK_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Player1"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Boss2"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_PHANTOM_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Boss2"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Boss3"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_PHANTOM_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Boss3"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	if (FAILED(Add_GROUP(TEXT("Boss4"), Add_UI_Dissolve(_float2(vMarkPos.x, vMarkPos.y),
		_float2(200.f, 250.f), UI_PHANTOM_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;
	if (FAILED(Add_GROUP(TEXT("Boss4"), Add_UI_Dissolve(_float2(vChatPos.x, vChatPos.y),
		_float2(550.f, 150.f), UI_CHATBOX_TAG, UI_DISSOLVE_TAG, GO_UIDISSOLVE_TAG)))) return E_FAIL;

	m_vecUIOrder.push_back(TEXT("Boss1"));
	m_vecUIOrder.push_back(TEXT("Boss2"));
	m_vecUIOrder.push_back(TEXT("Boss3"));
	m_vecUIOrder.push_back(TEXT("Player1"));
	m_vecUIOrder.push_back(TEXT("Boss4"));

	return S_OK;
}

void CBoss2Talk::Ready_Font()
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
	Desc.strText = TEXT("크하하하하");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("아주 재미있는 구경이었어");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("나랑도 싸워야지?");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("이자식이");
	m_vecFont.push_back(Desc);

	Desc.strText = TEXT("크하하하하");
	m_vecFont.push_back(Desc);
}

CBoss2Talk* CBoss2Talk::Create()
{
	CBoss2Talk* pInstance = new CBoss2Talk();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Cloned : CBoss2Talk");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss2Talk::Free()
{
	__super::Free();
}
