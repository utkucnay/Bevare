set(header_files

)

set(source_files
  ./Source/BevareSandbox.cc
)

set(all_files
  ${header_files}
  ${source_files}
)

add_executable(BevareSandbox ${all_files})
target_include_directories(BevareSandbox PUBLIC ./Include)

target_link_libraries(BevareSandbox PUBLIC Bevare)
