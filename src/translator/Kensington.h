/*
 * Kensington.h
 *
 *  Created on: Mar 19, 2013
 *      Author: psi
 */

#pragma once

#include "../Translator.h"

namespace mspr {

class Kensington: public mspr::Translator {
public:
	Kensington();
	virtual ~Kensington() noexcept;
public:
	virtual void timeout(VirtualMouse& virt) override final;
	virtual void handle(VirtualMouse& virt, struct ::input_event& ev) override final;
};

}
