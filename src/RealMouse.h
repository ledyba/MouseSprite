/*
 * Original.h
 *
 *  Created on: Mar 17, 2013
 *      Author: psi
 */

#pragma once

#include <vector>
namespace mspr {

class Translator;
class VirtualMouse;
class RealMouse {
private:
	bool grabbed_;
	std::vector<int> fds_;
public:
	RealMouse();
	~RealMouse() noexcept;
public:
	void grab();
	void ungrab();
public:
	void attach(Translator& tr, VirtualMouse& virt);
};

}
