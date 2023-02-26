#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>

class Device;

class Queue
{
public:
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
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