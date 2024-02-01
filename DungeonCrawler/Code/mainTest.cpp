#include <iostream>
#include "Game.h"
#include "Math/Vector2D.h"

// for small tests
// dont delete gDeltaT
float gDeltaT{ 1.0f };

class A
{
public:
	A();
	~A();

private:

};

A::A()
{
}

A::~A()
{
}
 
class B : public A
{
public:
	B();
	~B();

private:

};

B::B()
{
}

B::~B()
{
}


int main(int argc, char* arvg[])
{
	A testA{};
	B testB{};

	A* testPA = &testA;

	std::shared_ptr<A> testPA2 = &testA;



	return 0;
}
