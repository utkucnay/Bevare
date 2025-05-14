#pragma once

#include "EASTL/shared_ptr.h"
#include "EASTL/string_view.h"
#include "GLFW/glfw3.h"

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

  friend int ::main(int argc, char** argv);
};

eastl::shared_ptr<UApplication> CreateApplication(const FCommandLineArgs& desc);

}  // namespace bevare
