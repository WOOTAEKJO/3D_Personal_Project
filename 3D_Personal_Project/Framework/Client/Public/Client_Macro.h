#pragma once


#pragma region 데이터 파일 주소

#define FILE_PATH					TEXT("../Bin/")

#pragma endregion	

#pragma region 제이슨 파일 주소

#define	JSON_STAGE1_PATH			"../Bin/Data/Object/Stage1.json"

#pragma endregion

#pragma region 버퍼

#define BUFFER_RECT_TAG				TEXT("Prototype_Component_VIBuffer_Rect")
#define BUFFER_DRECT_TAG				TEXT("Prototype_Component_VIBuffer_DRect")

#define BUFFER_TERRAIN_TAG			TEXT("Prototype_Component_VIBuffer_Terrain")
#define BUFFER_TERRAIN2_TAG			TEXT("Prototype_Component_VIBuffer_Terrain2")
#define BUFFER_TERRAIN3_TAG			TEXT("Prototype_Component_VIBuffer_Terrain3")
#define BUFFER_TERRAIN_HEIGHT_PATH	TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")

#define BUFFER_DTERRAIN_TAG			TEXT("Prototype_Component_VIBuffer_DTerrain")

#define BUFFER_CUBE_TAG				TEXT("Prototype_Component_VIBuffer_Cube")

#define BUFFER_PARTICLEPOINT_TAG	TEXT("Prototype_Component_VIBuffer_ParticlePoint")

#pragma endregion

#pragma region 쉐이더

#define	SHADER_POS_TAG						TEXT("Prototype_Component_Shader_VtxPosTex")
//#define SHADER_NOR_TAG						TEXT("Prototype_Component_Shader_VtxNorTex")
//#define	SHADER_BTN_TAG						TEXT("Prototype_Component_Shader_VtxTBN")
#define	SHADER_MESH_TAG						TEXT("Prototype_Component_Shader_VtxMesh")
#define	SHADER_MESHINSTANCING_TAG			TEXT("Prototype_Component_Shader_VtxMeshInstancing")
#define	SHADER_PARTICLEPOINT_TAG			TEXT("Prototype_Component_Shader_VtxParticlePoint")
#define	SHADER_ANIMMESH_TAG					TEXT("Prototype_Component_Shader_VtxAnimMesh")
#define	SHADER_CUBE_TAG						TEXT("Prototype_Component_Shader_VtxCube")
#define	SHADER_TERRAIN_TAG						TEXT("Prototype_Component_Shader_VtxTerrain")

#pragma endregion

#pragma region 모델

#pragma region 애니메이션 모델

#define	ANIMMODEL_FIONA_TAG				TEXT("Prototype_Component_AnimModel_Fiona")

#define	ANIMMODEL_RAT_TAG				TEXT("Prototype_Component_AnimModel_Rat")

#define	ANIMMODEL_BAT_TAG				TEXT("Prototype_Component_AnimModel_Bat")

#define	ANIMMODEL_JACK_TAG				TEXT("Prototype_Component_AnimModel_Jack")

#define	ANIMMODEL_HELICOSCARROW_TAG		TEXT("Prototype_Component_AnimModel_HelicoScarrow")

#define	ANIMMODEL_SKULLCROSSBOW_TAG		TEXT("Prototype_Component_AnimModel_SkullCrossBow")

#define	ANIMMODEL_SPOOKETON_TAG			TEXT("Prototype_Component_AnimModel_Spooketon")

#define	ANIMMODEL_CROW_TAG				TEXT("Prototype_Component_AnimModel_Crow")

#define	ANIMMODEL_OWL_TAG				TEXT("Prototype_Component_AnimModel_Owl")

#define	ANIMMODEL_PHANTOM_TAG			TEXT("Prototype_Component_AnimModel_PhantomBoss")

#pragma endregion

#pragma region 일반모델

#pragma region 포레스트

#define	MODEL_PINETREE_TAG			TEXT("Prototype_Component_Model_PineTree")

#define	MODEL_SM_REED1_TAG			TEXT("Prototype_Component_Model_SMReed1")

#define	MODEL_SM_REED2_TAG			TEXT("Prototype_Component_Model_SMReed2")

#define	MODEL_DEADTREE2_TAG			TEXT("Prototype_Component_Model_DeadTree2")

#define	MODEL_DEADTREE3_TAG			TEXT("Prototype_Component_Model_DeadTree3")

#define	MODEL_DEADTREE4_TAG			TEXT("Prototype_Component_Model_DeadTree4")

#define	MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree01aTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree01bTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree01cTrunck")

#define	MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree02aTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree02bTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree02cTrunck")

#define	MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree03aTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree03bTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree03cTrunck")

#define	MODEL_FORESTTRUNK_TAG			TEXT("Prototype_Component_Model_ForestTrunk")

#define	MODEL_SM_CPINE1_TAG			TEXT("Prototype_Component_Model_SMCpine1")
#define	MODEL_SM_CPINE2_TAG			TEXT("Prototype_Component_Model_SMCpine2")
#define	MODEL_SM_CPINE3_TAG			TEXT("Prototype_Component_Model_SMCpine3")

#define	MODEL_SM_MAPLE1_TAG			TEXT("Prototype_Component_Model_SMMaple1")

#define	MODEL_SM_OWLTREE_TAG			TEXT("Prototype_Component_Model_SMOwlTree")

#define	MODEL_SM_PINE1_TAG			TEXT("Prototype_Component_Model_SMPine1")

#define	MODEL_SM_ROOTS_TAG			TEXT("Prototype_Component_Model_SMRoots")
#define	MODEL_SM_ROOTS1_TAG			TEXT("Prototype_Component_Model_SMRoots1")
#define	MODEL_SM_ROOTS2_TAG			TEXT("Prototype_Component_Model_SMRoots2")
#define	MODEL_SM_ROOTS3_TAG			TEXT("Prototype_Component_Model_SMRoots3")
#define	MODEL_SM_ROOTS4_TAG			TEXT("Prototype_Component_Model_SMRoots4")
#define	MODEL_SM_ROOTS5_TAG			TEXT("Prototype_Component_Model_SMRoots5")

#define	MODEL_TREE_PLATEFORME01X01_TAG			TEXT("Prototype_Component_Model_TreePlateforme01x01")

#define	MODEL_TRUNKBREAK_TAG			TEXT("Prototype_Component_Model_TrunckBreak")

#define	MODEL_WOODSTOMP_TAG			TEXT("Prototype_Component_Model_WoodStomp")
#pragma endregion

#pragma region 데코

#define	MODEL_BARREL_TAG			TEXT("Prototype_Component_Model_barrel")

#define	MODEL_CAULDRON_TAG			TEXT("Prototype_Component_Model_Cauldron")

#define	MODEL_EDGEROPE_TAG			TEXT("Prototype_Component_Model_EdgeRope")

#define	MODEL_FENCEL_TAG			TEXT("Prototype_Component_Model_Fencel")

#define	MODEL_FLOORLAMP_TAG			TEXT("Prototype_Component_Model_FloorLamp")

#define	MODEL_GROUNDLANTERN_TAG			TEXT("Prototype_Component_Model_GroundLantern")

#define	MODEL_HATHEAP_TAG			TEXT("Prototype_Component_Model_HayHeap")

#define	MODEL_LANTERN_TAG			TEXT("Prototype_Component_Model_Lantern")

#define	MODEL_PENDINGROPE_TAG			TEXT("Prototype_Component_Model_PendingRope")

#define	MODEL_PURPLESCHROOMS_TAG			TEXT("Prototype_Component_Model_PurpleSchrooms")

#define	MODEL_SCOURGEALTAR_TAG			TEXT("Prototype_Component_Model_ScourgeAltar")

#define	MODEL_WOODFUCKER_TAG			TEXT("Prototype_Component_Model_WoodFucker")

#define	MODEL_GRASSMESHBIG_TAG		TEXT("Prototype_Component_Model_GrassMeshBig")

#pragma endregion

#pragma region 바위

#define	MODEL_ROCKCLUSTER2_TAG						TEXT("Prototype_Component_Model_rockCluster2")

#define	MODEL_ROCKBIGFORMATION3_TAG				TEXT("Prototype_Component_Model_rockBigFormation3")

#define	MODEL_ROCKBIGFORMATION6_TAG				TEXT("Prototype_Component_Model_rockBigFormation6")

#define	MODEL_ROCKBIGFORMATION8_TAG				TEXT("Prototype_Component_Model_rockBigFormation8")

#define	MODEL_ROCKPACK1_TAG				TEXT("Prototype_Component_Model_rockPack1")

#define	MODEL_ROCKPACK2_TAG				TEXT("Prototype_Component_Model_rockPack2")

#define	MODEL_ROCKPACK2COL_TAG				TEXT("Prototype_Component_Model_rockPack2Col")

#define	MODEL_ROCKPACK3_TAG				TEXT("Prototype_Component_Model_rockPack3")

#define	MODEL_ROCKPACK4_TAG				TEXT("Prototype_Component_Model_rockPack4")

#define	MODEL_ROCKPACK5_TAG				TEXT("Prototype_Component_Model_rockPack5")

#define	MODEL_ROCKPACK52_TAG				TEXT("Prototype_Component_Model_rockPack52")

#define	MODEL_ROCKPACK6_TAG				TEXT("Prototype_Component_Model_rockPack6")

#define	MODEL_ROCKPACK7_TAG				TEXT("Prototype_Component_Model_rockPack7")

#define	MODEL_SMALLROCK_TAG				TEXT("Prototype_Component_Model_SmallRock")

#pragma endregion

#pragma region 나무

#define	MODEL_BROKENLATEFORMEWAY_TAG			TEXT("Prototype_Component_Model_brokenPlateformeWay")

#define	MODEL_CATEBOXPILE_TAG			TEXT("Prototype_Component_Model_cateBoxPile")
#define	MODEL_CRATEBOX_TAG			TEXT("Prototype_Component_Model_crateBox")

#define	MODEL_FENCE4_TAG			TEXT("Prototype_Component_Model_fence4")
#define	MODEL_FENCECLOSE_TAG			TEXT("Prototype_Component_Model_fenceClose")
#define	MODEL_FENCESTACK_TAG			TEXT("Prototype_Component_Model_fenceStack")
#define	MODEL_FENCESTACKSMALL_TAG			TEXT("Prototype_Component_Model_fenceStackSmall")
#define	MODEL_FENCESTACKSMALLCOL_TAG			TEXT("Prototype_Component_Model_fenceStackSmallCol")
#define	MODEL_FENCESTACKSMALLHIGHCOL_TAG			TEXT("Prototype_Component_Model_fenceStackSmallHighCol")

#define	MODEL_MINEPLATFORM_TAG			TEXT("Prototype_Component_Model_minePlatform")
#define	MODEL_MINEPLATFORMFULL_TAG			TEXT("Prototype_Component_Model_minePlatformFull")

#define	MODEL_MINETOWER_TAG			TEXT("Prototype_Component_Model_mineTower")
#define	MODEL_MINETOWERFULL_TAG			TEXT("Prototype_Component_Model_mineTowerFull")
#define	MODEL_MINETOWERFULLFULL_TAG			TEXT("Prototype_Component_Model_mineTowerFullFull")

#define	MODEL_SMROUNDFENCE_TAG			TEXT("Prototype_Component_Model_SMRoundFence")

#define	MODEL_WEATHERCLOCK_TAG			TEXT("Prototype_Component_Model_weatherClock")
#define	MODEL_WEATHERCLOCKHIGH_TAG			TEXT("Prototype_Component_Model_weatherClockHigh")

#define	MODEL_WOODPLATEFORME1_TAG			TEXT("Prototype_Component_Model_woodPlateforme1")
#define	MODEL_WOODPLATEFORME2_TAG			TEXT("Prototype_Component_Model_woodPlateforme2")
#define	MODEL_WOODPLATEFORME05X05X2_TAG			TEXT("Prototype_Component_Model_woodPlateforme05x05x2")
#define	MODEL_WOODPLATFORM1SMALL_TAG			TEXT("Prototype_Component_Model_woodPlatform1Small")

#define	MODEL_WOODROPEHOLDER_TAG			TEXT("Prototype_Component_Model_woodRopeHolder")

#define	MODEL_WOODWALL2_TAG					TEXT("Prototype_Component_Model_woodWall2")

#pragma endregion

#pragma region 인스턴싱

#define	MODELINSTANCING_GRASSMESHBIG_TAG		TEXT("Prototype_Component_ModelInstancing_GrassMeshBig")
#define	MODELINSTANCING_SMALLROCK_TAG			TEXT("Prototype_Component_ModelInstancing_SmallRock")
#define	MODELINSTANCING_PURPLESCHROOMS_TAG		TEXT("Prototype_Component_ModelInstancing_PurpleSchrooms")

#pragma region 파티클

#define	PARTICLE_TEST_TAG		TEXT("Prototype_Component_Particle_Test1")
#define	PARTICLE_JACKRUN_TAG		TEXT("Prototype_Component_Particle_JackRun")
#define	PARTICLE_JACKJUMP_TAG		TEXT("Prototype_Component_Particle_JackJump")
#define	PARTICLE_JACKLANDING_TAG		TEXT("Prototype_Component_Particle_JackLanding")

#define	PARTICLE_JACKELEC1_TAG		TEXT("Prototype_Component_Particle_JackElec1")
#define	PARTICLE_JACKELEC2_TAG		TEXT("Prototype_Component_Particle_JackElec2")
#define	PARTICLE_JACKELEC3_TAG		TEXT("Prototype_Component_Particle_JackElec3")

#define	PARTICLE_JACKATTACK1_TAG		TEXT("Prototype_Component_Particle_JackAttack1")
#define	PARTICLE_JACKATTACK2_TAG		TEXT("Prototype_Component_Particle_JackAttack2")
#define	PARTICLE_JACKATTACK3_TAG		TEXT("Prototype_Component_Particle_JackAttack3")
#define	PARTICLE_JACKATTACK4_TAG		TEXT("Prototype_Component_Particle_JackAttack4")
#define	PARTICLE_JACKATTACK5_TAG		TEXT("Prototype_Component_Particle_JackAttack5")
#define	PARTICLE_JACKATTACK6_TAG		TEXT("Prototype_Component_Particle_JackAttack6")
#define	PARTICLE_JACKATTACK7_TAG		TEXT("Prototype_Component_Particle_JackAttack7")
#define	PARTICLE_JACKATTACK8_TAG		TEXT("Prototype_Component_Particle_JackAttack8")

#define	PARTICLE_JACKHITTAG		TEXT("Prototype_Component_Particle_JackHit")

#define	PARTICLE_BOSS1AIRATTACK1_TAG		TEXT("Prototype_Component_Particle_Boss1AirAttack1")
#define	PARTICLE_BOSS1AIRATTACK2_TAG		TEXT("Prototype_Component_Particle_Boss1AirAttack2")
#define	PARTICLE_BOSS1AIRATTACK3_TAG		TEXT("Prototype_Component_Particle_Boss1AirAttack3")
#define	PARTICLE_BOSS1AIRATTACK4_TAG		TEXT("Prototype_Component_Particle_Boss1AirAttack4")
#define	PARTICLE_BOSS1AIRATTACK5_TAG		TEXT("Prototype_Component_Particle_Boss1AirAttack5")

#define	PARTICLE_BOSS1AIRATTACKEND_TAG		TEXT("Prototype_Component_Particle_Boss1AirAttackEnd")

#define	PARTICLE_BOSS1WAVE_TAG		TEXT("Prototype_Component_Particle_Boss1Wave")

#define	PARTICLE_BOSS1HIT_TAG		TEXT("Prototype_Component_Particle_Boss1Hit")

#define	PARTICLE_BAT1_TAG		TEXT("Prototype_Component_Particle_Bat1")
#define	PARTICLE_BAT2_TAG		TEXT("Prototype_Component_Particle_Bat2")

#define	PARTICLE_CROWATTACK_TAG		TEXT("Prototype_Component_Particle_CrowAttack")

#define	PARTICLE_BOSS2IDLE_TAG		TEXT("Prototype_Component_Particle_Boss2IDLE")
#define	PARTICLE_BOSS2WAVE_TAG		TEXT("Prototype_Component_Particle_Boss2Wave")

#define	PARTICLE_BOSS2LASERELEC_TAG		TEXT("Prototype_Component_Particle_Boss2LaserElec")

#define	PARTICLE_BOSS2DASH_TAG		TEXT("Prototype_Component_Particle_Boss2Dash")

#define	PARTICLE_BOMBPARTICLE_TAG		TEXT("Prototype_Component_Particle_BombParticle")

#define	PARTICLE_EXPLOSION1_TAG		TEXT("Prototype_Component_Particle_Explosion1")
#define	PARTICLE_EXPLOSION2_TAG		TEXT("Prototype_Component_Particle_Explosion2")
#define	PARTICLE_EXPLOSION3_TAG		TEXT("Prototype_Component_Particle_Explosion3")
#define	PARTICLE_EXPLOSION4_TAG		TEXT("Prototype_Component_Particle_Explosion4")

#define	PARTICLE_BOSS2SHOOTHIT_TAG		TEXT("Prototype_Component_Particle_Boss2ShootHit")

#define	PARTICLE_METEORTAIL_TAG		TEXT("Prototype_Component_Particle_MeteorTail")
#define	PARTICLE_METEORAFTER_TAG		TEXT("Prototype_Component_Particle_MeteorAfter")
#define	PARTICLE_METEORWAVE_TAG		TEXT("Prototype_Component_Particle_MeteorWave")

#define	PARTICLE_METEOREXPLOSION1_TAG		TEXT("Prototype_Component_Particle_MeteorExplosion1")
#define	PARTICLE_METEOREXPLOSION2_TAG		TEXT("Prototype_Component_Particle_MeteorExplosion2")
#define	PARTICLE_METEOREXPLOSION3_TAG		TEXT("Prototype_Component_Particle_MeteorExplosion3")

#pragma endregion

#pragma endregion

#pragma region 아이템

#define	MODEL_SPEAR_TAG			TEXT("Prototype_Component_Sub_Spear")
#define	MODEL_SHOVEL_TAG		TEXT("Prototype_Component_Sub_Shovel")

#define	MODEL_HELICOBULLET_TAG	TEXT("Prototype_Component_Model_HelicoBullet")

#define	MODEL_BOMB_TAG			TEXT("Prototype_Component_Model_Bomb")

#pragma endregion

#pragma endregion

#pragma region 기타 컴포넌트

#define COM_STATEMACHINE_TAG	TEXT("Prototype_Component_StateMachine")
#define COM_AI_TAG				TEXT("Prototype_Component_AI")

#define	COM_NAVIGATION_DEMO_TAG		TEXT("Prototype_Component_Navigation_Demo")
#define	COM_NAVIGATION_TAG			TEXT("Prototype_Component_Navi")
#define	COM_NAVIGATION2_TAG			TEXT("Prototype_Component_Navi2")
#define	COM_NAVIGATION3_TAG			TEXT("Prototype_Component_Navi3")

#define	COM_RIGIDBODY_TAG			TEXT("Prototype_Component_RigidBody")

#define	COM_COLLIDER_TAG			TEXT("Prototype_Component_Collider")

#define	COM_CONTROLLER_TAG			TEXT("Prototype_Component_Controller")

#pragma endregion

#pragma region 텍스쳐

#define TEX_BACKGROUND_TAG		TEXT("Prototype_Component_Texture_Default")

#define TEX_TERRAIN_TAG			TEXT("Prototype_Component_Texture_Tile")

#define TEX_TERRAIN_MASK_TAG	TEXT("Prototype_Component_Texture_Terrain_Mask")

#define TEX_TERRAIN_BRUSH_TAG	TEXT("Prototype_Component_Texture_Terrain_Brush")

#define TEX_SKYBOX_TAG			TEXT("Prototype_Component_Texture_SkyBox")

#define TEX_LANDSCAPE_TAG		TEXT("Prototype_Component_Texture_groundCrackedDirt")

#pragma region 이펙트 텍스쳐

#define TEX_BUBLE_TAG		TEXT("Prototype_Component_Texture_Buble")

#define TEX_CONFETTISF_TAG		TEXT("Prototype_Component_Texture_ConfettisF")
#define TEX_CONFETTISS_TAG		TEXT("Prototype_Component_Texture_ConfettisS")
#define TEX_CONFETTIST_TAG		TEXT("Prototype_Component_Texture_ConfettisT")

#define TEX_SMOKEPUFF_TAG		TEXT("Prototype_Component_Texture_SmokePuff")
#define TEX_SMOKEPUFFMUSH_TAG	TEXT("Prototype_Component_Texture_SmokePuffMush")

#define TEX_SNOW_TAG		TEXT("Prototype_Component_Texture_Snow")

#define TEX_ELEC_TAG		TEXT("Prototype_Component_Texture_ElecParts")

#define TEX_STRAW_TAG		TEXT("Prototype_Component_Texture_Straw")

#define TEX_DAMAGEIMPACT_TAG		TEXT("Prototype_Component_Texture_DamageImpact")

#define TEX_LANTERNHALO_TAG		TEXT("Prototype_Component_Texture_LanternHalo")

#define TEX_SOUL_TAG		TEXT("Prototype_Component_Texture_SoulTex")

#define TEX_REAPER_TAG		TEXT("Prototype_Component_Texture_Reaper")

#define TEX_BATSPRITE_TAG		TEXT("Prototype_Component_Texture_BatSprite")

#define TEX_FEATHER_TAG		TEXT("Prototype_Component_Texture_Feather")

#define TEX_LASER_TAG		TEXT("Prototype_Component_Texture_Laser")

#pragma endregion

#pragma endregion

#pragma region 원형객체

#define G0_BACKGROUND_TAG		TEXT("Prototype_GameObject_BackGround")

#define GO_GAMEOBJECTTEST_TAG	TEXT("Prototype_GameObject_GameObject_Test")

#define G0_TERRAIN_TAG			TEXT("Prototype_GameObject_Terrain")
#define G0_TERRAIN2_TAG			TEXT("Prototype_GameObject_Terrain2")

#define G0_DCAMERA_TAG			TEXT("Prototype_GameObject_DynamicCamera")
#define GO_TARGETCAMERA_TAG		TEXT("Prototype_GameObject_TargetCamera")

#define G0_TERRAIN_DEMO_TAG		TEXT("Prototype_GameObject_Terrain_Demo")
#define G0_OBJECTMESH_DEMO_TAG	TEXT("Prototype_GameObject_ObjectMesh_Demo")
#define G0_ANIMMESH_DEMO_TAG	TEXT("Prototype_GameObject_Animmesh_Demo")
#define G0_SUBMESH_DEMO_TAG		TEXT("Prototype_GameObject_Submesh_Demo")
#define G0_PARTICLE_DEMO_TAG		TEXT("Prototype_GameObject_Particle_Demo")

#define GO_PLATEFORM_TAG		TEXT("Prototype_GameObject_Plateform")
#define GO_PLATEFORM_INSTANCING_TAG		TEXT("Prototype_GameObject_PlateformInstancing")
#define GO_PLATEFORM_TRAP_TAG		TEXT("Prototype_GameObject_PlateformTrap")

#define GO_PARTICLENORMAL_TAG			TEXT("Prototype_GameObject__ParticleNormal")
#define GO_PARTICLESPRITE_TAG			TEXT("Prototype_GameObject__ParticleSprite")
#define GO_PARTICLESUB_TAG			TEXT("Prototype_GameObject__ParticleSub")
#define GO_PARTICLEATTACK_TAG			TEXT("Prototype_GameObject__ParticleAttack")
#define GO_PARTICLEALWAYS_TAG			TEXT("Prototype_GameObject__ParticleAlways")

#define GO_EFFECTDAMAGE_TAG			TEXT("Prototype_GameObject_EffectDamage")
#define GO_EFFECTHALO_TAG			TEXT("Prototype_GameObject_EffectHalo")
#define GO_EFFECTSOUL_TAG			TEXT("Prototype_GameObject_EffectSoul")
#define GO_EFFECTREAPER_TAG			TEXT("Prototype_GameObject_EffectReaper")
#define GO_EFFECTLASER_TAG			TEXT("Prototype_GameObject_EffectLaser")
#define GO_EFFECTDASHROAD_TAG		TEXT("Prototype_GameObject_DashRoad")
#define GO_EFFECTENERGY_TAG			TEXT("Prototype_GameObject_Energy")
#define GO_EFFECTTARGET_TAG			TEXT("Prototype_GameObject_Target")

#define GO_SKYBOX_TAG			TEXT("Prototype_GameObject_SkyBox")

#define GO_TRIGGER_TAG			TEXT("Prototype_GameObject_Trigger")

#define GO_NORMAL_BULLET_TAG			TEXT("Prototype_GameObject_NormalBullet")
#define GO_RANGE_BULLET_TAG			TEXT("Prototype_GameObject_RangeBullet")
#define GO_SHOCK_WAVE_TAG			TEXT("Prototype_GameObject_ShockWave")
#define GO_LASER_TAG			TEXT("Prototype_GameObject_Laser")
#define GO_MULTIPLY_TAG			TEXT("Prototype_GameObject_Mutiply")
#define GO_BOMB_TAG			TEXT("Prototype_GameObject_Bomb")
#define GO_TARGETBULLET_TAG			TEXT("Prototype_GameObject_TargetBullet")
#define GO_METEOR_TAG			TEXT("Prototype_GameObject_Meteor")

#pragma region 캐릭터

//#define GO_PLAYER_TAG			TEXT("Prototype_GameObject_PLAYER")
#define GO_PLAYER_BODY_TAG		TEXT("Prototype_GameObject_PLAYER_Body")
#define GO_PLAYER_SPEAR_TAG		TEXT("Prototype_GameObject_PLAYER_Spear")
#define GO_PLAYER_SHOVEL_TAG	TEXT("Prototype_GameObject_PLAYER_Shovel")

#pragma endregion

#pragma region 몬스터

//#define GO_SPOOKETON_TAG		TEXT("Prototype_GameObject_SPOOKETON")


#pragma endregion

#pragma endregion

#pragma region 폰트

#define FONT_139EX_TAG	TEXT("../Bin/Resources/Fonts/139ex.spritefont")

#pragma endregion