#include "Application/Application.h"
#include "EASTL/string_view.h"
#include "GLFW/glfw3.h"
#include "Graphics/Context/GraphicsContext_DX12.h"
#include "glm/trigonometric.hpp"
#include "nvrhi/nvrhi.h"

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

  d3d12::FGraphicsContextDesc graphicsContextDesc = {
      .Window = m_Window,
  };

  m_GraphicsContext =
      eastl::make_shared<d3d12::UGraphicsContext>(graphicsContextDesc);
  m_GraphicsContext->CreateDevice(nullptr);
  m_GraphicsContext->CreateSwapChain();

  m_CommandList = m_GraphicsContext->GetDevice()->createCommandList();

  nvrhi::ShaderDesc shaderVSDesc = {.entryName = "main_vs"};
  nvrhi::ShaderDesc shaderPSDesc = {.entryName = "main_ps"};
  eastl::string_view vs =
      "static const float2 g_positions[] = { 	float2(-0.5, -0.5), 	"
      "float2(0, 0.5), 	float2(0.5, -0.5) }; static const float3 g_colors[] = "
      "{ 	float3(1, 0, 0), 	float3(0, 1, 0), 	float3(0, 0, 1)	 }; void "
      "main_vs( 	uint i_vertexId : SV_VertexID, 	out float4 o_pos : "
      "SV_Position, 	out float3 o_color : COLOR ) { 	o_pos = "
      "float4(g_positions[i_vertexId], 0, 1); 	o_color = "
      "g_colors[i_vertexId]; } void main_ps( 	in float4 i_pos : SV_Position, "
      "	in float3 i_color : COLOR, 	out float4 o_color : SV_Target0 ) { 	"
      "o_color = float4(i_color, 1); }";

  nvrhi::ShaderHandle vsShader = m_GraphicsContext->GetDevice()->createShader(
      shaderVSDesc, vs.data(), vs.size());
  //nvrhi::ShaderHandle psShader = m_GraphicsContext->GetDevice()->createShader(
      //shaderPSDesc, vs.data(), vs.size());

  nvrhi::GraphicsPipelineDesc graphicsPipelineDesc = {
      .primType = nvrhi::PrimitiveType::TriangleList,
      //.VS = vsShader,
      //.PS = psShader,
      .renderState = {.depthStencilState = {.depthTestEnable = true}},
  };
  //m_Pipeline = m_GraphicsContext->GetDevice()->createGraphicsPipeline(
     // graphicsPipelineDesc, nullptr);
}

UApplication::~UApplication() {
  glfwDestroyWindow(m_Window);

  glfwTerminate();
}

void UApplication::Run() {

  while (!glfwWindowShouldClose(m_Window)) {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();

    float redChannel = glm::sin(glfwGetTime());

    nvrhi::IDevice* device = m_GraphicsContext->GetDevice();

    nvrhi::ITexture* frameBuffer = m_GraphicsContext->GetCurrentFrameBuffer();

    m_CommandList->open();

    m_CommandList->setTextureState(frameBuffer, nvrhi::TextureSubresourceSet(),
                                   nvrhi::ResourceStates::RenderTarget);

    m_CommandList->clearTextureFloat(
        frameBuffer, nvrhi::TextureSubresourceSet(), nvrhi::Color(redChannel, 0, 0, 1));

    nvrhi::GraphicsState state = {
        .pipeline = m_Pipeline,
        //.framebuffer = frameBuffer,
    };

    //state.viewport.addViewportAndScissorRect(frameBuffer->)

    m_CommandList->setTextureState(frameBuffer, nvrhi::TextureSubresourceSet(),
                                   nvrhi::ResourceStates::Present);

    m_CommandList->close();
    device->executeCommandList(m_CommandList);

    m_GraphicsContext->Present();
  }
}

}  // namespace bevare
