#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"

#include <process.h>

#include "BackGround.h"
#include "Terrain.h"
#include "Mark.h"
#include "DynamicCamera.h"
#include "TargetCamera.h"
#include "GameObject_Test.h"

#include "ObjectMesh_Demo.h"
#include "AnimMesh_Demo.h"
#include "SubObject_Demo.h"
#include "Terrain_Demo.h"
#include "Particle_Demo.h"

#include "Plateform.h"
#include "Plateform_Instancing.h"
#include "Plateform_Trap.h"
#include "SkyBox.h"

#include "Particle_Normal.h"
#include "Particle_Sprite.h"
#include "Particle_Sub.h"
#include "Particle_Attack.h"
#include "Particle_Always.h"
#include "Particle_Stage.h"

#include "Effect_Damage.h"
#include "Effect_Halo.h"
#include "Effect_Soul.h"
#include "Effect_Reaper.h"
#include "Effect_Laser.h"
#include "Effect_DashRoad.h"
#include "Effect_Energy.h"
#include "Effect_Target.h"
#include "Effect_Water.h"
#include "Effect_Trail.h"

#include "Player.h"
#include "Player_Body.h"
#include "Player_Weapon_Spear.h"
#include "Player_Weapon_Shovel.h"
#include "Crow.h"
#include "Owl.h"
#include "CrowDummy.h"

#include "Spooketon.h"
#include "SkullCrossBow.h"

#include "Trigger.h"
#include "Normal_Bullet.h"
#include "Range_Bullet.h"

#include "HelicoScarrow.h"
#include "Helico_Bullet.h"
#include "Shock_Wave.h"

#include "Phantom.h"
#include "Laser.h"
#include "Multiply.h"
#include "Bomb.h"
#include "Target_Bullet.h"
#include "Meteor.h"

#include "CameraPoint.h"

#include "Puzzle.h"


CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_uint APIENTRY LoadingThread(void* pArg)
{
	// APIENTRY -> __stdcall

	CoInitializeEx(nullptr, 0);
	// 컴객체를 초기화하는 함수
	//	다른 쓰레드에서 메인 쓰레드가 사용하던 컴 객체를 사용할 때 컴 객체를 초기화하고 사용해야 한다.

	CLoader*		pLoader = (CLoader*)pArg;
	// 인자로 받은 Loader 객체

	pLoader->Loading();
	
	return 0;
}


HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	/* 어떤 레벨의 자원을 로드해야하는지? */
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);
	// 크리티컬 섹션 초기화

	/* 스레드를 생성한다. */
	/* LoadingThread : 생성한 스레드가 가장 먼저 호출해야할 함수 */
	/* this : 위 함수를 호출하면서 인자로 전달할 데이터. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingThread, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	// this는 멤버함수마다 가지고 있다.

	return S_OK;
}

void CLoader::Print_LoadingText()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);
	// 소유권 획득을 요구

	HRESULT hr = 0;
	m_pGameInstance->Set_Current_Level(m_eNextLevelID);
	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Logo_Level();
		break;

	case LEVEL_GAMEPLAY:		
		hr = Loading_For_GamePlay_Level();
		break;
	case LEVEL_TOOL:
		hr = Loading_For_Tool_Level();
		break;
	case LEVEL_BOSS1:
		hr = Loading_For_Boss1_Level();
		break;
	case LEVEL_BOSS2:
		hr = Loading_For_Boss2_Level();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);
	// 소유권 반납

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo_Level()
{
	/* 로고 레벨에 필요한 자원을 로드하자. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_BACKGROUND_TAG, 2))) return E_FAIL;
		
	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로드하는 중입니다."));
	
	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로드하는 중입니다."));
	
	lstrcpy(m_szLoadingText, TEXT("원형객체를(을) 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CBackGround>(G0_BACKGROUND_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CGameObject_Test>(GO_GAMEOBJECTTEST_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	/* 게임플레이 레벨에 필요한 자원을 로드하자. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_TAG, 2))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_MASK_TAG, 1))) return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_LANDSCAPE_TAG,1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_SKYBOX_TAG, 3))) return E_FAIL;
	
	if (FAILED(Effect_Tex()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로드하는 중입니다."));

	//if (FAILED(m_pGameInstance->Add_Terrain_ProtoType_Height(BUFFER_TERRAIN_TAG, BUFFER_TERRAIN_HEIGHT_PATH))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Terrain_ProtoType_Binary(BUFFER_TERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_DTerrain>(BUFFER_DTERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_Cube>(BUFFER_CUBE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_Trail>(BUFFER_TRAIL_TAG))) return E_FAIL;

	_matrix	matPivot;
	matPivot = XMMatrixRotationY(XMConvertToRadians(180.f));

	matPivot = XMMatrixScaling(0.0005f, 0.0005f, 0.0005f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_JACK_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.00005f, 0.00005f, 0.00005f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_CROW_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_OWL_TAG, matPivot))) return E_FAIL;

	if (FAILED(Monster())) return E_FAIL;

	if (FAILED(Forest())) return E_FAIL;
	if (FAILED(Rock())) return E_FAIL;
	if (FAILED(Deco())) return E_FAIL;
	if (FAILED(Wood())) return E_FAIL;
	if (FAILED(Instancing())) return E_FAIL;

	if (FAILED(Item())) return E_FAIL;
	if (FAILED(Particle())) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("네비게이션를(을) 로드하는 중입니다."));
	if (FAILED(m_pGameInstance->Add_Navigation_ProtoType_File(COM_NAVIGATION_TAG))) return E_FAIL;
	
	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로드하는 중입니다."));

	//if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXNORTEX>(SHADER_NOR_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXMESH>(SHADER_MESH_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<INSTANCING_MESH>(SHADER_MESHINSTANCING_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXANIMMESH>(SHADER_ANIMMESH_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXCUBE>(SHADER_CUBE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXMESH>(SHADER_TERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<PARTICLE_POINT>(SHADER_PARTICLEPOINT_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("원형객체를(을) 로드하는 중입니다."));
	
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CTerrain>(G0_TERRAIN_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CDynamicCamera>(G0_DCAMERA_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CTargetCamera>(GO_TARGETCAMERA_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CAnimMesh_Demo>(G0_ANIMMESH_DEMO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlateform>(GO_PLATEFORM_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlateform_Instancing>(GO_PLATEFORM_INSTANCING_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlateform_Trap>(GO_PLATEFORM_TRAP_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CSkyBox>(GO_SKYBOX_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlayer>(ANIMMODEL_JACK_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlayer_Body>(GO_PLAYER_BODY_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlayer_Weapon_Spear>(GO_PLAYER_SPEAR_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPlayer_Weapon_Shovel>(GO_PLAYER_SHOVEL_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CCrow>(ANIMMODEL_CROW_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<COwl>(ANIMMODEL_OWL_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CCrowDummy>(GO_CROWDUMMY_TAG))) return E_FAIL;
	

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CSpooketon>(ANIMMODEL_SPOOKETON_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CSkullCrossBow>(ANIMMODEL_SKULLCROSSBOW_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CTrigger>(GO_TRIGGER_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPuzzle>(GO_PUZZLE_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CNormal_Bullet>(GO_NORMAL_BULLET_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CRange_Bullet>(GO_RANGE_BULLET_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CShock_Wave>(GO_SHOCK_WAVE_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CParticle_Normal>(GO_PARTICLENORMAL_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CParticle_Sprite>(GO_PARTICLESPRITE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CParticle_Sub>(GO_PARTICLESUB_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CParticle_Attack>(GO_PARTICLEATTACK_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CParticle_Always>(GO_PARTICLEALWAYS_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CParticle_Stage>(GO_PARTICLESTAGE_TAG))) return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CEffect_Damage>(GO_EFFECTDAMAGE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CEffect_Halo>(GO_EFFECTHALO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CEffect_Soul>(GO_EFFECTSOUL_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CEffect_Reaper>(GO_EFFECTREAPER_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CEffect_Energy>(GO_EFFECTENERGY_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CEffect_Trail>(GO_EFFECTTRAIL_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Tool_Level()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_TAG, 2))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_MASK_TAG, 1))) return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_LANDSCAPE_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_BRUSH_TAG, 1))) return E_FAIL;

	if (FAILED(Effect_Tex()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_DTerrain>(BUFFER_DTERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_Cube>(BUFFER_CUBE_TAG))) return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_Particle_Point>(BUFFER_PARTICLEPOINT_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(BUFFER_PARTICLEPOINT_TAG))) return E_FAIL;

	_matrix	matPivot;

	if (FAILED(Monster())) return E_FAIL;

	if (FAILED(Forest())) return E_FAIL;
	if (FAILED(Rock())) return E_FAIL;
	if (FAILED(Deco())) return E_FAIL;
	if (FAILED(Wood())) return E_FAIL;
	if (FAILED(Instancing())) return E_FAIL;

	if (FAILED(Item())) return E_FAIL;
	if (FAILED(Particle())) return E_FAIL;

	matPivot = XMMatrixScaling(0.0005f, 0.0005f, 0.0005f) *
		XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_JACK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_CROW_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_OWL_TAG, matPivot))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("네비게이션를(을) 로드하는 중입니다."));
	if (FAILED(m_pGameInstance->Add_Navigation_ProtoType_Demo(COM_NAVIGATION_DEMO_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로드하는 중입니다."));

	//if(FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXTBN>(SHADER_BTN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXMESH>(SHADER_TERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXMESH>(SHADER_MESH_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXANIMMESH>(SHADER_ANIMMESH_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<INSTANCING_MESH>(SHADER_MESHINSTANCING_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<PARTICLE_POINT>(SHADER_PARTICLEPOINT_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("원형객체를(을) 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CDynamicCamera>(G0_DCAMERA_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CTerrain_Demo>(G0_TERRAIN_DEMO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CObjectMesh_Demo>(G0_OBJECTMESH_DEMO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CAnimMesh_Demo>(G0_ANIMMESH_DEMO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CSubObject_Demo>(G0_SUBMESH_DEMO_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CParticle_Demo>(G0_PARTICLE_DEMO_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Boss1_Level()
{
	/* 게임플레이 레벨에 필요한 자원을 로드하자. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_TAG, 2))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_MASK_TAG, 1))) return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_LANDSCAPE_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_SKYBOX_TAG, 3))) return E_FAIL;

	if (FAILED(Effect_Tex()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_Terrain_ProtoType_Binary(BUFFER_TERRAIN2_TAG))) return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_DTerrain>(BUFFER_DTERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_Cube>(BUFFER_CUBE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_Trail>(BUFFER_TRAIL_TAG))) return E_FAIL;

	_matrix	matPivot;
	matPivot = XMMatrixRotationY(XMConvertToRadians(180.f));

	matPivot = XMMatrixScaling(0.0005f, 0.0005f, 0.0005f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_JACK_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.00005f, 0.00005f, 0.00005f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_CROW_TAG, matPivot))) return E_FAIL;

	if (FAILED(Monster())) return E_FAIL;

	if (FAILED(Forest())) return E_FAIL;
	if (FAILED(Rock())) return E_FAIL;
	if (FAILED(Deco())) return E_FAIL;
	if (FAILED(Wood())) return E_FAIL;
	if (FAILED(Instancing())) return E_FAIL;

	if (FAILED(Item())) return E_FAIL;
	if (FAILED(Particle())) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("네비게이션를(을) 로드하는 중입니다."));
	if (FAILED(m_pGameInstance->Add_Navigation_ProtoType_File(COM_NAVIGATION2_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로드하는 중입니다."));

	//if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXNORTEX>(SHADER_NOR_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXMESH>(SHADER_MESH_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<INSTANCING_MESH>(SHADER_MESHINSTANCING_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXANIMMESH>(SHADER_ANIMMESH_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXCUBE>(SHADER_CUBE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXMESH>(SHADER_TERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<PARTICLE_POINT>(SHADER_PARTICLEPOINT_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("원형객체를(을) 로드하는 중입니다."));

	
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CHelicoScarrow>(ANIMMODEL_HELICOSCARROW_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CHelico_Bullet>(MODEL_HELICOBULLET_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CPhantom>(ANIMMODEL_PHANTOM_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CCameraPoint>(GO_CAMERAPOINT_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Boss2_Level()
{
	/* 게임플레이 레벨에 필요한 자원을 로드하자. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_TAG, 2))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_TERRAIN_MASK_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_SKYBOX_TAG, 3))) return E_FAIL;

	if (FAILED(Effect_Tex()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_Terrain_ProtoType_Binary(BUFFER_TERRAIN3_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_Cube>(BUFFER_CUBE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_DRect>(BUFFER_DRECT_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Buffer_ProtoType<CVIBuffer_Trail>(BUFFER_TRAIL_TAG))) return E_FAIL;

	_matrix	matPivot;
	matPivot = XMMatrixRotationY(XMConvertToRadians(180.f));

	matPivot = XMMatrixScaling(0.0005f, 0.0005f, 0.0005f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_JACK_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.00005f, 0.00005f, 0.00005f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_CROW_TAG, matPivot))) return E_FAIL;

	if (FAILED(Monster())) return E_FAIL;

	if (FAILED(Forest())) return E_FAIL;
	if (FAILED(Rock())) return E_FAIL;
	if (FAILED(Deco())) return E_FAIL;
	if (FAILED(Wood())) return E_FAIL;
	if (FAILED(Instancing())) return E_FAIL;

	if (FAILED(Item())) return E_FAIL;
	if (FAILED(Particle())) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("네비게이션를(을) 로드하는 중입니다."));
	if (FAILED(m_pGameInstance->Add_Navigation_ProtoType_File(COM_NAVIGATION3_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXMESH>(SHADER_MESH_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<INSTANCING_MESH>(SHADER_MESHINSTANCING_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXANIMMESH>(SHADER_ANIMMESH_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXCUBE>(SHADER_CUBE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<VTXMESH>(SHADER_TERRAIN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Shader_ProtoType<PARTICLE_POINT>(SHADER_PARTICLEPOINT_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("원형객체를(을) 로드하는 중입니다."));

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CLaser>(GO_LASER_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CMultiply>(GO_MULTIPLY_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CBomb>(GO_BOMB_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CTarget_Bullet>(GO_TARGETBULLET_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CMeteor>(GO_METEOR_TAG))) return E_FAIL;

	

	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CEffect_Laser>(GO_EFFECTLASER_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CEffect_DashRoad>(GO_EFFECTDASHROAD_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CEffect_Target>(GO_EFFECTTARGET_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_ProtoType<CEffect_Water>(GO_EFFECTWATER_TAG))) return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true; 

	return S_OK;
}

HRESULT CLoader::Monster()
{
	_matrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_HELICOSCARROW_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_SKULLCROSSBOW_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_SPOOKETON_TAG, matPivot))) return E_FAIL;

	//matPivot = XMMatrixScaling(0.005f, 0.005f, 0.005f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ANIM_Model_ProtoType(ANIMMODEL_PHANTOM_TAG, matPivot))) return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Item()
{
#pragma region 아이템

	_matrix matPivot = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SPEAR_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SHOVEL_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.0015f, 0.0015f, 0.0015f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_HELICOBULLET_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_BOMB_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ENVY_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_GLUTTONY_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_GREED_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_LUXURY_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_PRIDE_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SLOTH_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WRATH_TAG, matPivot))) return E_FAIL;

#pragma endregion

	return S_OK;
}

HRESULT CLoader::Forest()
{
#pragma region FOREST
	_matrix	matPivot;

	matPivot = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_PINETREE_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_REED1_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_REED2_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE2_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE3_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREE4_TAG, matPivot))) return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FORESTTRUNK_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE1_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE2_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_CPINE3_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_MAPLE1_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_OWLTREE_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_PINE1_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS1_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS2_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS3_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS4_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SM_ROOTS5_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_TREE_PLATEFORME01X01_TAG, matPivot))) return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_TRUNKBREAK_TAG, MODEL_TRUNKBREAK_PATH, matPivot))) return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WOODSTOMP_TAG, MODEL_WOODSTOMP_PATH, matPivot))) return E_FAIL;


#pragma endregion

	return S_OK;
}

HRESULT CLoader::Rock()
{
#pragma region Rock
	_matrix	matPivot;

	matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKCLUSTER2_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKBIGFORMATION3_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKBIGFORMATION6_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKBIGFORMATION8_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKPACK1_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKPACK2_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKPACK2COL_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKPACK3_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKPACK4_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKPACK5_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKPACK52_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKPACK6_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_ROCKPACK7_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SMALLROCK_TAG, matPivot))) return E_FAIL;

#pragma endregion

	return S_OK;
}

HRESULT CLoader::Deco()
{
#pragma region Deco
	_matrix	matPivot;

	matPivot = XMMatrixScaling(0.001f, 0.001f, 0.001f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_BARREL_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_CAULDRON_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_EDGEROPE_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FENCEL_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FLOORLAMP_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_GROUNDLANTERN_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_HATHEAP_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_LANTERN_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_PENDINGROPE_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SCOURGEALTAR_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WOODFUCKER_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.0015f, 0.0015f, 0.0015f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_GRASSMESHBIG_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_PURPLESCHROOMS_TAG, matPivot))) return E_FAIL;

#pragma endregion

	return S_OK;
}

HRESULT CLoader::Wood()
{
#pragma region Wood
	_matrix	matPivot;

	matPivot = XMMatrixScaling(0.001f, 0.001f, 0.001f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_BROKENLATEFORMEWAY_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_CATEBOXPILE_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_CRATEBOX_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FENCE4_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FENCECLOSE_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FENCESTACK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FENCESTACKSMALL_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FENCESTACKSMALLCOL_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_FENCESTACKSMALLHIGHCOL_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_MINEPLATFORM_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_MINEPLATFORMFULL_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_MINETOWER_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_MINETOWERFULL_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_MINETOWERFULLFULL_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_SMROUNDFENCE_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WEATHERCLOCK_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WEATHERCLOCKHIGH_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WOODPLATEFORME1_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WOODPLATEFORME2_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WOODPLATEFORME05X05X2_TAG, matPivot))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WOODPLATFORM1SMALL_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WOODROPEHOLDER_TAG, matPivot))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Model_ProtoType(MODEL_WOODWALL2_TAG, matPivot))) return E_FAIL;

#pragma endregion

	return S_OK;
}

HRESULT CLoader::Instancing()
{
	_matrix	matPivot;

	matPivot = XMMatrixScaling(0.0015f, 0.0015f, 0.0015f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ModelInstancing_ProtoType(MODELINSTANCING_GRASSMESHBIG_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ModelInstancing_ProtoType(MODELINSTANCING_SMALLROCK_TAG, matPivot))) return E_FAIL;

	matPivot = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_ModelInstancing_ProtoType(MODELINSTANCING_PURPLESCHROOMS_TAG, matPivot))) return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Effect_Tex()
{
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_BUBLE_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_CONFETTISF_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_CONFETTISS_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_CONFETTIST_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_SMOKEPUFF_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_SMOKEPUFFMUSH_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_SNOW_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_ELEC_TAG, 9))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_STRAW_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_DAMAGEIMPACT_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_LANTERNHALO_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_SOUL_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_REAPER_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_BATSPRITE_TAG, 4))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_FEATHER_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_LASER_TAG, 1))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Texture_ProtoType(TEX_WATER_TAG, 1))) return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Particle()
{
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKRUN_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKJUMP_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKLANDING_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKELEC1_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKELEC2_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKELEC3_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKATTACK1_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKATTACK2_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKATTACK3_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKATTACK4_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKATTACK5_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKATTACK6_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKATTACK7_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKATTACK8_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_JACKHITTAG))) return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS1AIRATTACK1_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS1AIRATTACK2_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS1AIRATTACK3_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS1AIRATTACK4_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS1AIRATTACK5_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS1AIRATTACKEND_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS1WAVE_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS1HIT_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BAT1_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BAT2_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_CROWATTACK_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS2IDLE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS2WAVE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS2LASERELEC_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS2DASH_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOMBPARTICLE_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_EXPLOSION1_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_EXPLOSION2_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_EXPLOSION3_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_EXPLOSION4_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS2SHOOTHIT_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_METEORTAIL_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_METEORAFTER_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_METEORWAVE_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_METEOREXPLOSION1_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_METEOREXPLOSION2_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_METEOREXPLOSION3_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_STAGE1IDLE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_STAGE2IDLE_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_STAGE3IDLE_TAG))) return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS1BAT1_TAG))) return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Particle_ProtoType(PARTICLE_BOSS1BAT2_TAG))) return E_FAIL;
	
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eNextLevelID)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	// 현재 쓰레드가 작업 중이면 끝날 때까지 무한 대기

	DeleteCriticalSection(&m_CriticalSection);
	// 크리티컬 섹션을 삭제

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);
	// 쓰레드 삭제

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
