#pragma once

#include "nvrhi/nvrhi.h"

#if defined(BEVARE_WITH_DX11)
#include "nvrhi/d3d11.h"
#endif
#if defined(BEVARE_WITH_DX12)
#include "nvrhi/d3d12.h"
#endif
#if defined(BEVARE_WITH_VULKAN)
//#include "nvrhi/vulkan.h"
#endif

namespace bevare {

struct FAdapterInfo {
  typedef eastl::array<uint8_t, 16> UUID;
  typedef eastl::array<uint8_t, 8> LUID;

  eastl::string Name;
  uint32_t VendorID = 0;
  uint32_t DeviceID = 0;
  uint64_t DedicatedVideoMemory = 0;

  eastl::optional<UUID> Uuid;
  eastl::optional<LUID> Luid;

#if defined(BEVARE_WITH_DX11) || defined(BEVARE_WITH_DX12)
  nvrhi::RefCountPtr<IDXGIAdapter> DxgiAdapter = nullptr;
#endif
#if defined(BEVARE_WITH_VULKAN)
  //VkPhysicalDevice VkPhysicalDevice = nullptr;
#endif
};

class IGraphicsContext {
 public:
  virtual ~IGraphicsContext() = default;

 public:
  virtual void EnumarateAdapters(eastl::vector<FAdapterInfo>& outAdapters) = 0;
  virtual void CreateDevice(FAdapterInfo* adapterInfo) = 0;
  virtual void CreateSwapChain() = 0;
  virtual void Present() = 0;

  virtual nvrhi::ITexture* GetCurrentFrameBuffer() = 0;
  virtual uint32_t GetCurrentFrameBufferIndex() = 0;

  virtual nvrhi::GraphicsAPI GetGraphicsAPI() = 0;
  virtual nvrhi::IDevice* GetDevice() = 0;

 public:
  static IGraphicsContext* CreateInstance(nvrhi::GraphicsAPI graphicsApi);
};

}  // namespace Bevare
