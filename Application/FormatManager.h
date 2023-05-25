#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <map>
#include <string>

class VertexFormat;

class FormatManager
{
public:
        
    FormatManager();

    ~FormatManager();

public:

    VertexFormat* getFormat(const std::string& ID);

private:
    void createFormats();

    void createPCFormat();
    void createPCTFormat();
    void createPBRVertexFormat();

private:
    std::map<std::string, VertexFormat*> mIDToVF;
};
