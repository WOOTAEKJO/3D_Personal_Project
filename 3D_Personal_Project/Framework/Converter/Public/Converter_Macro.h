#pragma once
#pragma once


#pragma region 데이터 파일 주소

#define FILE_PATH					TEXT("../Bin/")
#define DATA_NONANIM_PATH			"../Bin/Data/NonAnim/"
#define DATA_ANIM_PATH			"../Bin/Data/Anim/"
//TEXT("C:/Users/Git/SourcTree/3D_Personal_Project/3D_Personal_Project/Framework/Client/Bin/Data/Terrain")

#pragma endregion	

#pragma region 버퍼

#define BUFFER_RECT_TAG				TEXT("Prototype_Component_VIBuffer_Rect")

#define BUFFER_TERRAIN_TAG			TEXT("Prototype_Component_VIBuffer_Terrain")
#define BUFFER_TERRAIN_HEIGHT_PATH	TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")

#define BUFFER_DTERRAIN_TAG			TEXT("Prototype_Component_VIBuffer_DTerrain")

#define BUFFER_CUBE_TAG				TEXT("Prototype_Component_VIBuffer_Cube")

#pragma endregion

#pragma region 쉐이더

#define	SHADER_POS_TAG			TEXT("Prototype_Component_Shader_VTXPOSTEX")
#define SHADER_NOR_TAG			TEXT("Prototype_Component_Shader_VTXNORTEX")
#define	SHADER_BTN_TAG			TEXT("Prototype_Component_Shader_VTXTBN")
#define	SHADER_MESH_TAG			TEXT("Prototype_Component_Shader_VTXMESH")
#define	SHADER_ANIMMESH_TAG		TEXT("Prototype_Component_Shader_VTXANIMMESH")

#define SHADER_POS_PATH			TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl")
#define SHADER_NOR_PATH			TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl")
#define SHADER_BTN_PATH			TEXT("../Bin/ShaderFiles/Shader_VtxTBN.hlsl")
#define	SHADER_MESH_PATH		TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl")
#define	SHADER_ANIMMESH_PATH	TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl")

#pragma endregion

#pragma region 모델

#pragma region 애니메이션 모델

#define	ANIMMODEL_FIONA_TAG			TEXT("Prototype_Component_AnimModel_Fiona")
#define	ANIMMODEL_FIONA_PATH		"../Bin/Resources/Models/AnimModel/Fiona/Fiona.fbx"

#define	ANIMMODEL_RAT_TAG		TEXT("Prototype_Component_AnimModel_Rat")
#define	ANIMMODEL_RAT_PATH		"../Bin/Resources/Models/AnimModel/Rat/Rat.fbx"

#define	ANIMMODEL_BAT_TAG		TEXT("Prototype_Component_AnimModel_Bat")
#define	ANIMMODEL_BAT_PATH		"../Bin/Resources/Models/AnimModel/Bat/Bat.fbx"

#define	ANIMMODEL_JACK_TAG		TEXT("Prototype_Component_AnimModel_Jack")
#define	ANIMMODEL_JACK_PATH		"../Bin/Resources/Models/AnimModel/Jack/Jack.fbx"

#define	ANIMMODEL_TEST_TAG			TEXT("Prototype_Component_AnimModel_Test")
#define	ANIMMODEL_TEST_PATH		"../Bin/Resources/Test/BaseHumanSkeleton.fbx"

#define	ANIMMODEL_HELICOSCARROW_TAG			TEXT("Prototype_Component_AnimModel_HelicoScarrow")
#define	ANIMMODEL_HELICOSCARROW_PATH			"../Bin/Resources/Models/AnimModel/HelicoScarrow/HelicoScarrow.fbx"

#define	ANIMMODEL_SKULLCROSSBOW_TAG			TEXT("Prototype_Component_AnimModel_SkullCrossBow")
#define	ANIMMODEL_SKULLCROSSBOW_PATH			"../Bin/Resources/Models/AnimModel/SkullCrossBow/SkullCrossBow.fbx"

#define	ANIMMODEL_SPOOKETON_TAG				TEXT("Prototype_Component_AnimModel_Spooketon")
#define	ANIMMODEL_SPOOKETON_PATH			"../Bin/Resources/Models/AnimModel/Spooketon/Spooketon.fbx"

#define	ANIMMODEL_CROW_TAG				TEXT("Prototype_Component_AnimModel_Crow")
#define	ANIMMODEL_CROW_PATH			"../Bin/Resources/Models/AnimModel/Crow/Crow.fbx"

#define	ANIMMODEL_OWL_TAG				TEXT("Prototype_Component_AnimModel_Owl")
#define	ANIMMODEL_OWL_PATH			"../Bin/Resources/Models/AnimModel/Owl/Owl.fbx"

#pragma endregion

#pragma region 일반모델

#pragma region 포레스트

#define	MODEL_PINETREE_TAG			TEXT("Prototype_Component_Model_PineTree")
#define	MODEL_PINETREE_PATH			"../Bin/Resources/Models/NonAnimModel/Forest/PineTree/PineTree.fbx"

#define	MODEL_SM_REED1_TAG			TEXT("Prototype_Component_Model_SMReed1")
#define	MODEL_SM_REED1_PATH			"../Bin/Resources/Models/NonAnimModel/Forest/SMReed1/SMReed1.fbx"

#define	MODEL_SM_REED2_TAG			TEXT("Prototype_Component_Model_SMReed2")
#define	MODEL_SM_REED2_PATH			"../Bin/Resources/Models/NonAnimModel/Forest/SMReed2/SMReed2.fbx"

#define	MODEL_DEADTREE2_TAG			TEXT("Prototype_Component_Model_DeadTree2")
#define	MODEL_DEADTREE2_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree2/DeadTree2.fbx"

#define	MODEL_DEADTREE3_TAG			TEXT("Prototype_Component_Model_DeadTree3")
#define	MODEL_DEADTREE3_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree3/DeadTree3.fbx"

#define	MODEL_DEADTREE4_TAG			TEXT("Prototype_Component_Model_DeadTree4")
#define	MODEL_DEADTREE4_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree4/DeadTree4.fbx"

#define	MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree01aTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_01_A_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree01aTrunck/DeadTree01aTrunck.fbx"

#define	MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree01bTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_01_B_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree01bTrunck/DeadTree01bTrunck.fbx"

#define	MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree01cTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_01_C_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree01cTrunck/DeadTree01cTrunck.fbx"

#define	MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree02aTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_02_A_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree02aTrunck/DeadTree02aTrunck.fbx"

#define	MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree02bTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_02_B_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree02bTrunck/DeadTree02bTrunck.fbx"

#define	MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree02cTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_02_C_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree02cTrunck/DeadTree02cTrunck.fbx"

#define	MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree03aTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_03_A_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree03aTrunck/DeadTree03aTrunck.fbx"

#define	MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree03bTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_03_B_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree03bTrunck/DeadTree03bTrunck.fbx"

#define	MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_TAG			TEXT("Prototype_Component_Model_DeadTree03cTrunck")
#define	MODEL_DEADTREES_DEAD_TREE_03_C_TRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/DeadTree03cTrunck/DeadTree03cTrunck.fbx"

#define	MODEL_FORESTTRUNK_TAG			TEXT("Prototype_Component_Model_ForestTrunk")
#define	MODEL_FORESTTRUNK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/ForestTrunk/ForestTrunk.fbx"

#define	MODEL_SM_CPINE1_TAG			TEXT("Prototype_Component_Model_SMCpine1")
#define	MODEL_SM_CPINE1_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMCpine1/SMCpine1.fbx"

#define	MODEL_SM_CPINE2_TAG			TEXT("Prototype_Component_Model_SMCpine2")
#define	MODEL_SM_CPINE2_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMCpine2/SMCpine2.fbx"

#define	MODEL_SM_CPINE3_TAG			TEXT("Prototype_Component_Model_SMCpine3")
#define	MODEL_SM_CPINE3_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMCpine3/SMCpine3.fbx"

#define	MODEL_SM_MAPLE1_TAG			TEXT("Prototype_Component_Model_SMMaple1")
#define	MODEL_SM_MAPLE1_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMMaple1/SMMaple1.fbx"

#define	MODEL_SM_OWLTREE_TAG			TEXT("Prototype_Component_Model_SMOwlTree")
#define	MODEL_SM_OWLTREE_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMOwlTree/SMOwlTree.fbx"

#define	MODEL_SM_PINE1_TAG			TEXT("Prototype_Component_Model_SMPine1")
#define	MODEL_SM_PINE1_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMPine1/SMPine1.fbx"

#define	MODEL_SM_ROOTS_TAG			TEXT("Prototype_Component_Model_SMRoots")
#define	MODEL_SM_ROOTS_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMRoots/SMRoots.fbx"

#define	MODEL_SM_ROOTS1_TAG			TEXT("Prototype_Component_Model_SMRoots1")
#define	MODEL_SM_ROOTS1_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMRoots1/SMRoots1.fbx"

#define	MODEL_SM_ROOTS2_TAG			TEXT("Prototype_Component_Model_SMRoots2")
#define	MODEL_SM_ROOTS2_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMRoots2/SMRoots2.fbx"

#define	MODEL_SM_ROOTS3_TAG			TEXT("Prototype_Component_Model_SMRoots3")
#define	MODEL_SM_ROOTS3_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMRoots3/SMRoots3.fbx"

#define	MODEL_SM_ROOTS4_TAG			TEXT("Prototype_Component_Model_SMRoots4")
#define	MODEL_SM_ROOTS4_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMRoots4/SMRoots4.fbx"

#define	MODEL_SM_ROOTS5_TAG			TEXT("Prototype_Component_Model_SMRoots5")
#define	MODEL_SM_ROOTS5_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/SMRoots5/SMRoots5.fbx"

#define	MODEL_TREE_PLATEFORME01X01_TAG			TEXT("Prototype_Component_Model_TreePlateforme01x01")
#define	MODEL_TREE_PLATEFORME01X01_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/TreePlateforme01x01/TreePlateforme01x01.fbx"

#define	MODEL_TRUNKBREAK_TAG			TEXT("Prototype_Component_Model_TrunckBreak")
#define	MODEL_TRUNKBREAK_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/TrunckBreak/TrunckBreak.fbx"

#define	MODEL_WOODSTOMP_TAG			TEXT("Prototype_Component_Model_WoodStomp")
#define	MODEL_WOODSTOMP_PATH		"../Bin/Resources/Models/NonAnimModel/Forest/WoodStomp/WoodStomp.fbx"
#pragma endregion

#pragma region 바위

#define	MODEL_CAVETUNNELFORWORAD2_TAG			TEXT("Prototype_Component_Model_caveTunnelForward2")
#define	MODEL_CAVETUNNELFORWORAD2_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/caveTunnelForward2.fbx"

#define	MODEL_MINECAVE_TAG						TEXT("Prototype_Component_Model_mineCave")
#define	MODEL_MINECAVE_PATH						"../Bin/Resources/Models/NonAnimModel/Rock/mineCave.fbx"

#define	MODEL_ROCKCLUSTER2_TAG						TEXT("Prototype_Component_Model_rockCluster2")
#define	MODEL_ROCKCLUSTER2_PATH						"../Bin/Resources/Models/NonAnimModel/Rock/rockCluster2.fbx"

#define	MODEL_ROCKBIGFORMATION3_TAG				TEXT("Prototype_Component_Model_rockBigFormation3")
#define	MODEL_ROCKBIGFORMATION3_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/rockBigFormation3.fbx"

#define	MODEL_ROCKBIGFORMATION6_TAG				TEXT("Prototype_Component_Model_rockBigFormation6")
#define	MODEL_ROCKBIGFORMATION6_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/rockBigFormation6.fbx"

#define	MODEL_ROCKBIGFORMATION8_TAG				TEXT("Prototype_Component_Model_rockBigFormation8")
#define	MODEL_ROCKBIGFORMATION8_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/rockBigFormation8.fbx"

#define	MODEL_ROCKPACK1_TAG				TEXT("Prototype_Component_Model_rockPack1")
#define	MODEL_ROCKPACK1_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/rockPack1.fbx"

#define	MODEL_ROCKPACK2_TAG				TEXT("Prototype_Component_Model_rockPack2")
#define	MODEL_ROCKPACK2_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/rockPack2.fbx"

#define	MODEL_ROCKPACK2COL_TAG				TEXT("Prototype_Component_Model_rockPack2Col")
#define	MODEL_ROCKPACK2COL_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/rockPack2Col.fbx"

#define	MODEL_ROCKPACK3_TAG				TEXT("Prototype_Component_Model_rockPack3")
#define	MODEL_ROCKPACK3_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/rockPack3.fbx"

#define	MODEL_ROCKPACK4_TAG				TEXT("Prototype_Component_Model_rockPack4")
#define	MODEL_ROCKPACK4_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/rockPack4.fbx"

#define	MODEL_ROCKPACK5_TAG				TEXT("Prototype_Component_Model_rockPack5")
#define	MODEL_ROCKPACK5_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/rockPack5.fbx"

#define	MODEL_ROCKPACK52_TAG				TEXT("Prototype_Component_Model_rockPack52")
#define	MODEL_ROCKPACK52_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/rockPack52.fbx"

#define	MODEL_ROCKPACK6_TAG				TEXT("Prototype_Component_Model_rockPack6")
#define	MODEL_ROCKPACK6_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/rockPack6.fbx"

#define	MODEL_ROCKPACK7_TAG				TEXT("Prototype_Component_Model_rockPack7")
#define	MODEL_ROCKPACK7_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/rockPack7.fbx"

#define	MODEL_SMALLROCK_TAG				TEXT("Prototype_Component_Model_SmallRock")
#define	MODEL_SMALLROCK_PATH			"../Bin/Resources/Models/NonAnimModel/Rock/SmallRock.fbx"

#pragma endregion

#pragma region 데코

#define	MODEL_BARREL_TAG			TEXT("Prototype_Component_Model_barrel")
#define	MODEL_BARREL_PATH			"../Bin/Resources/Models/NonAnimModel/Deco/barrel.fbx"

#define	MODEL_CAULDRON_TAG			TEXT("Prototype_Component_Model_Cauldron")
#define	MODEL_CAULDRON_PATH			"../Bin/Resources/Models/NonAnimModel/Deco/Cauldron.fbx"

#define	MODEL_EDGEROPE_TAG			TEXT("Prototype_Component_Model_EdgeRope")
#define	MODEL_EDGEROPE_PATH			"../Bin/Resources/Models/NonAnimModel/Deco/EdgeRope.fbx"

#define	MODEL_FENCEL_TAG			TEXT("Prototype_Component_Model_Fencel")
#define	MODEL_FENCEL_PATH			"../Bin/Resources/Models/NonAnimModel/Deco/Fencel.fbx"

#define	MODEL_FLOORLAMP_TAG			TEXT("Prototype_Component_Model_FloorLamp")
#define	MODEL_FLOORLAMP_PATH		"../Bin/Resources/Models/NonAnimModel/Deco/FloorLamp.fbx"

#define	MODEL_GRASSMESHBIG_TAG			TEXT("Prototype_Component_Model_GrassMeshBig")
#define	MODEL_GRASSMESHBIG_PATH		"../Bin/Resources/Models/NonAnimModel/Deco/GrassMeshBig.fbx"

#define	MODEL_GROUNDLANTERN_TAG			TEXT("Prototype_Component_Model_GroundLantern")
#define	MODEL_GROUNDLANTERN_PATH		"../Bin/Resources/Models/NonAnimModel/Deco/GroundLantern.fbx"

#define	MODEL_HATHEAP_TAG			TEXT("Prototype_Component_Model_HayHeap")
#define	MODEL_HATHEAP_PATH		"../Bin/Resources/Models/NonAnimModel/Deco/HayHeap.fbx"

#define	MODEL_LANTERN_TAG			TEXT("Prototype_Component_Model_Lantern")
#define	MODEL_LANTERN_PATH		"../Bin/Resources/Models/NonAnimModel/Deco/Lantern.fbx"

#define	MODEL_PENDINGROPE_TAG			TEXT("Prototype_Component_Model_PendingRope")
#define	MODEL_PENDINGROPE_PATH		"../Bin/Resources/Models/NonAnimModel/Deco/PendingRope.fbx"

#define	MODEL_PURPLESCHROOMS_TAG			TEXT("Prototype_Component_Model_PurpleSchrooms")
#define	MODEL_PURPLESCHROOMS_PATH		"../Bin/Resources/Models/NonAnimModel/Deco/PurpleSchrooms.fbx"

#define	MODEL_SCOURGEALTAR_TAG			TEXT("Prototype_Component_Model_ScourgeAltar")
#define	MODEL_SCOURGEALTAR_PATH		"../Bin/Resources/Models/NonAnimModel/Deco/ScourgeAltar.fbx"

#define	MODEL_WOODFUCKER_TAG			TEXT("Prototype_Component_Model_WoodFucker")
#define	MODEL_WOODFUCKER_PATH		"../Bin/Resources/Models/NonAnimModel/Deco/WoodFucker.fbx"

#pragma endregion

#pragma region 나무

#define	MODEL_BROKENLATEFORMEWAY_TAG			TEXT("Prototype_Component_Model_brokenPlateformeWay")
#define	MODEL_BROKENLATEFORMEWAY_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/brokenPlateformeWay.fbx"

#define	MODEL_CATEBOXPILE_TAG			TEXT("Prototype_Component_Model_cateBoxPile")
#define	MODEL_CATEBOXPILE_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/cateBoxPile.fbx"

#define	MODEL_CRATEBOX_TAG			TEXT("Prototype_Component_Model_crateBox")
#define	MODEL_CRATEBOX_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/crateBox.fbx"

#define	MODEL_FENCE4_TAG			TEXT("Prototype_Component_Model_fence4")
#define	MODEL_FENCE4_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/fence4.fbx"

#define	MODEL_FENCECLOSE_TAG			TEXT("Prototype_Component_Model_fenceClose")
#define	MODEL_FENCECLOSE_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/fenceClose.fbx"

#define	MODEL_FENCESTACK_TAG			TEXT("Prototype_Component_Model_fenceStack")
#define	MODEL_FENCESTACK_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/fenceStack.fbx"

#define	MODEL_FENCESTACKSMALL_TAG			TEXT("Prototype_Component_Model_fenceStackSmall")
#define	MODEL_FENCESTACKSMALL_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/fenceStackSmall.fbx"

#define	MODEL_FENCESTACKSMALLCOL_TAG			TEXT("Prototype_Component_Model_fenceStackSmallCol")
#define	MODEL_FENCESTACKSMALLCOL_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/fenceStackSmallCol.fbx"

#define	MODEL_FENCESTACKSMALLHIGHCOL_TAG			TEXT("Prototype_Component_Model_fenceStackSmallHighCol")
#define	MODEL_FENCESTACKSMALLHIGHCOL_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/fenceStackSmallHighCol.fbx"

#define	MODEL_MINEPLATFORM_TAG			TEXT("Prototype_Component_Model_minePlatform")
#define	MODEL_MINEPLATFORM_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/minePlatform.fbx"

#define	MODEL_MINEPLATFORMFULL_TAG			TEXT("Prototype_Component_Model_minePlatformFull")
#define	MODEL_MINEPLATFORMFULL_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/minePlatformFull.fbx"

#define	MODEL_MINETOWER_TAG			TEXT("Prototype_Component_Model_mineTower")
#define	MODEL_MINETOWER_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/mineTower.fbx"

#define	MODEL_MINETOWERFULL_TAG			TEXT("Prototype_Component_Model_mineTowerFull")
#define	MODEL_MINETOWERFULL_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/mineTowerFull.fbx"

#define	MODEL_MINETOWERFULLFULL_TAG			TEXT("Prototype_Component_Model_mineTowerFullFull")
#define	MODEL_MINETOWERFULLFULL_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/mineTowerFullFull.fbx"

#define	MODEL_SMROUNDFENCE_TAG			TEXT("Prototype_Component_Model_SMRoundFence")
#define	MODEL_SMROUNDFENCE_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/SMRoundFence.fbx"

#define	MODEL_WEATHERCLOCK_TAG			TEXT("Prototype_Component_Model_weatherClock")
#define	MODEL_WEATHERCLOCK_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/weatherClock.fbx"

#define	MODEL_WEATHERCLOCKHIGH_TAG			TEXT("Prototype_Component_Model_weatherClockHigh")
#define	MODEL_WEATHERCLOCKHIGH_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/weatherClockHigh.fbx"

#define	MODEL_WOODPLATEFORME1_TAG			TEXT("Prototype_Component_Model_woodPlateforme1")
#define	MODEL_WOODPLATEFORME1_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/woodPlateforme1.fbx"

#define	MODEL_WOODPLATEFORME2_TAG			TEXT("Prototype_Component_Model_woodPlateforme2")
#define	MODEL_WOODPLATEFORME2_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/woodPlateforme2.fbx"

#define	MODEL_WOODPLATEFORME05X05X2_TAG			TEXT("Prototype_Component_Model_woodPlateforme05x05x2")
#define	MODEL_WOODPLATEFORME05X05X2_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/woodPlateforme05x05x2.fbx"

#define	MODEL_WOODPLATFORM1SMALL_TAG			TEXT("Prototype_Component_Model_woodPlatform1Small")
#define	MODEL_WOODPLATFORM1SMALL_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/woodPlatform1Small.fbx"

#define	MODEL_WOODROPEHOLDER_TAG			TEXT("Prototype_Component_Model_woodRopeHolder")
#define	MODEL_WOODROPEHOLDER_PATH			"../Bin/Resources/Models/NonAnimModel/Wood/woodRopeHolder.fbx"

#define	MODEL_WOODWALL2_TAG					TEXT("Prototype_Component_Model_woodWall2")
#define	MODEL_WOODWALL2_PATH				"../Bin/Resources/Models/NonAnimModel/Wood/woodWall2.fbx"

#pragma endregion

#pragma region 아이템

#define	MODEL_SPEAR_TAG			TEXT("Prototype_Component_Model_Spear")
#define	MODEL_SPEAR_PATH		"../Bin/Resources/Models/NonAnimModel/Weapon/Spear/Spear.fbx"

#define	MODEL_SHOVEL_TAG			TEXT("Prototype_Component_Model_Shovel")
#define	MODEL_SHOVEL_PATH		"../Bin/Resources/Models/NonAnimModel/Weapon/Shovel/Shovel.fbx"

#pragma endregion

#define	MODEL_LAND_TAG			TEXT("Prototype_Component_Model_Land")
#define	MODEL_LAND_PATH		"../Bin/Resources/Models/NonAnimModel/Land/Land.fbx"

#pragma endregion

#pragma region 기타 컴포넌트

#define COM_STATEMACHINE_TAG	TEXT("Prototype_Component_StateMachine")
#define COM_AI_TAG				TEXT("Prototype_Component_AI")

#pragma endregion

#pragma region 텍스쳐

#define TEX_BACKGROUND_TAG		TEXT("Prototype_Component_Texture_BackGround")
#define TEX_BACKGROUND_PATH		TEXT("../Bin/Resources/Textures/Default%d.jpg")

#define TEX_TERRAIN_TAG			TEXT("Prototype_Component_Texture_Terrain")
#define TEX_TERRAIN_PATH		TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds")

#define TEX_TERRAIN_MASK_TAG	TEXT("Prototype_Component_Texture_Terrain_Mask")
#define TEX_TERRAIN_MASK_PATH	TEXT("../Bin/Resources/Textures/Terrain/Mask.bmp")

#define TEX_TERRAIN_BRUSH_TAG	TEXT("Prototype_Component_Texture_Terrain_Brush")
#define TEX_TERRAIN_BRUSH_PATH	TEXT("../Bin/Resources/Textures/Terrain/Brush.png")

#pragma endregion

#pragma region 원형객체

#define G0_BACKGROUND_TAG		TEXT("Prototype_GameObject_BackGround")

#define GO_GAMEOBJECTTEST_TAG	TEXT("Prototype_GameObject_GameObject_Test")

#define G0_TERRAIN_TAG			TEXT("Prototype_GameObject_Terrain")

#define G0_TERRAIN_DEMO_TAG		TEXT("Prototype_GameObject_Terrain_Demo")

#define G0_DCAMERA_TAG			TEXT("Prototype_GameObject_DynamicCamera")

#define G0_OBJECTMESH_DEMO_TAG	TEXT("Prototype_GameObject_ObjectMesh_Demo")

#define G0_ANIMMESH_DEMO_TAG	TEXT("Prototype_GameObject_Animmesh_Demo")

#define G0_MESH_DEMO_TAG	TEXT("Prototype_GameObject_Mesh_Demo")

#pragma endregion