cmake_minimum_required (VERSION 3.16)

set(APPLICATION_NAME "Rasterizer")

set(BUILD_FOLDER "${CMAKE_SOURCE_DIR}/Build/Binaries")

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/ThirdParty")
set(THIRD_PARTY_BUILD_FOLDER "${BUILD_FOLDER}/ThirdParty")

set(GLAD_INCLUDE_DIR "${THIRD_PARTY_DIR}/glad/include")
set(GLFW_INCLUDE_DIR "${THIRD_PARTY_DIR}/GLFW/include")
set(IMGUI_INCLUDE_DIR "${THIRD_PARTY_DIR}/ImGui")
set(STBIMAGE_INCLUDE_DIR "${THIRD_PARTY_DIR}/stb_image/")


macro(EnableMultiProcessorCompilation TARGET_NAME)
    if(MSVC)
	    target_compile_options(${TARGET_NAME} PUBLIC /MP)
    endif(MSVC)
    #TODO: support other compilers
endmacro()


project (${APPLICATION_NAME})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APPLICATION_NAME})

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)


add_subdirectory("${THIRD_PARTY_DIR}/glad")
add_subdirectory("${THIRD_PARTY_DIR}/GLFW")
add_subdirectory("${THIRD_PARTY_DIR}/ImGui")

add_subdirectory("Core")
add_subdirectory(${APPLICATION_NAME})