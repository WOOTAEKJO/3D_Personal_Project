#pragma once


#pragma region ������ ���� �ּ�

#define DATA_TERRAIN_PATH			TEXT("../Bin/Export/Debug/x64/Data/Terrain")
//TEXT("C:/Users/Git/SourcTree/3D_Personal_Project/3D_Personal_Project/Framework/Client/Bin/Export/Debug/x64/Data/Terrain")

#pragma endregion	

#pragma region ����

#define BUFFER_RECT_TAG				TEXT("Prototype_Component_VIBuffer_Rect")

#define BUFFER_TERRAIN_TAG			TEXT("Prototype_Component_VIBuffer_Terrain")
#define BUFFER_TERRAIN_HEIGHT_PATH	TEXT("../Bin/Export/Debug/x64/Resources/Textures/Terrain/Height1.bmp")

#define BUFFER_DTERRAIN_TAG			TEXT("Prototype_Component_VIBuffer_DTerrain")

#define BUFFER_CUBE_TAG				TEXT("Prototype_Component_VIBuffer_Cube")

#pragma endregion

#pragma region ���̴�

#define	SHADER_POS_TAG			TEXT("Prototype_Component_Shader_VTXPOSTEX")
#define SHADER_NOR_TAG			TEXT("Prototype_Component_Shader_VTXNORTEX")
#define	SHADER_BTN_TAG			TEXT("Prototype_Component_Shader_VTXTBN")
#define	SHADER_MESH_TAG			TEXT("Prototype_Component_Shader_VTXMESH")
#define	SHADER_ANIMMESH_TAG		TEXT("Prototype_Component_Shader_VTXANIMMESH")

#define SHADER_POS_PATH			TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxPosTex.hlsl")
#define SHADER_NOR_PATH			TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxNorTex.hlsl")
#define SHADER_BTN_PATH			TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxTBN.hlsl")
#define	SHADER_MESH_PATH		TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxMesh.hlsl")
#define	SHADER_ANIMMESH_PATH	TEXT("../Bin/Export/Debug/x64/ShaderFiles/Shader_VtxAnimMesh.hlsl")

#pragma endregion

#pragma region ��

#pragma region �ִϸ��̼� ��

#define	ANIMMODEL_FIONA_TAG			TEXT("Prototype_Component_AnimModel_Fiona")
#define	ANIMMODEL_FIONA_PATH		"../Bin/Export/Debug/x64/Resources/Models/AnimModel/Fiona/Fiona.fbx"

#define	ANIMMODEL_RAT_TAG		TEXT("Prototype_Component_AnimModel_Rat")
#define	ANIMMODEL_RAT_PATH		"../Bin/Export/Debug/x64/Resources/Models/AnimModel/Rat/Rat.fbx"

#define	ANIMMODEL_BAT_TAG		TEXT("Prototype_Component_AnimModel_Bat")
#define	ANIMMODEL_BAT_PATH		"../Bin/Export/Debug/x64/Resources/Models/AnimModel/Bat/Bat.fbx"

#pragma endregion

#pragma region �Ϲݸ�

#define	MODEL_PINETREE_TAG		TEXT("Prototype_Component_Model_PineTree")
#define	MODEL_PINETREE_PATH		"../Bin/Export/Debug/x64/Resources/Models/NonAnimModel/PineTree/PineTree.fbx"

#define	MODEL_SM_REED1_TAG		TEXT("Prototype_Component_Model_SM_Reed1")
#define	MODEL_SM_REED1_PATH		"../Bin/Export/Debug/x64/Resources/Models/NonAnimModel/SM_Reed1/SM_Reed1.fbx"

#define	MODEL_SM_REED2_TAG		TEXT("Prototype_Component_Model_SM_Reed2")
#define	MODEL_SM_REED2_PATH		"../Bin/Export/Debug/x64/Resources/Models/NonAnimModel/SM_Reed2/SM_Reed2.fbx"

#pragma endregion

#pragma endregion

#pragma region ��Ÿ ������Ʈ

#define COM_STATEMACHINE_TAG	TEXT("Prototype_Component_StateMachine")
#define COM_AI_TAG				TEXT("Prototype_Component_AI")

#pragma endregion

#pragma region �ؽ���

#define TEX_BACKGROUND_TAG		TEXT("Prototype_Component_Texture_BackGround")
#define TEX_BACKGROUND_PATH		TEXT("../Bin/Export/Debug/x64/Resources/Textures/Default%d.jpg")

#define TEX_TERRAIN_TAG			TEXT("Prototype_Component_Texture_Terrain")
#define TEX_TERRAIN_PATH		TEXT("../Bin/Export/Debug/x64/Resources/Textures/Terrain/Tile%d.dds")

#define TEX_TERRAIN_MASK_TAG	TEXT("Prototype_Component_Texture_Terrain_Mask")
#define TEX_TERRAIN_MASK_PATH	TEXT("../Bin/Export/Debug/x64/Resources/Textures/Terrain/Mask.bmp")

#define TEX_TERRAIN_BRUSH_TAG	TEXT("Prototype_Component_Texture_Terrain_Brush")
#define TEX_TERRAIN_BRUSH_PATH	TEXT("../Bin/Export/Debug/x64/Resources/Textures/Terrain/Brush.png")

#pragma endregion

#pragma region ������ü

#define G0_BACKGROUND_TAG		TEXT("Prototype_GameObject_BackGround")

#define GO_GAMEOBJECTTEST_TAG	TEXT("Prototype_GameObject_GameObject_Test")

#define G0_TERRAIN_TAG			TEXT("Prototype_GameObject_Terrain")

#define G0_TERRAIN_DEMO_TAG		TEXT("Prototype_GameObject_Terrain_Demo")

#define G0_DCAMERA_TAG			TEXT("Prototype_GameObject_DynamicCamera")

#define G0_OBJECTMESH_DEMO_TAG	TEXT("Prototype_GameObject_ObjectMesh_Demo")

#define G0_ANIMMESH_DEMO_TAG	TEXT("Prototype_GameObject_Animmesh_Demo")

#pragma endregion