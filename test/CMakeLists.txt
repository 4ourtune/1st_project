cmake_minimum_required(VERSION 3.16)  # VS 2022 권장 최소 버전
project(GTestStandalone)

# C++ 표준 설정 (VS 2022는 C++20까지 지원)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 런타임 충돌 방지 (MSVC에서는 꼭 필요)
if (MSVC)
    foreach (flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE)
        string (REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        string (REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endforeach()
endif()

# GoogleTest를 FetchContent로 자동 다운로드
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0  # 안정적인 버전 사용
)

# Windows에서 gtest의 런타임 라이브러리 설정
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

# 테스트 소스 자동 탐색 (main.cpp 제외)
file(GLOB TEST_SOURCES "*.cpp")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# 테스트 실행 파일 생성
add_executable(test_runner ${TEST_SOURCES})

target_link_libraries(test_runner
    gtest
    gtest_main
    gmock
    gmock_main
)

# 테스트 등록
include(GoogleTest)
gtest_discover_tests(test_runner)