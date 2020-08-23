cmake_minimum_required(VERSION 3.0.0)
project(dtmf_gen VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(dtmf_gen src/main.c src/dtmf.c src/wave.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(dtmf_gen m)