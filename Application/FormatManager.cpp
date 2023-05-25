#include "FormatManager.h"
#include "PCVertexFormat.h"
#include "PCTVertexFormat.h"
#include "PBRVertexFormat.h"

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
	createPCTFormat();
	createPBRVertexFormat();
}


void FormatManager::createPCFormat()
{
	PCVertexFormat* format = new PCVertexFormat();
	format->init();

	mIDToVF["PCFormat"] = format;
}

void FormatManager::createPCTFormat()
{
	PCTVertexFormat* format = new PCTVertexFormat();
	format->init();

	mIDToVF["PCTFormat"] = format;
}

void FormatManager::createPBRVertexFormat()
{
	PBRVertexFormat* format = new PBRVertexFormat();
	format->init();

	mIDToVF["PBRFormat"] = format;
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