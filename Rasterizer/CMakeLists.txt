cmake_minimum_required (VERSION 3.16)

project(${APPLICATION_NAME})

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS 
	"Source/*.h" 
	"Source/*.cpp" 
)

add_executable(${APPLICATION_NAME} WIN32 ${SRC_FILES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC_FILES} )

EnableMultiProcessorCompilation(${APPLICATION_NAME})
set_property(TARGET ${APPLICATION_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${APPLICATION_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY  "${CMAKE_SOURCE_DIR}/${APPLICATION_NAME}")
set_property(TARGET ${APPLICATION_NAME} PROPERTY DEBUG_POSTFIX "")

# Build folder
set_target_properties(${APPLICATION_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${BUILD_FOLDER}/${APPLICATION_NAME}"
	LIBRARY_OUTPUT_DIRECTORY "${BUILD_FOLDER}/${APPLICATION_NAME}"
	RUNTIME_OUTPUT_DIRECTORY "${BUILD_FOLDER}/${APPLICATION_NAME}"
)

target_include_directories(${APPLICATION_NAME} PRIVATE 
	"${CMAKE_SOURCE_DIR}/Core" 
	"Source/"
	${THIRD_PARTY_DIR}
	${IMGUI_INCLUDE_DIR}
)

target_link_libraries(${APPLICATION_NAME} PRIVATE Core)
