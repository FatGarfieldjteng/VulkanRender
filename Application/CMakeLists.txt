cmake_minimum_required(VERSION 3.16.1)

set(TARGET_NAME VulkanRenderApp)

set(SOURCE_FILES 
	VulkanMain.cpp
)

add_executable( ${TARGET_NAME} WIN32
    ${SOURCE_FILES}
)

find_package(Vulkan REQUIRED)

target_include_directories (VulkanRenderApp 
    PUBLIC ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(VulkanRenderApp ${Vulkan_LIBRARIES})

target_compile_features( VulkanRenderApp 
	PUBLIC cxx_std_17
)