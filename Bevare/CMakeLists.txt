add_subdirectory(Vendor)

set(header_files

)

set(source_files
  ./Source/Application/Application.cc
  ./Source/Memory/BevareEASTLAlloc.cc
  ./Source/Graphics/GraphicsContext_DX12.cc
)

set(all_files
  ${header_files}
  ${source_files}
)

add_library(Bevare ${all_files})
target_precompile_headers(Bevare PUBLIC ./Include/BevarePCH.h)
target_include_directories(Bevare PUBLIC ./Include)

target_link_libraries(Bevare PUBLIC nvrhi glfw glm spdlog EASTL assimp nvrhi_d3d12 DXGI.lib nvrhi_d3d11 nvrhi_vk)

if (BEVARE_TEST)
  add_subdirectory(./Test/)
endif()
