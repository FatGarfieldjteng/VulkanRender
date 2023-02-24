cmake_minimum_required(VERSION 3.16.1)

set(TARGET_NAME VulkanRenderApp)

set(HEADER_FILES
	GraphicsSystem.h
	Validation.h
)

set(SOURCE_FILES 
	VulkanMain.cpp
	GraphicsSystem.cpp
	Validation.cpp
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup")

add_executable( ${TARGET_NAME} WIN32
	${HEADER_FILES}
    ${SOURCE_FILES}
)

find_package(Vulkan REQUIRED)

get_filename_component(Vulkan_LIBRARIES_Path ${Vulkan_LIBRARIES} DIRECTORY)

set (GLEW_lib ${Vulkan_LIBRARIES_Path}/glfw3_mt.lib)

message("Vulkan_LIBRARIES is \"${Vulkan_LIBRARIES}\"")
message("GLEW_lib is \"${GLEW_lib}\"")


target_include_directories (VulkanRenderApp 
    PUBLIC ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(VulkanRenderApp 
${Vulkan_LIBRARIES}
${GLEW_lib}
)

target_compile_features( VulkanRenderApp 
	PUBLIC cxx_std_17
)