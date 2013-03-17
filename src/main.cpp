//============================================================================
// Name        : mousprite.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Self.h"
#include "VirtualMouse.h"
#include "RealMouse.h"
using namespace mspr;

int main() {
	Self self;
	RealMouse orig;

	VirtualMouse ui;

	orig.process(ui);

	return 0;
}
