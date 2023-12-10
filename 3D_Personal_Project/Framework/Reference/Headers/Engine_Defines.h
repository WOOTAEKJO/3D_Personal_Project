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
}


#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

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




