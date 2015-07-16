#pragma once
#include "Level.h"
// Singleton http://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class Uber
{
public:
	static Uber& I() {
		static Uber instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}
	Level* level;
	float gravity = 9.8f;
private:
	Uber() {};                   // Constructor? (the {} brackets) are needed here.

	// C++ 11
	// =======
	// We can use the better technique of deleting the methods
	// we don't want.
	Uber(Uber const&) = delete;
	void operator=(Uber const&) = delete;

};