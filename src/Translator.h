/*
 * Translator.h
 *
 *  Created on: Mar 19, 2013
 *      Author: psi
 */

#pragma once
#include <linux/input.h>

namespace mspr {

class RealMouse;
class VirtualMouse;
class Translator {
public:
	Translator() = default;
	virtual ~Translator() noexcept = 0;
public:
	virtual void timeout(VirtualMouse& virt) = 0;
	virtual void handle(VirtualMouse& virt, struct ::input_event& ev) = 0;
};

}
