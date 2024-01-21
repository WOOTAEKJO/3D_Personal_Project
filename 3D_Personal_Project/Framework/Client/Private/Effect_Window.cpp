#include "stdafx.h"
#include "../Imgui/imgui.h"
#include "../Imgui/imgui_impl_win32.h"
#include "../Imgui/imgui_impl_dx11.h"
#include "..\Public\Effect_Window.h"

#include "../Public/ImGuiMgr.h"

#include "GameInstance.h"
#include "Particle_Demo.h"

CEffect_Window::CEffect_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CImGui_Window(pDevice, pContext)
{
}

HRESULT CEffect_Window::Initialize(void* pArg)
{
	if (FAILED(CImGui_Window::Initialize(pArg)))
		return E_FAIL;

	CComponent_Manager::PROTOTYPE mapProtoCom = m_pGameInstance->Get_Com_ProtoType(m_pGameInstance->Get_Current_Level());

	for (auto& iter : mapProtoCom)
	{
		if (!wcscmp(CUtility_String::Get_MiddleName(iter.first).c_str(), L"Texture"))
		{
			m_vecTextureTag.push_back(iter.first);
		}
	}

	m_RandomNumber = mt19937_64(m_RandomDevice());

	return S_OK;
}

void CEffect_Window::Tick()
{
}

HRESULT CEffect_Window::Render()
{
	CImGui_Window::Begin();

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("Effect", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Particle"))
		{
			Particle();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	CImGui_Window::End();

	return S_OK;
}

void CEffect_Window::Set_Variable(void* pArg)
{
}

void CEffect_Window::Terrain_Picked(_float4 vPickPoint)
{
}

void CEffect_Window::Demo_Picked()
{
}

string CEffect_Window::Get_Path()
{
	return string();
}

void CEffect_Window::Particle()
{
	ImGui::Text("Texture_List");
	ImVec2 vSize = ImVec2(250, 100);
	ImGui::BeginListBox("Texture", vSize);
	for (auto& iter : m_vecTextureTag) {

		size_t pos = iter.rfind(L"_");
		wstring wstr = iter.substr(pos + 1);
		string str;
		str.assign(wstr.begin(), wstr.end());// 무슨 오류가 남
		if (ImGui::Selectable(str.c_str(), iter == m_strPickTextureTag))
			m_strPickTextureTag = iter;
	}
	ImGui::EndListBox();

	Arrow_Button("Interval", 0.1f, &m_fInterval);
	
	ImGui::InputInt("InstanceNum", &m_eParticleInfo.iInstanceNum);
	if (m_eParticleInfo.iInstanceNum <= 0)
		m_eParticleInfo.iInstanceNum = 1;

	Arrow_Button("Range", m_fInterval,&m_eParticleInfo.fRange);

	Arrow_Button("MinLife", m_fInterval, &m_eParticleInfo.fLifeTime.x);
	Arrow_Button("MaxLife", m_fInterval, &m_eParticleInfo.fLifeTime.y);

	Arrow_Button("MinScale", m_fInterval, &m_eParticleInfo.fScale.x);
	Arrow_Button("MaxScale", m_fInterval, &m_eParticleInfo.fScale.y);

	Arrow_Button("MinSpeed", m_fInterval, &m_eParticleInfo.fSpeed.x);
	Arrow_Button("MaxSpeed", m_fInterval, &m_eParticleInfo.fSpeed.y);

	ImGui::InputFloat4("Color", &(_float&)(m_eParticleInfo.vColor));

	ImGui::RadioButton("Texture_Color", &m_eParticleInfo.iTextureType, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Solid_Color", &m_eParticleInfo.iTextureType, 1);

	ImGui::InputFloat3("Dir", &(_float&)(m_eParticleInfo.vDir));

	ImGui::InputFloat3("RunDir", &(_float&)(m_eParticleInfo.vRunDir));

	ImGui::Checkbox("X", &m_bRandomRotation[0]);
	ImGui::SameLine();
	ImGui::Checkbox("Y", &m_bRandomRotation[1]);
	ImGui::SameLine();
	ImGui::Checkbox("Z", &m_bRandomRotation[2]);

	Particle_Rotation();

	if (m_pParticle != nullptr)
	{
		if(ImGui::Checkbox("Update", &m_bUpdate))
			m_pParticle->Set_Update(m_bUpdate);	

		if (ImGui::Button("Change_Color_Type"))
			m_pParticle->TextureType((CParticle_Demo::COLORTYPE)m_eParticleInfo.iTextureType);
	}

	if (ImGui::Button("Create Particle"))
	{
		Create_Particle();
	}
}

void CEffect_Window::Create_Particle()
{
	if (m_pParticle != nullptr)
	{
		m_pParticle->Set_Dead();
		m_pParticle = nullptr;
	}

	CParticle_Demo::PARTICLEDEMO_DESC Particle_Info = {};

	Particle_Info.strModelTag = m_strPickTextureTag;

	Particle_Info.fRange = m_eParticleInfo.fRange;
	//Particle_Info.vCenter = m_eParticleInfo.vCenter;
	Particle_Info.vCenter = _float3(0.f,0.f,0.f);
	Particle_Info.fLifeTime = m_eParticleInfo.fLifeTime;
	Particle_Info.fScale = m_eParticleInfo.fScale;
	Particle_Info.fSpeed = m_eParticleInfo.fSpeed;
	Particle_Info.iInstanceNum = m_eParticleInfo.iInstanceNum; // 인스턴스 갯수

	Particle_Info.vColor = m_eParticleInfo.vColor; // 생성 색
	Particle_Info.eTextureType = (CParticle_Demo::COLORTYPE)m_eParticleInfo.iTextureType; // 생성 컬러 타입

	Particle_Info.vDir = m_eParticleInfo.vDir;	// 생성 방향
	Particle_Info.vRotation = m_eParticleInfo.vRotation; // 생성 각도

	Particle_Info.vRunDir = m_eParticleInfo.vRunDir; // 실시간 방향

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL, g_strLayerName[LAYER::LAYER_EFFECT], G0_PARTICLE_DEMO_TAG,
		&Particle_Info, reinterpret_cast<CGameObject**>(&m_pParticle))))
		return;

	m_bUpdate = false;
}

void CEffect_Window::Particle_Rotation()
{
	uniform_real_distribution<float>	RandomAngle(0.f, 360.f);

	_float3 vRot = { 0.f,0.f,0.f };

	if (m_bRandomRotation[0])
	{
		m_eParticleInfo.vRotation.x = RandomAngle(m_RandomNumber);
	}
	else
		ImGui::InputFloat("RotationX", &m_eParticleInfo.vRotation.x);

	if (m_bRandomRotation[1])
	{
		m_eParticleInfo.vRotation.y = RandomAngle(m_RandomNumber);
	}else
		ImGui::InputFloat("RotationY", &m_eParticleInfo.vRotation.y);

	if (m_bRandomRotation[2])
	{
		m_eParticleInfo.vRotation.z = RandomAngle(m_RandomNumber);
	}else
		ImGui::InputFloat("RotationZ", &m_eParticleInfo.vRotation.z);
}

CEffect_Window* CEffect_Window::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,void* pArg)
{
	CEffect_Window* pInstance = new CEffect_Window(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CEffect_Window");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Window::Free()
{
	__super::Free();

	//Safe_Release(m_pParticle);

}
