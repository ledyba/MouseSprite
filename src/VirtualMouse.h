/*
 * UInput.h
 *
 *  Created on: Mar 17, 2013
 *      Author: psi
 */

#pragma once
#include <linux/input.h>
#include <time.h>

namespace mspr {

class VirtualMouse {
private:
	int fd_;
public:
	VirtualMouse();
	~VirtualMouse() noexcept;
public:
	void sendButton(struct input_event const& ev, int key, int value);
	void inline sendButton(int key, int value);
	void sendRelX(struct input_event const& ev, int x);
	void inline sendRelX(int x);
	void sendRelY(struct input_event const& ev, int y);
	void inline sendRelY(int y);
	void sendWheel(struct input_event const& ev, int value);
	void inline sendWheel(int value);
	void transfer(struct input_event const& ev);
private:
	void createDevice();
	void removeDevice();
};

void inline VirtualMouse::sendButton(int key, int value) {
	struct input_event ev;
	::gettimeofday(&ev.time, NULL);
	this->sendButton(ev, key, value);
}
void inline VirtualMouse::sendRelX(int x) {
	struct input_event ev;
	::gettimeofday(&ev.time, NULL);
	this->sendRelX(ev, x);
}
void inline VirtualMouse::sendRelY(int y) {
	struct input_event ev;
	::gettimeofday(&ev.time, NULL);
	this->sendRelY(ev, y);
}
void inline VirtualMouse::sendWheel(int value) {
	struct input_event ev;
	::gettimeofday(&ev.time, NULL);
	this->sendWheel(ev, value);
}

}
