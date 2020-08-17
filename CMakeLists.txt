cmake_minimum_required(VERSION 3.0.0)
project(dtmf_gen VERSION 0.1.0)

add_executable(testdtmf testdtmf.c src/dtmf.c src/wave.c)

include(CTest)
enable_testing()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_FLAGS)
include(CPack)

target_compile_definitions(testdtmf PUBLIC NDEBUG _USE_MATH_DEFINES)
target_link_libraries(testdtmf m)
