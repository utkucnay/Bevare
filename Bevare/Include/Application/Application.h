#pragma once

#include "Graphics/Context/GraphicsContext.h"
#include "nvrhi/nvrhi.h"

int main(int argc, char** argv);

namespace bevare {

struct FApplicationDesc {
  eastl::string_view Name;
};

struct FCommandLineArgs {
  char** Argv = nullptr;
  int Argc = 0;
};

class UApplication {
 public:
  UApplication(FApplicationDesc& desc);
  virtual ~UApplication();

 public:
  virtual void Run();

 private:
  GLFWwindow* m_Window;

  eastl::shared_ptr<IGraphicsContext> m_GraphicsContext;
  nvrhi::CommandListHandle m_CommandList;

  nvrhi::GraphicsPipelineHandle m_Pipeline;

  friend int ::main(int argc, char** argv);
};

eastl::shared_ptr<UApplication> CreateApplication(const FCommandLineArgs& desc);

}  // namespace bevare
