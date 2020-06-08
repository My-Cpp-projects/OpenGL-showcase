



#ifndef SHOWCASE_BASE
#define SHOWCASE_BASE

#include <string>
#include "abstract_showcase.h"

class ShowcaseBase : public AbstractShowcase
{
public:
	ShowcaseBase(std::string&& sceneName);
	virtual ~ShowcaseBase() = default;

	virtual void run() override = 0;

protected:
	virtual void setupWindow();
	virtual void loadModels();
	virtual void setupInitialPositions();
	virtual void loadShaders();
	virtual void setupShaders();

	// defines the call order of an usual scene
	void init(ShowcaseBase* scene);
};

#endif // !SHOWCASE_BASE

