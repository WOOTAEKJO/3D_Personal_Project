#pragma once


#pragma region 데이터 파일 주소

#define DATA_PATH					TEXT("../Bin/Data/")
#define SHADER_PATH					TEXT("../Bin/ShaderFiles/")
#define RESOURCE_PATH				TEXT("../Bin/Resources/")

#pragma endregion	

#pragma region 제이슨 파일 주소

#define	JSON_STAGE1_PATH			"../Bin/Data/Object/Stage1.json"

#pragma endregion

#pragma region 버퍼

#define BUFFER_RECT_TAG				TEXT("Prototype_Component_VIBuffer_Rect")

#define BUFFER_TERRAIN_TAG			TEXT("Prototype_Component_VIBuffer_Terrain")
#define BUFFER_TERRAIN_HEIGHT_PATH	TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")
#define BUFFER_TERRAIN_BIN_PATH		TEXT("../Bin/Data/Terrain/Terrain.bin")

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

#define SHADER_POS_PATH			TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl")
#define SHADER_NOR_PATH			TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl")
#define SHADER_BTN_PATH			TEXT("../Bin/ShaderFiles/Shader_VtxTBN.hlsl")
#define	SHADER_MESH_PATH		TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl")
#define	SHADER_ANIMMESH_PATH	TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl")
#define	SHADER_CUBE_PATH		TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl")

#pragma endregion

#pragma region 모델

#pragma region 애니메이션 모델

#define	ANIMMODEL_FIONA_TAG			TEXT("Prototype_Component_AnimModel_Fiona")
#define	ANIMMODEL_FIONA_PATH		"../Bin/Resources/Models/AnimModel/Fiona/Fiona.bin"

#define	ANIMMODEL_RAT_TAG		TEXT("Prototype_Component_AnimModel_Rat")
#define	ANIMMODEL_RAT_PATH		"../Bin/Resources/Models/AnimModel/Rat/Rat.fbx"

#define	ANIMMODEL_BAT_TAG		TEXT("Prototype_Component_AnimModel_Bat")
#define	ANIMMODEL_BAT_PATH		"../Bin/Resources/Models/AnimModel/Bat/Bat.fbx"

#define	ANIMMODEL_JACK_TAG		TEXT("Prototype_Component_AnimModel_Jack")
#define	ANIMMODEL_JACK_PATH		"../Bin/Resources/Models/AnimModel/Jack/Jack.bin"

#define	ANIMMODEL_TEST_TAG			TEXT("Prototype_Component_AnimModel_Test")
#define	ANIMMODEL_TEST_PATH		"../Bin/Resources/Models/AnimModel/Test/Test.bin"

#pragma endregion

#pragma region 일반모델

#pragma region 포레스트

#define	MODEL_PINETREE_TAG			TEXT("Prototype_Component_Model_PineTree")
#define	MODEL_PINETREE_PATH			"../Bin/Resources/Models/NonAnimModel/Forest/PineTree/PineTree.bin"

#define	MODEL_SM_REED1_TAG			TEXT("Prototype_Component_Model_SMReed1")
#define	MODEL_SM_REED1_PATH			"../Bin/Resources/Models/NonAnimModel/Forest/SMReed1/SMReed1.bin"

#define	MODEL_SM_REED2_TAG			TEXT("Prototype_Component_Model_SMReed2")
#define	MODEL_SM_REED2_PATH			"../Bin/Resources/Models/NonAnimModel/Forest/SMReed2/SMReed2.bin"

#define	MODEL_DEADTREE2_TAG			TEXT("Prototype_Component_Model_DeadTree2")
#define	MODEL_DEADTREE2_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree2/DeadTree2.bin"

#define	MODEL_DEADTREE3_TAG			TEXT("Prototype_Component_Model_DeadTree3")
#define	MODEL_DEADTREE3_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree3/DeadTree3.bin"

#define	MODEL_DEADTREE4_TAG			TEXT("Prototype_Component_Model_DeadTree4")
#define	MODEL_DEADTREE4_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree4/DeadTree4.bin"

#define	MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree01aTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree01aTrunck/DeadTree01aTrunck.bin"

#define	MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree01bTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree01bTrunck/DeadTree01bTrunck.bin"

#define	MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree01cTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree01cTrunck/DeadTree01cTrunck.bin"

#define	MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree02aTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree02aTrunck/DeadTree02aTrunck.bin"

#define	MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree02bTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree02bTrunck/DeadTree02bTrunck.bin"

#define	MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree02cTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree02cTrunck/DeadTree02cTrunck.bin"

#define	MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree03aTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree03aTrunck/DeadTree03aTrunck.bin"

#define	MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree03bTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree03bTrunck/DeadTree03bTrunck.bin"

#define	MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree03cTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree03cTrunck/DeadTree03cTrunck.bin"

#define	MODEL_FORESTTRUNK_TAG			TEXT("Prototype_Component_Model_ForestTrunk")
#define	MODEL_FORESTTRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/ForestTrunk/ForestTrunk.bin"

#define	MODEL_SM_CPINE1_TAG			TEXT("Prototype_Component_Model_SMCpine1")
#define	MODEL_SM_CPINE1_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMCpine1/SMCpine1.bin"

#define	MODEL_SM_CPINE2_TAG			TEXT("Prototype_Component_Model_SMCpine2")
#define	MODEL_SM_CPINE2_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMCpine2/SMCpine2.bin"

#define	MODEL_SM_CPINE3_TAG			TEXT("Prototype_Component_Model_SMCpine3")
#define	MODEL_SM_CPINE3_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMCpine3/SMCpine3.bin"

#define	MODEL_SM_MAPLE1_TAG			TEXT("Prototype_Component_Model_SMMaple1")
#define	MODEL_SM_MAPLE1_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMMaple1/SMMaple1.bin"

#define	MODEL_SM_OWLTREE_TAG			TEXT("Prototype_Component_Model_SMOwlTree")
#define	MODEL_SM_OWLTREE_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMOwlTree/SMOwlTree.bin"

#define	MODEL_SM_PINE1_TAG			TEXT("Prototype_Component_Model_SMPine1")
#define	MODEL_SM_PINE1_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMPine1/SMPine1.bin"

#define	MODEL_SM_ROOTS_TAG			TEXT("Prototype_Component_Model_SMRoots")
#define	MODEL_SM_ROOTS_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMRoots/SMRoots.bin"

#define	MODEL_SM_ROOTS1_TAG			TEXT("Prototype_Component_Model_SMRoots1")
#define	MODEL_SM_ROOTS1_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMRoots1/SMRoots1.bin"

#define	MODEL_SM_ROOTS2_TAG			TEXT("Prototype_Component_Model_SMRoots2")
#define	MODEL_SM_ROOTS2_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMRoots2/SMRoots2.bin"

#define	MODEL_SM_ROOTS3_TAG			TEXT("Prototype_Component_Model_SMRoots3")
#define	MODEL_SM_ROOTS3_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMRoots3/SMRoots3.bin"

#define	MODEL_SM_ROOTS4_TAG			TEXT("Prototype_Component_Model_SMRoots4")
#define	MODEL_SM_ROOTS4_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMRoots4/SMRoots4.bin"

#define	MODEL_SM_ROOTS5_TAG			TEXT("Prototype_Component_Model_SMRoots5")
#define	MODEL_SM_ROOTS5_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMRoots5/SMRoots5.bin"

#define	MODEL_TREE_PLATEFORME01X01_TAG			TEXT("Prototype_Component_Model_TreePlateforme01x01")
#define	MODEL_TREE_PLATEFORME01X01_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/TreePlateforme01x01/TreePlateforme01x01.bin"

#define	MODEL_TRUNKBREAK_TAG			TEXT("Prototype_Component_Model_TrunckBreak")
#define	MODEL_TRUNKBREAK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/TrunckBreak/TrunckBreak.bin"

#define	MODEL_WOODSTOMP_TAG			TEXT("Prototype_Component_Model_WoodStomp")
#define	MODEL_WOODSTOMP_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/WoodStomp/WoodStomp.bin"
#pragma endregion

#pragma region 아이템

#define	MODEL_SPEAR_TAG			TEXT("Prototype_Component_Sub_Spear")
#define	MODEL_SPEAR_PATH		"../Bin/Resources/Models/NonAnimModel/Weapon/Spear/Spear.bin"

#pragma endregion

#pragma endregion

#pragma region 기타 컴포넌트

#define COM_STATEMACHINE_TAG	TEXT("Prototype_Component_StateMachine")
#define COM_AI_TAG				TEXT("Prototype_Component_AI")

#define	COM_NAVIGATION_DEMO_TAG		TEXT("Prototype_Component_Navigation_Demo")
#define	COM_NAVIGATION_TAG			TEXT("Prototype_Component_Navi")
#define COM_NAVOGATION_PAHT		"../Bin/Data/Terrain/Navigation/Navi.bin"

#define	COM_RIGIDBODY_TAG			TEXT("Prototype_Component_RigidBody")

#define	COM_COLLIDER_TAG			TEXT("Prototype_Component_Collider")

#pragma endregion

#pragma region 텍스쳐

#define TEX_BACKGROUND_TAG		TEXT("Prototype_Component_Texture_Default")
#define TEX_BACKGROUND_PATH		TEXT("../Bin/Resources/Textures/Default%d.jpg")

#define TEX_TERRAIN_TAG			TEXT("Prototype_Component_Texture_Tile")
#define TEX_TERRAIN_PATH		TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds")

#define TEX_TERRAIN_MASK_TAG	TEXT("Prototype_Component_Texture_Terrain_Mask")
#define TEX_TERRAIN_MASK_PATH	TEXT("../Bin/Resources/Textures/Terrain/Mask.bmp")

#define TEX_TERRAIN_BRUSH_TAG	TEXT("Prototype_Component_Texture_Terrain_Brush")
#define TEX_TERRAIN_BRUSH_PATH	TEXT("../Bin/Resources/Textures/Terrain/Brush.png")

#define TEX_SKYBOX_TAG	TEXT("Prototype_Component_Texture_SkyBox")
#define TEX_SKYBOX_PATH	TEXT("../Bin/Resources/Textures/SkyBox/SkyBox%d.dds")

#pragma endregion

#pragma region 원형객체

#define G0_BACKGROUND_TAG		TEXT("Prototype_GameObject_BackGround")

#define GO_GAMEOBJECTTEST_TAG	TEXT("Prototype_GameObject_GameObject_Test")

#define G0_TERRAIN_TAG			TEXT("Prototype_GameObject_Terrain")

#define G0_DCAMERA_TAG			TEXT("Prototype_GameObject_DynamicCamera")

#define G0_TERRAIN_DEMO_TAG		TEXT("Prototype_GameObject_Terrain_Demo")
#define G0_OBJECTMESH_DEMO_TAG	TEXT("Prototype_GameObject_ObjectMesh_Demo")
#define G0_ANIMMESH_DEMO_TAG	TEXT("Prototype_GameObject_Animmesh_Demo")
#define G0_SUBMESH_DEMO_TAG		TEXT("Prototype_GameObject_Submesh_Demo")

#define GO_PLATEFORM_TAG		TEXT("Prototype_GameObject_Plateform")

#define GO_SKYBOX_TAG			TEXT("Prototype_GameObject_SkyBox")

#pragma region 캐릭터

#define GO_PLAYER_TAG		TEXT("Prototype_GameObject_PLAYER")
#define GO_PLAYER_BODY_TAG		TEXT("Prototype_GameObject_PLAYER_Body")
#define GO_PLAYER_SPEAR_TAG		TEXT("Prototype_GameObject_PLAYER_Spear")

#pragma endregion

#pragma endregion