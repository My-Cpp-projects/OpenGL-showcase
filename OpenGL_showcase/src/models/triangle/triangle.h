



#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <memory>
#include "../mesh.h"

struct TriangleData;

class Triangle : public Mesh
{
public:
	Triangle(std::shared_ptr<TriangleData> data);
	virtual ~Triangle();

private:
	 std::shared_ptr<TriangleData> m_data;
};

#endif // !TRIANGLE_H

