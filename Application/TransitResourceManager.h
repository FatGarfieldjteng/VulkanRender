#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <map>
#include <string>

class TransitResource;
class Device;

class TransitResourceManager
{
public:

    TransitResourceManager(Device* device);

    ~TransitResourceManager();

public:
    void createResources();

    void addResource(const std::string& ID, TransitResource* resource);
    
    TransitResource* getResource(const std::string& ID);
    

private:
    void createTransitDepthMap();
    
private:
    
    Device* mDevice = nullptr;
    std::map<std::string, TransitResource*> mIDToResource;
};
