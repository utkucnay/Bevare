#pragma once

#include "Graphics/Context/GraphicsContext.h"

#include "Windows.h"
#include "dxgi1_5.h"

#include "nvrhi/nvrhi.h"

namespace bevare::d3d12 {

struct FGraphicsContextDesc {
  GLFWwindow* Window;
};

class UGraphicsContext : public IGraphicsContext {
 public:
  UGraphicsContext(const FGraphicsContextDesc& graphicsContextDesc);
  virtual ~UGraphicsContext();

 public:
  virtual void EnumarateAdapters(
      eastl::vector<FAdapterInfo>& outAdapters) override;
  virtual void CreateDevice(FAdapterInfo* adapterInfo) override;
  virtual void CreateSwapChain() override;
  virtual void Present() override;

  virtual nvrhi::GraphicsAPI GetGraphicsAPI() override {
    return nvrhi::GraphicsAPI::D3D12;
  }

  virtual nvrhi::IDevice* GetDevice() override { return m_Device; }

  virtual uint32_t GetCurrentFrameBufferIndex() override {
    return m_SwapChain->GetCurrentBackBufferIndex();
  }

  virtual nvrhi::ITexture* GetCurrentFrameBuffer() override {
    return m_RhiSwapChainBuiffers[GetCurrentFrameBufferIndex()];
  }

 private:
  void CreateFence();
  void CreateRenderTargets();

 private:
  eastl::string GetAdapterName(DXGI_ADAPTER_DESC const& aDesc) {
    size_t length = wcsnlen(aDesc.Description, _countof(aDesc.Description));

    eastl::string name;
    name.resize(length);
    WideCharToMultiByte(CP_ACP, 0, aDesc.Description, int(length), name.data(),
                        int(name.size()), nullptr, nullptr);

    return name;
  }

 private:
  GLFWwindow* m_Window;
  HWND m_hWnd;

  nvrhi::RefCountPtr<IDXGIFactory2> m_DxgiFactory2;
  nvrhi::RefCountPtr<ID3D12Device> m_NativeDevice;
  nvrhi::RefCountPtr<ID3D12CommandQueue> m_GraphicsQueue;
  nvrhi::RefCountPtr<IDXGISwapChain3> m_SwapChain;
  //nvrhi::RefCountPtr<IDXGIAdapter> m_Adapter;

  eastl::vector<nvrhi::RefCountPtr<ID3D12Resource>> m_SwapChainBuffers;
  std::vector<nvrhi::TextureHandle> m_RhiSwapChainBuiffers;

  nvrhi::RefCountPtr<ID3D12Fence> m_FrameFence;
  eastl::vector<HANDLE> m_FrameFenceEvents;

  nvrhi::DeviceHandle m_Device;

  uint64_t m_FrameCount;
};

}  // namespace bevare::d3d12
