set(header_files

)

set(source_files
  ./Source/BasicTest.cc
)

set(all_files
  ${header_files}
  ${source_files}
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_executable(BevareTest ${all_files})
target_include_directories(BevareTest PUBLIC ./Include)

target_link_libraries(BevareTest PUBLIC Bevare GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(BevareTest)
