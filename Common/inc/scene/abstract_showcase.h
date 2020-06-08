



#ifndef ABSTRACT_SHOWCASE_H
#define ABSTRACT_SHOWCASE_H

class AbstractShowcase
{
public:
	// Used for render loop
	virtual void run() = 0;

	virtual ~AbstractShowcase() = default;
};

#endif // !ABSTRACT_SHOWCASE_H

