#include "DX11Framework.h"
#include "Objects/Cameras/StaticCamera.h"
#include "Objects/Cameras/DebugCamera.h"
#include "Objects/HardCoded/Cube.h"
#include "Objects/HardCoded/Pyramid.h"
#include <ios>
#include <string>
#define ReturnFail(x) if(FAILED(x)) return x;
#define ThrowOnFail(x) if(FAILED(x)) throw new std::exception;

LRESULT CALLBACK WndProc(const HWND h_wnd, const UINT message, const WPARAM w_param, const LPARAM l_param)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(h_wnd, &ps);
        EndPaint(h_wnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(h_wnd, message, w_param, l_param);
    }

    return 0;
}

HRESULT DX11Framework::Initialise(const HINSTANCE h_instance, const int n_show_cmd)
{
    #pragma region Ptr Initialization
    loading_ = new Loading();
    render_ = new RenderData();
    light_ = new LightData();
    camera_ = new StaticCamera();
    objects_ = new std::map<int, BaseHardObject*>();
    #pragma endregion Ptr Initialization

    HRESULT hr = CreateWindowHandle(h_instance, n_show_cmd);
    if (FAILED(hr)) return E_FAIL;

    hr = CreateD3DDevice();
    if (FAILED(hr)) return E_FAIL;

    hr = CreateSwapChainAndFrameBuffer();
    if (FAILED(hr)) return E_FAIL;

    hr = InitShadersAndInputLayout();
    if (FAILED(hr)) return E_FAIL;

    hr = InitPipelineState();
    if (FAILED(hr)) return E_FAIL;

    hr = InitPipelineVariables();
    if (FAILED(hr)) return E_FAIL;

    hr = InitRunTimeData();
    if (FAILED(hr)) return E_FAIL;

    return hr;
}

DX11Framework::~DX11Framework()
{
    if(dxgi_device_)dxgi_device_->Release();
    
    if(dxgi_factory_)dxgi_factory_->Release();
    
    if(frame_buffer_view_)frame_buffer_view_->Release();
    
    if(swap_chain_)swap_chain_->Release();

    if(fill_state_)fill_state_->Release();
    if(wireframe_state_)wireframe_state_->Release();
    
    if(vs_lighting_)vs_lighting_->Release();
    if (ps_lighting_)ps_lighting_->Release();
    if(vs_skybox_)vs_skybox_->Release();
    if (ps_skybox_)ps_skybox_->Release();
    if(input_layout_)input_layout_->Release();
    
    if (depth_stencil_buffer_)depth_stencil_buffer_->Release();
    
    if (depth_stencil_view_)depth_stencil_view_->Release();

#pragma region Pointer Uninitialisation
    if (camera_) camera_ = nullptr;
    if (loading_) loading_ = nullptr;
    if (render_) render_ = nullptr;
    if (light_) light_ = nullptr;
    if (pos_) pos_ = nullptr;
    if (skybox_) skybox_ = nullptr;
    if (knot_) knot_ = nullptr;
    if (objects_) objects_->clear(); objects_ = nullptr;
#pragma endregion
}

HRESULT DX11Framework::CreateWindowHandle(const HINSTANCE h_instance, const int n_show_cmd)
{
    const wchar_t* windowName  = L"DX11Graphics";

    WNDCLASSW wndClass;
    wndClass.style = 0;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = 0;
    wndClass.hIcon = 0;
    wndClass.hCursor = 0;
    wndClass.hbrBackground = 0;
    wndClass.lpszMenuName = 0;
    wndClass.lpszClassName = windowName;

    RegisterClassW(&wndClass);

    window_handle_ = CreateWindowExW(0, windowName, windowName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 
        static_cast<int>(camera_->GetWindowWidth()), static_cast<int>(camera_->GetWindowHeight()), nullptr, nullptr, h_instance, nullptr);

    return S_OK;
}

HRESULT DX11Framework::CreateD3DDevice()
{
    HRESULT hr = S_OK;

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
    };

    ID3D11Device* baseDevice;
    ID3D11DeviceContext* baseDeviceContext;

    DWORD createDeviceFlags = 0;
#ifdef _DEBUG
    // If we're building for Debug, contain device warnings and errors
    // These are output to the visual studio console
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT | createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &baseDevice, nullptr, &baseDeviceContext);
    if (FAILED(hr)) return hr;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    hr = baseDevice->QueryInterface(__uuidof(ID3D11Device), reinterpret_cast<void**>(&render_->device_));
    hr = baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext), reinterpret_cast<void**>(&render_->device_context_));

    baseDevice->Release();
    baseDeviceContext->Release();

    ///////////////////////////////////////////////////////////////////////////////////////////////

    hr = render_->device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgi_device_));
    if (FAILED(hr)) return hr;

    IDXGIAdapter* dxgiAdapter;
    hr = dxgi_device_->GetAdapter(&dxgiAdapter);
    hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgi_factory_));
    dxgiAdapter->Release();

    return hr;
}

HRESULT DX11Framework::CreateSwapChainAndFrameBuffer()
{
#pragma region Swap Chain Creation & Binding
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
    swapChainDesc.Width = 0; // Defer to WindowWidth
    swapChainDesc.Height = 0; // Defer to WindowHeight
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //FLIP* modes don't support sRGB backbuffer
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = 0;

    HRESULT hr = dxgi_factory_->CreateSwapChainForHwnd(render_->device_, window_handle_, &swapChainDesc, nullptr, nullptr, &swap_chain_);
    if (FAILED(hr)) return hr;
#pragma endregion

#pragma region Frame Buffer Initialisation & Binding
    ID3D11Texture2D* frameBuffer = nullptr;

    hr = swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&frameBuffer));
    if (FAILED(hr)) return hr;

    D3D11_RENDER_TARGET_VIEW_DESC framebufferDesc = {};
    framebufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //sRGB render target enables hardware gamma correction
    framebufferDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    hr = render_->device_->CreateRenderTargetView(frameBuffer, &framebufferDesc, &frame_buffer_view_);
#pragma endregion 

#pragma region Depth Buffer Creation & Binding
    D3D11_TEXTURE2D_DESC depthBufferDesc = {};
    frameBuffer->GetDesc(&depthBufferDesc);

    // Formats the depth to be a 24bit unsigned float, between 0 - 1 & stenicl to be 8bit unsigned int
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    render_->device_->CreateTexture2D(&depthBufferDesc, nullptr, &depth_stencil_buffer_);
    render_->device_->CreateDepthStencilView(depth_stencil_buffer_, nullptr, &depth_stencil_view_);
#pragma endregion 
    
    frameBuffer->Release();
    return hr;
}

HRESULT DX11Framework::InitShadersAndInputLayout()
{
    HRESULT hr = S_OK;
    ID3DBlob* errorBlob;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag and D3DCOMPILE_SKIP_OPTIMIZATION
    // This means the shader runs slower, but debugging is easier
    // As optimizations will move/remove lines of code in the shader
    // https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/d3dcompile-constants
    dwShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

#pragma region Lighting Shader
    #pragma region VS Compiling, Creation & Binding
    ID3DBlob* vsBlob;

    hr =  D3DCompileFromFile(L"Shaders/LightingShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS_main", "vs_5_0", dwShaderFlags, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(window_handle_, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }

    hr = render_->device_->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vs_lighting_);
    if (FAILED(hr))
    {
        MessageBoxA(window_handle_, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 }
    };

    hr = render_->device_->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &input_layout_);
    if (FAILED(hr)) return hr;
    #pragma endregion 

    #pragma region PS Compiling, Creation & Binding
    ID3DBlob* psBlob;

    hr = D3DCompileFromFile(L"Shaders/LightingShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_main", "ps_5_0", dwShaderFlags, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(window_handle_, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }

    hr = render_->device_->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &ps_lighting_);
    if (FAILED(hr))
    {
        MessageBoxA(window_handle_, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }
    #pragma endregion 
#pragma endregion 

#pragma region Skybox Shader
    #pragma region VS Compiling, Creation & Binding
    hr =  D3DCompileFromFile(L"Shaders/SkyboxShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS_main", "vs_5_0", dwShaderFlags, 0, &vsBlob, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(window_handle_, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }

    hr = render_->device_->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vs_skybox_);
    if (FAILED(hr))
    {
        MessageBoxA(window_handle_, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }
    #pragma endregion 

    #pragma region PS Compiling, Creation & Binding
    hr = D3DCompileFromFile(L"Shaders/SkyboxShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_main", "ps_5_0", dwShaderFlags, 0, &psBlob, &errorBlob);
    if (FAILED(hr))
    {
        MessageBoxA(window_handle_, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }

    hr = render_->device_->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &ps_skybox_);
    if (FAILED(hr))
    {
        MessageBoxA(window_handle_, (char*)errorBlob->GetBufferPointer(), nullptr, ERROR);
        errorBlob->Release();
        return hr;
    }
    #pragma endregion 

#pragma endregion 

    vsBlob->Release();
    psBlob->Release();

    return hr;
}

HRESULT DX11Framework::InitPipelineState()
{
#pragma region Object Bilinear Sampler Creation & Binding
    D3D11_SAMPLER_DESC bi_obj_sampler_desc = {};
    bi_obj_sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    bi_obj_sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    bi_obj_sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    bi_obj_sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    bi_obj_sampler_desc.MinLOD = 0;
    bi_obj_sampler_desc.MaxLOD = 1;
    
    HRESULT hr = render_->device_->CreateSamplerState(&bi_obj_sampler_desc, &bi_obj_sampler_);
    if (FAILED(hr)) return hr;

    render_->device_context_->PSSetSamplers(0, 1, &bi_obj_sampler_);
#pragma endregion 

#pragma region Skybox Bilinear Sampler Creation & Binding
    D3D11_SAMPLER_DESC bi_skybox_sampler_desc = {};
    bi_skybox_sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    bi_skybox_sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    bi_skybox_sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    bi_skybox_sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    
    hr = render_->device_->CreateSamplerState(&bi_skybox_sampler_desc, &bi_skybox_sampler);
    if (FAILED(hr)) return hr;

    render_->device_context_->PSSetSamplers(0, 1, &bi_skybox_sampler);
#pragma endregion 

#pragma region Skybox Depth Stencil Creation & Binding
    D3D11_DEPTH_STENCIL_DESC ds_skybox_desc = { };
    ds_skybox_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    ds_skybox_desc.DepthEnable = TRUE;
    ds_skybox_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

    hr = render_->device_->CreateDepthStencilState(&ds_skybox_desc, &depth_stencil_skybox_);
    if (FAILED(hr)) return hr;
    render_->device_context_->OMSetDepthStencilState(depth_stencil_skybox_, 0);
#pragma endregion
    
    return hr;
}

HRESULT DX11Framework::InitPipelineVariables()
{
#pragma region Input Layout Setting
    render_->device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    render_->device_context_->IASetInputLayout(input_layout_);
#pragma endregion 

#pragma region Filled Rasterizer State Creation & Binding
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;

    HRESULT hr = render_->device_->CreateRasterizerState(&rasterizerDesc, &fill_state_);
    if (FAILED(hr)) return hr;

    render_->device_context_->RSSetState(fill_state_);
#pragma endregion 

#pragma region Wireframe Rasterizer State Creation & Binding
    // -- For Development / Debugging Purposes -- //
    rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;

    hr = render_->device_->CreateRasterizerState(&rasterizerDesc, &wireframe_state_);
    if (FAILED(hr)) return hr;
#pragma endregion 

#pragma region Viewport Creation & Setting
    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, camera_->GetWindowWidth(), camera_->GetWindowHeight(), 0.0f, 1.0f};
    render_->device_context_->RSSetViewports(1, &viewport);
#pragma endregion 

#pragma region Constant Buffer Creation & Binding
    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.ByteWidth = sizeof(ConstantBufferData);
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = render_->device_->CreateBuffer(&constantBufferDesc, nullptr, &render_->constant_buffer_);
    if (FAILED(hr)) { return hr; }

    render_->device_context_->VSSetConstantBuffers(0, 1, &render_->constant_buffer_);
    render_->device_context_->PSSetConstantBuffers(0, 1, &render_->constant_buffer_);
#pragma endregion
    
    return hr;
}

HRESULT DX11Framework::InitRunTimeData()
{
    HRESULT hr = S_OK;

    knot_ = new GameObject(render_, "Objects\\Models\\torusKnot.obj", "Textures\\asphalt_COLOR.dds", PLANET);
    //--------------------------------------------------//
    skybox_ = new Cube(render_, BACKDROP);
    //--------------------------------------------------//
    for (int i = 0; i < OBJ_COUNT; ++i)
    {
        const auto type = static_cast<BodyType>(i);

        type == STATION ? objects_->insert_or_assign(i, new Pyramid(render_, type)) : objects_->insert_or_assign(i, new Cube(render_, type));
    }
    //--------------------------------------------------//
    camera_->SetView();
    
    return hr;
}

void DX11Framework::InitDrawData() const
{
    //Present unbinds render target, so rebind and clear at start of each frame
    render_->device_context_->OMSetRenderTargets(1, &frame_buffer_view_, depth_stencil_view_);
    render_->device_context_->ClearRenderTargetView(frame_buffer_view_, backgroundColor_);
    render_->device_context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    render_->device_context_->VSSetShader(vs_lighting_, nullptr, 0);
    render_->device_context_->PSSetShader(ps_lighting_, nullptr, 0);
}

float DX11Framework::SimpleCount() const
{  
    static ULONGLONG frame_start = GetTickCount64();

    ULONGLONG frame_now = GetTickCount64();
    
    float delta_time = static_cast<float>(frame_now - frame_start) / 900.0f;
    
    frame_start = frame_now;

    static float simple_count = 0.0f;
    
    simple_count += delta_time;
    
    cb_data_.SetSimpleCount(simple_count);
    
    return simple_count;
}

void DX11Framework::Update()
{
   const float simple_count = SimpleCount();

    knot_->Update(simple_count);
    //--------------------------------------------------//
    for (int i = 0; i < OBJ_COUNT; ++i)
    {
        objects_->at(i)->Update(simple_count);
    }
    //--------------------------------------------------//
    camera_->Update(simple_count);
    //--------------------------------------------------//
    skybox_->Update(simple_count);

    #pragma region User Input Handling
    if (GetAsyncKeyState('Q') & 0x8000)
    {
        render_->device_context_->RSSetState(wireframe_state_);
    }
    if (GetAsyncKeyState('E') & 0x8000)
    {
        render_->device_context_->RSSetState(fill_state_);
    }
    if (GetAsyncKeyState('1') & 0x8000)
    {
        camera_ = new StaticCamera();
    }
    if (GetAsyncKeyState('2') & 0x8000)
    {
        camera_ = new DebugCamera();
    }
    #pragma endregion
}

void DX11Framework::Draw() const 
{
    InitDrawData();
    //--------------------------------------------------//
    for (int i = 0; i < OBJ_COUNT; ++i)
    {
        objects_->at(i)->Render();
    }
    //--------------------------------------------------//
    knot_->Render();
    //--------------------------------------------------//
    render_->device_context_->VSSetShader(vs_skybox_, nullptr, 0);
    render_->device_context_->PSSetShader(ps_skybox_, nullptr, 0);
    skybox_->Render();
    //--------------------------------------------------//
    swap_chain_->Present(0, 0);
}
