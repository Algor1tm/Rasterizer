cmake_minimum_required (VERSION 3.16)

function(AddLibrary TARGET_NAME)
    EnableMultiProcessorCompilation(${TARGET_NAME})
    set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER Dependencies)

    set_target_properties(${TARGET_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${THIRD_PARTY_BUILD_FOLDER}/${TARGET_NAME}"
	LIBRARY_OUTPUT_DIRECTORY "${THIRD_PARTY_BUILD_FOLDER}/${TARGET_NAME}"
	RUNTIME_OUTPUT_DIRECTORY "${THIRD_PARTY_BUILD_FOLDER}/${TARGET_NAME}")

    target_link_libraries(Core PRIVATE ${TARGET_NAME})
endfunction()


project(Core)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS 
	"*.h" 
	"*.cpp" 
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC_FILES} )

file(GLOB_RECURSE STB_IMAGE_FILES CONFIGURE_DEPENDS 
	"${THIRD_PARTY_DIR}/stb_image/stb_image.h"
	"${THIRD_PARTY_DIR}/stb_image/stb_image.cpp"
)

source_group("ThirdParty/stb_image" FILES ${STB_IMAGE_FILES} )

add_library(Core ${SRC_FILES} ${STB_IMAGE_FILES})


EnableMultiProcessorCompilation(Core)
set_property(TARGET Core PROPERTY CXX_STANDARD 20)
set_property(TARGET Core PROPERTY DEBUG_POSTFIX "")
set_property(TARGET Core PROPERTY FOLDER Core)


target_compile_definitions(Core PRIVATE
	$<$<CONFIG:Debug>:APP_DEBUG>
	$<$<CONFIG:Release>:APP_RELEASE>
	MATH_SIMD
)

# Build folder
set_target_properties(Core
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${BUILD_FOLDER}/Core"
	LIBRARY_OUTPUT_DIRECTORY "${BUILD_FOLDER}/Core"
	RUNTIME_OUTPUT_DIRECTORY "${BUILD_FOLDER}/Core"
)

# Precompile Header
target_precompile_headers(Core PRIVATE
	<memory> <algorithm> <functional> <utility> <chrono>
	<iomanip> <thread> <filesystem> <limits> <random>
	<numeric> <math.h> <cmath> <iostream> <sstream>
	<fstream> <array> <vector> <string> <unordered_map> <unordered_set>

	Core/Core.h
)

target_include_directories(Core PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR} 
	${THIRD_PARTY_DIR}
	${GLAD_INCLUDE_DIR}
	${GLFW_INCLUDE_DIR}
	${IMGUI_INCLUDE_DIR}
)

AddLibrary(glad)
AddLibrary(GLFW)
AddLibrary(ImGui)

# Windows specific code
if(WIN32)
	target_link_libraries(Core PRIVATE opengl32.lib)
	target_compile_definitions(Core PUBLIC APP_WINDOWS)
endif(WIN32)

# Linux specific code
if(UNIX AND NOT APPLE)
	set_property(TARGET Core PROPERTY POSITION_INDEPENDENT_CODE ON)
	target_link_libraries(Core PRIVATE opengl32.lib)
    target_compile_definitions(Core PUBLIC APP_LINUX)
endif(UNIX AND NOT APPLE)
