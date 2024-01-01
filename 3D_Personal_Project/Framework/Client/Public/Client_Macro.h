#pragma once


#pragma region 데이터 파일 주소

#define FILE_PATH					TEXT("../Bin/")

#pragma endregion	

#pragma region 제이슨 파일 주소

#define	JSON_STAGE1_PATH			"../Bin/Data/Object/Stage1.json"

#pragma endregion

#pragma region 버퍼

#define BUFFER_RECT_TAG				TEXT("Prototype_Component_VIBuffer_Rect")

#define BUFFER_TERRAIN_TAG			TEXT("Prototype_Component_VIBuffer_Terrain")
#define BUFFER_TERRAIN_HEIGHT_PATH	TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")

#define BUFFER_DTERRAIN_TAG			TEXT("Prototype_Component_VIBuffer_DTerrain")

#define BUFFER_CUBE_TAG				TEXT("Prototype_Component_VIBuffer_Cube")

#pragma endregion

#pragma region 쉐이더

#define	SHADER_POS_TAG			TEXT("Prototype_Component_Shader_VtxPosTex")
#define SHADER_NOR_TAG			TEXT("Prototype_Component_Shader_VtxNorTex")
#define	SHADER_BTN_TAG			TEXT("Prototype_Component_Shader_VtxTBN")
#define	SHADER_MESH_TAG			TEXT("Prototype_Component_Shader_VtxMesh")
#define	SHADER_ANIMMESH_TAG		TEXT("Prototype_Component_Shader_VtxAnimMesh")
#define	SHADER_CUBE_TAG			TEXT("Prototype_Component_Shader_VtxCube")

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

#pragma region 아이템

#define	MODEL_SPEAR_TAG			TEXT("Prototype_Component_Sub_Spear")
#define	MODEL_SHOVEL_TAG		TEXT("Prototype_Component_Sub_Shovel")

#pragma endregion

#pragma endregion

#pragma region 기타 컴포넌트

#define COM_STATEMACHINE_TAG	TEXT("Prototype_Component_StateMachine")
#define COM_AI_TAG				TEXT("Prototype_Component_AI")

#define	COM_NAVIGATION_DEMO_TAG		TEXT("Prototype_Component_Navigation_Demo")
#define	COM_NAVIGATION_TAG			TEXT("Prototype_Component_Navi")

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

#pragma endregion

#pragma region 원형객체

#define G0_BACKGROUND_TAG		TEXT("Prototype_GameObject_BackGround")

#define GO_GAMEOBJECTTEST_TAG	TEXT("Prototype_GameObject_GameObject_Test")

#define G0_TERRAIN_TAG			TEXT("Prototype_GameObject_Terrain")

#define G0_DCAMERA_TAG			TEXT("Prototype_GameObject_DynamicCamera")
#define GO_TARGETCAMERA_TAG		TEXT("Prototype_GameObject_TargetCamera")

#define G0_TERRAIN_DEMO_TAG		TEXT("Prototype_GameObject_Terrain_Demo")
#define G0_OBJECTMESH_DEMO_TAG	TEXT("Prototype_GameObject_ObjectMesh_Demo")
#define G0_ANIMMESH_DEMO_TAG	TEXT("Prototype_GameObject_Animmesh_Demo")
#define G0_SUBMESH_DEMO_TAG		TEXT("Prototype_GameObject_Submesh_Demo")

#define GO_PLATEFORM_TAG		TEXT("Prototype_GameObject_Plateform")

#define GO_SKYBOX_TAG			TEXT("Prototype_GameObject_SkyBox")

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