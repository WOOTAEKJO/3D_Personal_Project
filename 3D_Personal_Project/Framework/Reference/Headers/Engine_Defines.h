#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Effects11\d3dx11effect.h"
#include <d3dcompiler.h>
using namespace DirectX;

#include <unordered_map>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <typeinfo>
#include <functional>
using namespace std;

#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"
#include "Engine_Typedef.h"
using namespace Engine;



#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG




