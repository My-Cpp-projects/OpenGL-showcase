



#ifndef MODEL_H
#define MODEL_H

class Model
{
public:
	virtual ~Model() {}

	virtual void draw() = 0;
	virtual void handleKey() {};
	virtual void handleMouseButton() {};
	virtual void handleScrolling() {};
};

#endif // !MODEL_H

