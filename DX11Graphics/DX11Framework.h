#pragma once
#ifndef DX11_DX11FRAMEWORK_H
#define DX11_DX11FRAMEWORK_H

#include <windows.h>
#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Objects/GameObject.h"
#include "Objects/Cameras/BaseCamera.h"
#include "Objects/HardCoded/BaseHardObject.h"

#include "Structures/LightingData.h"
#include "Structures/PositionData.h"
#include "Structures/RenderData.h"
#include "Structures/ConstantBufferData.h"

#include "Loading.h"

#define OBJ_COUNT 4

class DX11Framework
{
	ConstantBufferData& cb_data_ = ConstantBufferData::GetInstance();
	//--------------------------------------------------//
	Loading* loading_ = nullptr;
	RenderData* render_ = nullptr;
	LightData* light_ = nullptr;
	PositionData* pos_ = nullptr;
	//--------------------------------------------------//
	float backgroundColor_[4] = { 0.0025f, 0.0025f, 0.0025f, 1.0f };
	//--------------------------------------------------//
	std::map<int, BaseHardObject*>* objects_ = nullptr;
	BaseHardObject* skybox_ = nullptr;
	GameObject* knot_ = nullptr;
	BaseCamera* camera_ = nullptr;
	//--------------------------------------------------//
	IDXGIDevice* dxgi_device_ = nullptr;
	//--------------------------------------------------//
	IDXGIFactory2* dxgi_factory_ = nullptr;
	//--------------------------------------------------//
	ID3D11RenderTargetView* frame_buffer_view_ = nullptr;
	//--------------------------------------------------//
	IDXGISwapChain1* swap_chain_ = nullptr;
	//--------------------------------------------------//
	ID3D11SamplerState* bi_obj_sampler_ = nullptr;
	ID3D11SamplerState* bi_skybox_sampler = nullptr;
	//--------------------------------------------------//
	ID3D11RasterizerState* fill_state_ = nullptr;
	ID3D11RasterizerState* wireframe_state_ = nullptr;
	//--------------------------------------------------//
	ID3D11VertexShader* vs_lighting_ = nullptr;
	ID3D11VertexShader* vs_skybox_ = nullptr;
	ID3D11PixelShader* ps_lighting_ =nullptr;
	ID3D11PixelShader* ps_skybox_ =nullptr;
	//--------------------------------------------------//
	ID3D11InputLayout* input_layout_ = nullptr;
	//--------------------------------------------------//
	ID3D11Texture2D* depth_stencil_buffer_ = nullptr;
	//--------------------------------------------------//
	ID3D11DepthStencilView* depth_stencil_view_ = nullptr;
	//--------------------------------------------------//
	ID3D11DepthStencilState* depth_stencil_skybox_ = nullptr;
	//--------------------------------------------------//
	HWND window_handle_ = nullptr;

public:
	HRESULT Initialise(HINSTANCE h_instance, int n_show_cmd);
	//--------------------------------------------------//
	HRESULT CreateWindowHandle(HINSTANCE h_instance, int n_show_cmd);
	HRESULT CreateD3DDevice();
	HRESULT CreateSwapChainAndFrameBuffer();
	//--------------------------------------------------//
	HRESULT InitShadersAndInputLayout();
	HRESULT InitPipelineState();
	HRESULT InitPipelineVariables();
	HRESULT InitRunTimeData();
	//--------------------------------------------------//
	void InitDrawData() const;
	//--------------------------------------------------//
	float SimpleCount() const;
	void Draw() const;
	void Update();
	//--------------------------------------------------//
	~DX11Framework();
};

#endif
