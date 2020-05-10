



#ifndef TRIANGLE_VIEW_H
#define TRIANGLE_VIEW_H

#include <memory>
#include "../mesh_view.h"

struct TriangleData;

class TriangleView : public MeshView
{
public:
	TriangleView(std::shared_ptr<TriangleData*> data);

	virtual void draw() override;

private:
	std::shared_ptr<TriangleData> m_data;
};

#endif // !TRIANGLE_VIEW_H

