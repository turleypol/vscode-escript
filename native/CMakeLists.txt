cmake_minimum_required(VERSION 3.9)
cmake_policy(SET CMP0042 NEW)
set (CMAKE_CXX_STANDARD 11)

if(APPLE AND NOT DEFINED ENV{BUILD_OSX_SLIM_BINARY})
  message("** Building universal macOS binary")
  set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
  set(BUILD_OSX_UNIVERSAL_BINARY TRUE)
  set(PIPED_OSX_ARCHITECTURES "arm64|x86_64") # For ExternalProject
endif()

project (vscode-escript-native)
include_directories(${CMAKE_JS_INC})
set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB_RECURSE SOURCE_FILES "cpp/*.cc" "cpp/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

if(APPLE)
  set(PLATFORM darwin)
elseif(WIN32)
  set(PLATFORM win32)
else()
  set(PLATFORM linux)
endif()

if(BUILD_OSX_UNIVERSAL_BINARY)
  set(ARCH universal)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm.*|ARM.*)")
  set(ARCH arm64)
else()
  set(ARCH x64)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "vscode-escript-native.${PLATFORM}-${ARCH}")

target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  # Generate node.lib
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()

execute_process(
  COMMAND node -p "require('node-addon-api').include"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE NODE_ADDON_API_DIR
)
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC 
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
endif()


#if (MSVC)
#  set(CMAKE_CXX_FLAGS_RELEASE "/MT")
#  set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
#endif()
set(BSCRIPT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pol")
set(BUILD_ALL OFF CACHE BOOL "nothing" FORCE)
add_subdirectory(${BSCRIPT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} bscript plib libtinyxml)

#if(MSVC)
#    add_compile_options(
#        $<$<CONFIG:>:/MT> #---------|
#        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
#        $<$<CONFIG:Release>:/MT> #--|
#    )
#endif()

add_definitions(-DNAPI_VERSION=5)
