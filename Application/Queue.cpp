#include "Queue.h"
#include "Device.h"

Queue::Queue(Type type)
	:mType(type)
{

}

Queue::~Queue()
{
   
}

void Queue::acquireQueue(Device* device)
{
	device->acquireQueue(mType, &mQueue);
}

Queue::Type Queue::getType()
{
	return mType;
}

Queue::QueueFamilyIndices Queue::findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    Queue::QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (size_t i = 0; i < queueFamilies.size(); ++i)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = static_cast<uint32_t>(i);
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = (uint32_t)i;
        }

        if (indices.isComplete()) {
            break;
        }

    }

    return indices;
}