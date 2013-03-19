/*
 * MarbleMouse.h
 *
 *  Created on: Mar 19, 2013
 *      Author: psi
 */

#pragma once
#include <linux/input.h>
#include <vector>
#include "../Translator.h"

namespace mspr {

class MarbleMouse : public Translator{
public:
	MarbleMouse();
	virtual ~MarbleMouse() noexcept;
private:
	enum State{
		NORMAL,
		CLICK_RESERVED,
		SCROLL_RESERVED,
		SCROLLING
	} state_;
	int scrollIntegrate_;
	std::vector<struct ::input_event> reserved_;
private:
	void consumeReserved(VirtualMouse& virt);
	void transState(enum State const& to);
private:
	void initScroll();
	void scroll(VirtualMouse& virt, struct ::input_event& ev);
private:
	virtual void timeout(VirtualMouse& virt) override final;
	virtual void handle(VirtualMouse& virt, struct ::input_event& ev) override final;
};

}
