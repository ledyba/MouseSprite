/*
 * UInput.h
 *
 *  Created on: Mar 17, 2013
 *      Author: psi
 */

#pragma once

namespace mspr {

class VirtualMouse {
private:
	int fd_;
public:
	VirtualMouse();
	virtual ~VirtualMouse();
public:
	void sendButton(int key, int value);
	void sendRelX(int x);
	void sendRelY(int y);
	void sendWheel(int value);
private:
	void createDevice();
	void removeDevice();
};

}
