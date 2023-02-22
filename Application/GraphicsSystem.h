#pragma once
class GraphicsSystem
{
public:

	GraphicsSystem();
	~GraphicsSystem();

public:
    void run();

private:
    void initVulkan();

    void mainLoop();

    void cleanup();
};