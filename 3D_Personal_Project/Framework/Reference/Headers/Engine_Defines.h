#pragma once

#pragma warning(disable : 4251)

namespace Engine
{
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum TEXTURETYPE {
		TYPE_NONE, TYPE_DIFFUSE, TYPE_SPECULAR, TYPE_AMBIENT, TYPE_EMISSIVE, TYPE_HEIGHT, TYPE_NORMALS,
		TYPE_SHININESS, TYPE_OPACITY, TYPE_DISPLACEMENT, TYPE_LIGHTMAP, TYPE_REFLECTION, TYPE_BASE_COLOR, TYPE_NORMAL_CAMERA,
		TYPE_EMISSION_COLOR, TYPE_METALNESS, TYPE_DIFFUSE_ROUNGHNESS, TYPE_AMBIENT_OCCLUSION, TYPE_UNKNOWN
	};

	enum RTV_TYPE { RT_DIFFUSE, RT_LIGHTDEPTH, RT_NORMAL, RT_SHADE,RT_DEPTH,RT_SPECULAR,
		RT_EFFECT,RT_BLUR_X,RT_BLUR_Y,RT_RESULT,RT_END };


	enum CHANNELID {
		SOUND_BGM, SOUND_ENVIRONMENT,
		SOUND_PLAYER_VOICE,SOUND_PLAYER_MOVE,SOUND_PLAYER_ATTACK, SOUND_PLAYER_HIT,
		SOUND_MONSTER_VOICE,SOUND_MONSTER_MOVE,SOUND_MOSTER_ATTACK, SOUND_MONSTER_HIT,
		SOUND_NPC_VOICE,
		SOUND_PUZZLE_EFFECT,
		SOUND_BOSS_VOICE, SOUND_BOSS_MOVE, SOUND_BOSS_ATTACK, SOUND_BOSS_HIT,
		MAXCHANNEL
	};

	enum CHANNEL_GROUP_ID { BGM_GROUP, SND1_GROUP, SND2_GROUP, MAX_CHANNEL_GROUP };
}

#include "../Fmod/core/fmod.h"
#include "../Fmod/core/fmod.hpp"
#include "../Fmod/core/fmod_errors.h"
#include "../Fmod/studio/fmod_studio.h"
#include "../Fmod/studio/fmod_studio.hpp"
#include "../Fmod/core/fmod_common.h"

#include <DirectXTK/PrimitiveBatch.h>
#include <DirectXTK/Effects.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/SpriteBatch.h>
#include <DirectXTK/SpriteFont.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <random>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Effects11\d3dx11effect.h"
#include "DirectXTK\DDSTextureLoader.h"
#include "DirectXTK\WICTextureLoader.h"

#include "DirectXTK/SimpleMath.h"

#include <d3dcompiler.h>
using namespace DirectX;

#include <unordered_map>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <typeinfo>
#include <functional>
#include <iostream>
#include <tuple>
using namespace std;

namespace Engine
{
	static const wchar_t* g_pTransformTag = TEXT("Com_Transform");
}

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"
using namespace Engine;



#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/pointer.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/reader.h"
#include "rapidjson/filereadstream.h"
#include <locale>
#include <codecvt>

#include "commdlg.h"
#include "shlwapi.h"
#include <fstream>
using namespace rapidjson;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#else
#define DBG_NEW new
//#endif

#endif // _DEBUG




