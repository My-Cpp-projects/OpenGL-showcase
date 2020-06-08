



#include <memory>
#include "cube_showcase.h"

int main()
{
	std::unique_ptr<AbstractShowcase> showcase = std::make_unique<CubeShowcase>("CubeScene");
	showcase->run();

	return 0;
}
