# SS6Player for Cpp (C++/ssfb2)
# 
# Copyright(C) Web Technology Corp.
#                      All rights reserved.

cmake_minimum_required (VERSION 3.8)
project(SS6PlayerForCpp_AppliationMain)

# command options
# TODO: 
# option(ENABLE_CCACHE "Enable ccache?" ON)

# platform detect
# TODO:
if (MSVC)
    set(DEFAULT_STATIC_OPTION YES)
else()
    set(DEFAULT_STATIC_OPTION NO)
endif()

#
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# header-file directories
set(headerDirectories_SS6player
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/Entity"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/Library"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/PrimitiveBatcher"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/Project"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/Sequencer"
    "${CMAKE_CURRENT_SOURCE_DIR}/../OSS"
)
include_directories(${headerDirectories_SS6player})

# platform
set(librariesAdditionTargetPlatform "")
if(OPENGL EQUAL 1)
  ## OpenGL
  set(SS6PlayerForCppApp_srcs ${SS6PlayerForCppApp_srcs}
          OpenGL/main.cpp
          OpenGL/application.cpp
          OpenGL/application.h
          )

  if(TARGET_ENV_WINDOWS EQUAL 1)
    ## Windows
    set(pathGLEW "${CMAKE_CURRENT_SOURCE_DIR}/../Platform/OpenGL/OSS/GLEW")       # GLEW base directory
    set(pathGLFW "${CMAKE_CURRENT_SOURCE_DIR}/../Platform/OpenGL/OSS/GLFW")       # GLFW base directory
    set(pathSTB "${CMAKE_CURRENT_SOURCE_DIR}/../Platform/OpenGL/OSS/STB")         # stb base directory

    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../Platform/OpenGL")         # [SS6Player for CPP] Platform
    include_directories("${pathGLEW}/include")                                    # GLEW
    include_directories("${pathGLFW}/include")                                    # GLFW
    include_directories("${pathSTB}")                                             # STB

    link_directories("${pathGLEW}/lib/Release/${NAME_TARGET_SPEC_GLEW}")
    link_directories("${pathGLFW}/lib-${NAME_TARGET_SPEC_GLFW}")
  else()
  endif()

  # dependency libraries
  set(librariesAdditionTargetPlatform
       glew32s.lib
       glfw3.lib
       opengl32.lib
       )
endif()

# link
add_executable(SS6PlayerForCpp_AppliationMain ${SS6PlayerForCppApp_srcs})
target_link_libraries(SS6PlayerForCpp_AppliationMain SS6PlayerForCpp ${librariesAdditionTargetPlatform})
