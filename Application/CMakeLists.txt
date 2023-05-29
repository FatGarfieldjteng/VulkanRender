cmake_minimum_required(VERSION 3.16.1)

set(TARGET_NAME VulkanRenderApp)

set(HEADER_FILES
	GraphicsSystem.h
	Validation.h
	Device.h
	Queue.h
	SwapChain.h
	DepthStencilBuffer.h
	ShaderManager.h
	Pipeline.h
	PassManager.h
	PipelineManager.h
	SyncObjectManager.h
	ConstantBufferManager.h
	ConstantBuffer.h
	Texture.h
	Managers.h
	FrameBuffer.h
	Command.h
	VertexFormat.h
	PBRVertexFormat.h
	FormatManager.h
	VertexBuffer.h
	IndexBuffer.h
	PCVertexFormat.h
	PCTVertexFormat.h
	Mesh.h
	BoundingBox.h
	Scene.h
	PBRMaterial.h
	Camera.h
	SimpleScene.h
	RenderPass.h
	ClearRenderPass.h
	BeautyRenderPass.h
	FinalRenderPass.h
	BeautyRenderPass.h
	ShadowRenderPass.h
	RenderPassManager.h
	tinyobjloader/tiny_obj_loader.h
	stb/stb_image.h
)

set(SOURCE_FILES 
	VulkanMain.cpp
	GraphicsSystem.cpp
	Validation.cpp
	Device.cpp
	Queue.cpp
	SwapChain.cpp
	ShaderManager.cpp
	Pipeline.cpp
	DepthStencilBuffer.cpp
	PassManager.cpp
	PipelineManager.cpp
	SyncObjectManager.cpp
	ConstantBufferManager.cpp
	ConstantBuffer.cpp
	Texture.cpp
	Managers.cpp
	FrameBuffer.cpp
	Command.cpp
	VertexFormat.cpp
	PBRVertexFormat.cpp
	FormatManager.cpp
	VertexBuffer.cpp
	IndexBuffer.cpp
	PCVertexFormat.cpp
	PCTVertexFormat.cpp
	Mesh.cpp
	BoundingBox.cpp
	Scene.cpp
	PBRMaterial.cpp
	Camera.cpp
	SimpleScene.cpp
	RenderPass.cpp
	ClearRenderPass.cpp
	BeautyRenderPass.cpp
	FinalRenderPass.cpp
	ShadowRenderPass.cpp
	RenderPassManager.cpp
)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup")

add_executable( ${TARGET_NAME} WIN32
	${HEADER_FILES}
    ${SOURCE_FILES}
)

find_package(assimp REQUIRED)
if (assimp_FOUND)
    link_directories(${ASSIMP_LIBRARY_DIRS})
    include_directories(${ASSIMP_INCLUDE_DIRS})
    target_link_libraries (VulkanRenderApp ${ASSIMP_LIBRARIES})
endif(assimp_FOUND)

find_package(Vulkan REQUIRED)

get_filename_component(Vulkan_LIBRARIES_Path ${Vulkan_LIBRARIES} DIRECTORY)

set (GLEW_lib ${Vulkan_LIBRARIES_Path}/glfw3_mt.lib)

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

add_subdirectory(Shader)