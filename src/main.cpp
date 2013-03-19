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

#include "translator/MarbleMouseTranslator.h"

using namespace mspr;

int main() {
	Self self;
	RealMouse real;

	VirtualMouse virt;

	MarbleMouseTranslator trans;

	real.attach(trans, virt);

	return 0;
}
