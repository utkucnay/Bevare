#include "Application/Application.h"
#include "GLFW/glfw3.h"

namespace bevare {

UApplication::UApplication(FApplicationDesc& desc) {
  int errCode = glfwInit();

  if (errCode == 0) {
    return;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  m_Window = glfwCreateWindow(1280, 720, desc.Name.data(), nullptr, nullptr);

  if (m_Window == nullptr) {
    return;
  }

  glfwMakeContextCurrent(m_Window);
}

UApplication::~UApplication() {
  glfwDestroyWindow(m_Window);

  glfwTerminate();
}

void UApplication::Run() {

  while (!glfwWindowShouldClose(m_Window)) {

    glfwSwapBuffers(m_Window);
    glfwPollEvents();
  }
}

}  // namespace bevare
