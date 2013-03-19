/*
 * Kensington.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: psi
 */

#include "Kensington.h"
#include <linux/input.h>
#include "../VirtualMouse.h"
#include <cstdio>

namespace mspr {

Kensington::Kensington()
{
}

Kensington::~Kensington() noexcept
{
}

void Kensington::timeout(VirtualMouse& virt)
{

}

void Kensington::handle(VirtualMouse& virt, struct ::input_event& ev)
{
	if( ev.type == EV_REL && ev.code == REL_WHEEL ){
		virt.sendWheel(ev, ev.value*3);
	}else{
		virt.transfer(ev);
	}
}


}
