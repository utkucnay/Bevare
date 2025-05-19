#include "Graphics/Context/GraphicsContext_DX12.h"
#include <directx/d3d12.h>
#include "BevareCore.h"
#include "Graphics/Context/GraphicsContext.h"
#include "nvrhi/d3d12.h"
#include "nvrhi/nvrhi.h"

namespace bevare::d3d12 {

UGraphicsContext::UGraphicsContext(
    const FGraphicsContextDesc& graphicsContextDesc) {
  m_Window = graphicsContextDesc.Window;
  m_hWnd = glfwGetWin32Window(graphicsContextDesc.Window);

  HRESULT hr = CreateDXGIFactory2(0, IID_PPV_ARGS(&m_DxgiFactory2));

  if (FAILED(hr)) {
    BEVARE_CORE_ERROR("Failed to create a dxgi factory");
  } else {
    BEVARE_CORE_LOG("Created a dxgi factory");
  }
}

UGraphicsContext::~UGraphicsContext() {}

void UGraphicsContext::EnumarateAdapters(
    eastl::vector<FAdapterInfo>& outAdapters) {
  UINT i = 0;
  IDXGIAdapter* pNativeAdapter;

  while (m_DxgiFactory2->EnumAdapters(i, &pNativeAdapter) ==
         DXGI_ERROR_NOT_FOUND) {
    DXGI_ADAPTER_DESC adapterDesc;
    pNativeAdapter->GetDesc(&adapterDesc);

    /*FAdapterInfo adapter = {
        .Name = GetAdapterName(adapterDesc),
        .VendorID = adapterDesc.VendorId,
        .DeviceID = adapterDesc.DeviceId,
        .DedicatedVideoMemory = adapterDesc.DedicatedVideoMemory,
        .DxgiAdapter = pNativeAdapter,
    };

    outAdapters.push_back(adapter);*/
  }
}

void UGraphicsContext::CreateDevice(FAdapterInfo* adapterInfo) {
  IDXGIAdapter* pAdapter = nullptr;

  if (adapterInfo != nullptr) {
    //pAdapter = adapterInfo->DxgiAdapter;
  }

  HRESULT hr = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_1,
                                 IID_PPV_ARGS(&m_NativeDevice));

  if (FAILED(hr)) {
    BEVARE_CORE_ERROR("Didn't create dx12 device");
  } else {
    BEVARE_CORE_LOG("Created dx12 device")
  }

  //m_Adapter = adapterInfo;

  D3D12_COMMAND_QUEUE_DESC graphicsCommandQueueDesc = {
      .Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
      .Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
      .Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
      .NodeMask = 0,
  };

  hr = m_NativeDevice->CreateCommandQueue(&graphicsCommandQueueDesc,
                                          IID_PPV_ARGS(&m_GraphicsQueue));

  if (FAILED(hr)) {
    BEVARE_CORE_ERROR("Didn't create dx12 graphics command queue");
  } else {
    BEVARE_CORE_LOG("Created dx12 graphics command queue");
  }

  nvrhi::d3d12::DeviceDesc deviceDesc = {
      .pDevice = m_NativeDevice,
      .pGraphicsCommandQueue = m_GraphicsQueue,
  };
  m_Device = nvrhi::d3d12::createDevice(deviceDesc);
}

void UGraphicsContext::CreateSwapChain() {
  RECT clientRect;
  GetClientRect(m_hWnd, &clientRect);
  UINT width = clientRect.right - clientRect.left;
  UINT height = clientRect.bottom - clientRect.top;

  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {
      .Width = 1280,
      .Height = 720,
      .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
      .SampleDesc =
          {
              .Count = 1,
              .Quality = 0,
          },
      .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
      .BufferCount = 3,
      .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
      .Flags = 0,
  };

  DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullscreenDesc = {
      .Windowed = true,
  };

  nvrhi::RefCountPtr<IDXGISwapChain1> pSwapChain1;
  HRESULT hr = m_DxgiFactory2->CreateSwapChainForHwnd(
      m_GraphicsQueue, m_hWnd, &swapChainDesc, &swapChainFullscreenDesc,
      nullptr, &pSwapChain1);

  if (FAILED(hr)) {
    BEVARE_CORE_ERROR("Didn't create dx12 swap chain");
  }

  hr = pSwapChain1->QueryInterface(IID_PPV_ARGS(&m_SwapChain));

  if (FAILED(hr)) {
    BEVARE_CORE_ERROR("Didn't get interface dx12 swap chain");
  } else {
    BEVARE_CORE_LOG("Created dx12 swap chain");
  }

  CreateRenderTargets();

  CreateFence();
}

void UGraphicsContext::CreateFence() {
  m_NativeDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
                              IID_PPV_ARGS(&m_FrameFence));

  for (UINT bufferIndex = 0; bufferIndex < 3; bufferIndex++) {
    m_FrameFenceEvents.push_back(CreateEvent(nullptr, false, true, nullptr));
  }
}

void UGraphicsContext::CreateRenderTargets() {
  m_SwapChainBuffers.resize(3);
  m_RhiSwapChainBuiffers.resize(3);

  for (UINT i = 0; i < 3; ++i) {
    HRESULT hr =
        m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffers[i]));

    if (FAILED(hr)) {
      BEVARE_CORE_ERROR("Didn't get a buffer on swap chain");
    }

    RECT clientRect;
    GetClientRect(m_hWnd, &clientRect);
    UINT width = clientRect.right - clientRect.left;
    UINT height = clientRect.bottom - clientRect.top;

    nvrhi::TextureDesc textureDesc{
        .width = 1280,
        .height = 720,
        .sampleCount = 1,
        .sampleQuality = 0,
        .format = nvrhi::Format::RGBA8_UNORM,
        .debugName = "SwapChain Buffer",
        .isRenderTarget = true,
        .isUAV = false,
        .initialState = nvrhi::ResourceStates::Present,
        .keepInitialState = true,
    };

    m_RhiSwapChainBuiffers[i] = m_Device->createHandleForNativeTexture(
        nvrhi::ObjectTypes::D3D12_Resource,
        nvrhi::Object(m_SwapChainBuffers[i]), textureDesc);
  }
}

void UGraphicsContext::Present() {
  auto bufferIndex = m_SwapChain->GetCurrentBackBufferIndex();

  UINT presentFlags = 0;

  HRESULT hr = m_SwapChain->Present(0, presentFlags);

  m_FrameFence->SetEventOnCompletion(m_FrameCount,
                                     m_FrameFenceEvents[bufferIndex]);

  m_GraphicsQueue->Signal(m_FrameFence, m_FrameCount);
  m_FrameCount++;

  if (FAILED(hr)) {
    BEVARE_CORE_ERROR("Didn't work dx12 present")
  }
}

}  // namespace bevare::d3d12
