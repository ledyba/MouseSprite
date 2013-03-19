/*
 * UInput.cpp
 *
 *Created on: Mar 17, 2013
 *Author: psi
 */

#include <cstdio>
#include <stdexcept>
#include <linux/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <cstring>
#include <linux/input.h>
#include <linux/uinput.h>
#include "VirtualMouse.h"

namespace mspr {

static const std::string uinput_devices[] = {
	"/dev/uinput",
	"/dev/input/uinput"
};

static const std::string uinput_devname="mousprite";

VirtualMouse::VirtualMouse()
{
	struct stat st;
	for(std::string const& ui : uinput_devices) {
		if( stat(ui.c_str(), &st) == 0 && S_ISCHR(st.st_mode) ) {
			this->fd_ = open(ui.c_str(), O_WRONLY | O_NONBLOCK);
			this->createDevice();
			return;
		}
	}
	throw std::invalid_argument("Uinput device not found.");
}

VirtualMouse::~VirtualMouse() noexcept
{
	if(close(this->fd_) < 0){
		std::fprintf(stderr, "Oops. Failed to close uinput: %d", errno);
	}
}

void VirtualMouse::createDevice()
{
	struct uinput_user_dev dev;
	memset(&dev, 0, sizeof(dev));
	std::strcpy(dev.name, uinput_devname.c_str());
	write(this->fd_, &dev, sizeof(dev));

	ioctl(this->fd_, UI_SET_EVBIT, EV_REL);
	ioctl(this->fd_, UI_SET_RELBIT, REL_X);
	ioctl(this->fd_, UI_SET_RELBIT, REL_Y);
	ioctl(this->fd_, UI_SET_RELBIT, REL_WHEEL);

	ioctl(this->fd_, UI_SET_EVBIT, EV_KEY);
	ioctl(this->fd_, UI_SET_KEYBIT, BTN_LEFT);
	ioctl(this->fd_, UI_SET_KEYBIT, BTN_RIGHT);
	ioctl(this->fd_, UI_SET_KEYBIT, BTN_MIDDLE);
	ioctl(this->fd_, UI_SET_KEYBIT, BTN_SIDE);
	ioctl(this->fd_, UI_SET_KEYBIT, BTN_EXTRA);
	ioctl(this->fd_, UI_SET_KEYBIT, BTN_FORWARD);
	ioctl(this->fd_, UI_SET_KEYBIT, BTN_BACK);

	ioctl(this->fd_, UI_DEV_CREATE, 0);
}

void VirtualMouse::sendButton(struct input_event const& ev, int key, int value)
{
	struct input_event event;
	event.time = ev.time;
	event.type = EV_KEY;
	event.code = key;
	event.value = value;
	write(fd_, &event, sizeof(event));

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(fd_, &event, sizeof(event));
}

void VirtualMouse::sendRelX(struct input_event const& ev, int x)
{
	struct input_event event;
	event.time = ev.time;
	event.type = EV_REL;
	event.code = REL_X;
	event.value = x;
	write(fd_, &event, sizeof(event));

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(fd_, &event, sizeof(event));
}
void VirtualMouse::sendRelY(struct input_event const& ev, int y)
{
	struct input_event event;
	event.time = ev.time;
	event.type = EV_REL;
	event.code = REL_Y;
	event.value = y;
	write(fd_, &event, sizeof(event));

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(fd_, &event, sizeof(event));
}

void VirtualMouse::sendWheel(struct input_event const& ev, int value)
{
	struct input_event event;
	event.time = ev.time;
	event.type = EV_REL;
	event.code = REL_WHEEL;
	event.value = value;
	write(fd_, &event, sizeof(event));

	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(fd_, &event, sizeof(event));
}

void VirtualMouse::transfer(struct input_event const& ev)
{
	write(fd_, &ev, sizeof(ev));

	struct input_event event;
	event.time = ev.time;
	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	write(fd_, &event, sizeof(event));

}

void VirtualMouse::removeDevice()
{
	ioctl(this->fd_, UI_DEV_DESTROY, 0);
}

}
