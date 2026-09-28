cmake_minimum_required(VERSION 3.14)

project(JTest
        VERSION 0.1.0
        DESCRIPTION "A minimal, header-light Jasmine-style C++ test framework (no external dependencies)."
        LANGUAGES CXX)

# ---------------------------------------------------------------------------
# Static library
#   The framework has NO external dependency: it only includes C++ standard
#   library headers plus its own <JTest/...> headers (verified by a full
#   #include scan of include/, src/, examples/). No find_package/FetchContent is
#   needed.
#
#   JTEST_SOURCES is globbed with CONFIGURE_DEPENDS so newly added translation
#   units are picked up automatically; the build dir does the heavy lifting.
#   JTest is PUBLIC on its include dir so that anything linking it inherits the
#   include path transitively -- the demo executable and tests rely on this.
# ---------------------------------------------------------------------------
file(GLOB JTEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/JTest/*.cpp")

add_library(JTest STATIC ${JTEST_SOURCES})

target_include_directories(JTest PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Require C++17 (the sources use std::optional etc.) for JTest's sources and
# propagate the requirement to any target that links JTest.
target_compile_features(JTest PUBLIC cxx_std_17)

# ---------------------------------------------------------------------------
# Examples
#   Build the demonstration as an executable, but do NOT register it with CTest:
#   examples/example.cpp deliberately contains failing/skipping cases to show
#   JTest's reporting output and always returns 0, so a CTest gate on it would
#   be noise. Keep it a normal target so it still builds and runs.
# ---------------------------------------------------------------------------
add_executable(example examples/example.cpp)
target_link_libraries(example PRIVATE JTest)

# ---------------------------------------------------------------------------
# Testing
#   enable_testing() + add_test() wire a genuine self-check that actually
#   passes. tests/smoke.cpp exercises the public API (describe/it/expect/
#   execute) and returns non-zero on any failed assertion, so `ctest` is a real
#   pass/fail gate: a broken or misconfigured build goes red, not green.
# ---------------------------------------------------------------------------
enable_testing()

add_executable(test_smoke tests/smoke.cpp)
target_link_libraries(test_smoke PRIVATE JTest)

add_test(NAME smoke COMMAND test_smoke)
