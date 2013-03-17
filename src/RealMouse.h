/*
 * Original.h
 *
 *  Created on: Mar 17, 2013
 *      Author: psi
 */

#pragma once

#include <vector>
#include <linux/input.h>
namespace mspr {

class VirtualMouse;
class RealMouse {
private:
	bool grabbed_;
	std::vector<int> fds_;
public:
	RealMouse();
	virtual ~RealMouse();
public:
	void grab();
	void ungrab();
public:
	void process(VirtualMouse& uinput);
private:
	void handle(VirtualMouse& uinput, struct ::input_event& ev);
};

}
