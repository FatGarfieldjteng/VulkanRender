#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

class Device;

class Queue
{
public:

	enum class Type
	{
		Present = 1,
		Graphics
	};

	Queue(Type type);
	~Queue();

public:
	void acquireQueue(Device* device);
	Type getType();

private:
	VkQueue mQueue = VK_NULL_HANDLE;
	Type mType;
};