#pragma once

#include "Application/Application.h"
#include "Application/ApplicationEntryPoint.inc"

namespace sndbox {

class USandboxApp : public bevare::UApplication {
 public:
  USandboxApp(bevare::FApplicationDesc& desc) : UApplication(desc) {}

  virtual ~USandboxApp() override {}

 public:
  virtual void Run() override { UApplication::Run(); }
};

}  // namespace sndbox

eastl::shared_ptr<bevare::UApplication> CreateApplication(
    const bevare::FCommandLineArgs& args) {
  bevare::FApplicationDesc desc = {
      .Name = "Bevare",
  };

  return eastl::make_shared<sndbox::USandboxApp>(desc);
}
