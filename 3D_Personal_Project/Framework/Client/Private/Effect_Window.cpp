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

HRESULT CEffect_Window::Save_Data(const _char* strFilePath)
{
	ofstream fout;

	fout.open(strFilePath, std::ofstream::binary);

	if (fout.is_open())
	{
		
	}
	else
		return E_FAIL;

	fout.close();

	return S_OK;
}

HRESULT CEffect_Window::Load_Data(const _char* strFilePath)
{
	return S_OK;
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

	ImGui::InputFloat2("Life", &(_float&)m_eParticleInfo.fLifeTime);

	ImGui::InputFloat2("Scale", &(_float&)m_eParticleInfo.fScale);
	Arrow_Button("ScaleControl", m_fInterval, &m_eParticleInfo.fScaleControl);

	ImGui::InputFloat2("SpeedX", &(_float&)m_eParticleInfo.fSpeed[0]);
	ImGui::InputFloat2("SpeedY", &(_float&)m_eParticleInfo.fSpeed[1]);
	ImGui::InputFloat2("SpeedZ", &(_float&)m_eParticleInfo.fSpeed[2]);

	ImGui::InputFloat3("PowerSpeed", &(_float&)(m_eParticleInfo.fPowerSpeed));

	ImGui::InputFloat4("Color", &(_float&)(m_eParticleInfo.vColor));

	if (ImGui::RadioButton("Normal", &m_iTypeRadio, 0))
		m_eParticleInfo.eColorType = (INSTANCING_DESC::COLORTYPE)m_iTypeRadio;
	ImGui::SameLine();
	if(ImGui::RadioButton("Solid", &m_iTypeRadio, 1))
		m_eParticleInfo.eColorType = (INSTANCING_DESC::COLORTYPE)m_iTypeRadio;

	ImGui::InputFloat3("Dir", &(_float&)(m_eParticleInfo.vDir));
	ImGui::InputFloat3("RunDir", &(_float&)(m_eParticleInfo.vRunDir));

	ImGui::InputFloat2("RunRotX", &(_float&)(m_eParticleInfo.fRunRotation[0]));
	ImGui::InputFloat2("RunRotY", &(_float&)(m_eParticleInfo.fRunRotation[1]));
	ImGui::InputFloat2("RunRotZ", &(_float&)(m_eParticleInfo.fRunRotation[2]));

	ImGui::Checkbox("RandomX", &m_bRandomRotation[0]);
	ImGui::SameLine();
	ImGui::Checkbox("RandomY", &m_bRandomRotation[1]);
	ImGui::SameLine();
	ImGui::Checkbox("RandomZ", &m_bRandomRotation[2]);

	Particle_Rotation();

	ImGui::Checkbox("Loop", &m_eParticleInfo.bLoop);

	if (m_pParticle != nullptr)
	{
		if(ImGui::Checkbox("Update", &m_bUpdate))
			m_pParticle->Set_Update(m_bUpdate);	
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

	INSTANCING_DESC Particle_Info = {};

	Particle_Info.strTextureTag = m_strPickTextureTag;

	Particle_Info.fRange = m_eParticleInfo.fRange;
	Particle_Info.vCenter = _float3(0.f,0.f,0.f);
	Particle_Info.fLifeTime = m_eParticleInfo.fLifeTime;

	Particle_Info.fScale = m_eParticleInfo.fScale;
	Particle_Info.fScaleControl = m_eParticleInfo.fScaleControl;

	Particle_Info.fSpeed[0] = m_eParticleInfo.fSpeed[0];
	Particle_Info.fSpeed[1] = m_eParticleInfo.fSpeed[1];
	Particle_Info.fSpeed[2] = m_eParticleInfo.fSpeed[2];
	Particle_Info.fPowerSpeed = m_eParticleInfo.fPowerSpeed;

	Particle_Info.iInstanceNum = m_eParticleInfo.iInstanceNum; // 인스턴스 갯수

	Particle_Info.vColor = m_eParticleInfo.vColor; // 생성 색

	Particle_Info.vDir = m_eParticleInfo.vDir;	// 생성 방향
	Particle_Info.vRunDir = m_eParticleInfo.vRunDir; // 실시간 방향

	Particle_Info.fRotation[0] = m_eParticleInfo.fRotation[0]; // 생성 각도
	Particle_Info.fRotation[1] = m_eParticleInfo.fRotation[1];
	Particle_Info.fRotation[2] = m_eParticleInfo.fRotation[2];

	Particle_Info.fRunRotation[0] = m_eParticleInfo.fRunRotation[0]; // 실시간 각도
	Particle_Info.fRunRotation[1] = m_eParticleInfo.fRunRotation[1];
	Particle_Info.fRunRotation[2] = m_eParticleInfo.fRunRotation[2];

	Particle_Info.bLoop = m_eParticleInfo.bLoop;

	Particle_Info.eColorType = m_eParticleInfo.eColorType;

	if (FAILED(m_pGameInstance->Add_Clone(LEVEL_TOOL, g_strLayerName[LAYER::LAYER_EFFECT], G0_PARTICLE_DEMO_TAG,
		&Particle_Info, reinterpret_cast<CGameObject**>(&m_pParticle))))
		return;

	m_bUpdate = false;
}

void CEffect_Window::Particle_Rotation()
{
	uniform_real_distribution<float>	RandomAngle(0.f, 360.f);

	if (m_bRandomRotation[0])
	{
		do {
			m_eParticleInfo.fRotation[0].x = RandomAngle(m_RandomNumber);
			m_eParticleInfo.fRotation[0].y = RandomAngle(m_RandomNumber);
		} while (m_eParticleInfo.fRotation[0].x > m_eParticleInfo.fRotation[0].y);
	}
	else
		ImGui::InputFloat2("RotX", &(_float&)(m_eParticleInfo.fRotation[0]));

	if (m_bRandomRotation[1])
	{
		/*m_eParticleInfo.fRotation[1].x = RandomAngle(m_RandomNumber);
		m_eParticleInfo.fRotation[1].y = RandomAngle(m_RandomNumber);*/
		do {
			m_eParticleInfo.fRotation[1].x = RandomAngle(m_RandomNumber);
			m_eParticleInfo.fRotation[1].y = RandomAngle(m_RandomNumber);
		} while (m_eParticleInfo.fRotation[1].x > m_eParticleInfo.fRotation[1].y);
	}else
		ImGui::InputFloat2("RotY", &(_float&)(m_eParticleInfo.fRotation[1]));

	if (m_bRandomRotation[2])
	{
		/*m_eParticleInfo.fRotation[2].x = RandomAngle(m_RandomNumber);
		m_eParticleInfo.fRotation[2].y = RandomAngle(m_RandomNumber);*/
		do {
			m_eParticleInfo.fRotation[2].x = RandomAngle(m_RandomNumber);
			m_eParticleInfo.fRotation[2].y = RandomAngle(m_RandomNumber);
		} while (m_eParticleInfo.fRotation[2].x > m_eParticleInfo.fRotation[2].y);

	}else
		ImGui::InputFloat2("RotZ", &(_float&)(m_eParticleInfo.fRotation[2]));
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
