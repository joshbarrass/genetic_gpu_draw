# Setup project
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Genetic_GPU_Draw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set build type variable in preprocessor
if(CMAKE_BUILD_TYPE)
    string(TOUPPER "${CMAKE_BUILD_TYPE}" _upper_build_type)
    add_compile_definitions(BUILD_${_upper_build_type}=1)
endif()

# profiling
OPTION (ENABLE_PROFILING "Enable profiling" OFF)
IF (ENABLE_PROFILING)
  MESSAGE("-- Profiling enabled")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
ENDIF()

# testing new error function
OPTION (USE_OLD_ERR "Use old error function instead of dot product-based one" OFF)
IF(USE_OLD_ERR)
  add_compile_definitions(USE_OLD_ERR=1)
ENDIF()

find_package(X11 REQUIRED)

# add include dir
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${X11_INCLUDE_DIR})

# add libraries
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${X11_LIBRARIES})

# add sources and headers
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/images/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/src/images/*.h)

# add executable
add_executable(drawer drawer.cpp ${sources} ${headers})

# add libraries
set(GLFW_LIBS X11 rt m dl pthread)
target_link_libraries(drawer glfw ${GLFW_LIBS})
