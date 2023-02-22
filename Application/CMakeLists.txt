cmake_minimum_required(VERSION 3.16.1)

set(TARGET_NAME VulkanRenderApp)

set(SOURCE_FILES 
	VulkanMain.cpp
)

add_executable( ${TARGET_NAME} WIN32
    ${SOURCE_FILES}
)

find_package(Vulkan REQUIRED)