/*
 * MirrorTranslator.h
 *
 *  Created on: Mar 19, 2013
 *      Author: psi
 */

#pragma once

#include "../Translator.h"

namespace mspr {

class MirrorTranslator: public mspr::Translator {
public:
	MirrorTranslator();
	virtual ~MirrorTranslator() noexcept;
public:
	virtual void timeout(VirtualMouse& virt) override final;
	virtual void handle(VirtualMouse& virt, struct ::input_event& ev) override final;
};

}
