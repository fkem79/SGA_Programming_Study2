#pragma once

#include <Windows.h>
#include <assert.h>

//STL
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <functional>
using namespace std;

// DirectWrite
#include <d2d1_1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//Direct3D
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//ImGui
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

//Json Cpp
#include <json/json.h>
#pragma comment(lib, "jsoncpp.lib")
#pragma warning( disable : 4996 )

//DirectXTex
// 11�� �̹��� � �ȵ� �߰� ���̺귯�� ms�� �������ذ� Texture
#include <DirectXTex.h>
#pragma comment(lib, "directxtex.lib")

// FBX SDK
#define FBXSDK_SHARED // �̰� ���� �� �������
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk.lib")
using namespace fbxsdk;

const wstring Shaders = L"../_Shaders/";
const wstring Contents = L"../_Contents/";
const wstring Textures = L"../_Textures/";
const wstring Assets = L"../_Assets/";
const wstring Models = L"../_Models/";
const wstring Materials = L"../_Materials/";

#define SAFE_RELEASE(p){ if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p){ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p); (p) = NULL; } }

#pragma comment(lib, "FrameWork/Fbx.lib")
#pragma comment(lib, "FrameWork/Interfaces.lib")
#pragma comment(lib, "FrameWork/Model.lib")
#pragma comment(lib, "FrameWork/PostEffects.lib")
#pragma comment(lib, "FrameWork/Renders.lib")
#pragma comment(lib, "FrameWork/Systems.lib")
#pragma comment(lib, "FrameWork/Utilities.lib")
#pragma comment(lib, "FrameWork/Viewer.lib")
#pragma comment(lib, "FrameWork/Objects.lib")
#pragma comment(lib, "FrameWork/LandScape.lib")

#include "Systems\Systems.h"

#include "Renders\Renders.h"

#include "Executes\Execute.h"

#include "Utilities\Utilities.h"

#include "Model/Model.h"
#include "Model/ModelBone.h"
#include "Model/ModelMesh.h"

typedef VertexTextureNormalTangent ModelVertexType; 