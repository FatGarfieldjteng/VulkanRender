#include "FormatManager.h"
#include "PCVertexFormat.h"

FormatManager::FormatManager()
{
	createFormats();
}

FormatManager::~FormatManager()
{
	
}

void FormatManager::createFormats()
{
	createPCFormat();
}


void FormatManager::createPCFormat()
{
	PCVertexFormat* format = new PCVertexFormat();
	format->init();

	mIDToVF["PCFormat"] = format;
}

VertexFormat* FormatManager::getFormat(const std::string& ID)
{
	std::map<std::string, VertexFormat* >::iterator it = mIDToVF.find(ID);

	if (it != mIDToVF.end())
	{
		return  it->second;
	}
	else
	{
		return nullptr;
	}
}