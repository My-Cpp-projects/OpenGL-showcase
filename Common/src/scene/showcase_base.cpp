



#include "scene/showcase_base.h"

void printDefaultMessage(std::string&& funcName);

ShowcaseBase::ShowcaseBase(std::string&& sceneName)
{
	printf("Creating showcase: %s\n", sceneName.c_str());
}

void ShowcaseBase::setupWindow()
{
	printDefaultMessage("ShowcaseBase::setupWindow");
}

void ShowcaseBase::loadModels()
{
	printDefaultMessage("ShowcaseBase::loadModels");
}

void ShowcaseBase::setupInitialPositions()
{
	printDefaultMessage("ShowcaseBase::setupInitialPositions");
}

void ShowcaseBase::loadShaders()
{
	printDefaultMessage("ShowcaseBase::loadShaders");
}

void ShowcaseBase::setupShaders()
{
	printDefaultMessage("ShowcaseBase::setupShaders");
}

void ShowcaseBase::init(ShowcaseBase* scene)
{
	scene->setupWindow();
	scene->loadModels();
	scene->setupInitialPositions();
	scene->loadShaders();
	scene->setupShaders();
}

// ---------------------------------------------
void printDefaultMessage(std::string&& funcName)
{
	printf("%s was not overriden\n", funcName.c_str());
}