/*
 * Original.cpp
 *
 *	Created on: Mar 17, 2013
 *			Author: psi
 */

#include <stdexcept>
#include <sys/types.h>
#include <dirent.h>
#include <cstdio>
#include <linux/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include "RealMouse.h"
#include "VirtualMouse.h"

namespace mspr {

static const std::string DEV_INPUT_DIR("/dev/input");

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)	((x)%BITS_PER_LONG)
#define BIT(x)	(1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)	((array[LONG(bit)] >> OFF(bit)) & 1)

static bool testMouseDevice(std::string const& fname)
{
	int fd = open(fname.c_str(), O_RDONLY);
	if (fd < 0) {
		return false;
	}
	unsigned long bit[NBITS(KEY_MAX)];
	std::memset(bit, 0, sizeof(bit));
	ioctl(fd, EVIOCGBIT(EV_REL, REL_MAX), bit);
	close(fd);
	return test_bit(REL_X, bit)
			&& test_bit(REL_Y, bit);
}

RealMouse::RealMouse()
:grabbed_(false)
{
	DIR* input_dir = opendir(DEV_INPUT_DIR.c_str());
	if (input_dir == NULL) {
		throw std::runtime_error("Failed to open input dir.");
	}
	struct dirent* entry;
	struct stat st;
	int fd = 0;
	while ((entry = readdir(input_dir))) {
		std::string dev_file = DEV_INPUT_DIR+"/"+entry->d_name;
		if (stat(dev_file.c_str(), &st) == 0 && S_ISCHR(st.st_mode)) {
			if (testMouseDevice(dev_file)) {
				fd = open(dev_file.c_str(), O_RDWR);
				if (fd > 0) {
					this->fds_.push_back(fd);
				} else {
					std::fprintf(stderr, "Failed to open device: %s\n", dev_file.c_str());
				}
			}
		}
	}
	this->grab();
}

void RealMouse::grab()
{
	if(this->grabbed_) {
		throw std::invalid_argument("already grabbed");
	}
	for(int fd : this->fds_) {
		ioctl(fd, EVIOCGRAB, 1);
	}
	this->grabbed_ = false;
}

void RealMouse::ungrab()
{
	if(this->grabbed_) {
		throw std::invalid_argument("already ungrabbed");
	}
	for(int fd : this->fds_) {
		ioctl(fd, EVIOCGRAB, 0);
	}
	this->grabbed_ = true;
}

RealMouse::~RealMouse()
{
	if(this->grabbed_){
		this->ungrab();
	}
}

void RealMouse::handle(VirtualMouse& uinput, struct ::input_event& ev)
{
	if( ev.type == EV_REL ){
		switch(ev.code) {
		case REL_X:
			std::printf("relx: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			uinput.sendRelX(ev.value);
			break;
		case REL_Y:
			std::printf("rely: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			uinput.sendRelY(ev.value);
			break;
		case REL_WHEEL:
			std::printf("wheel: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			uinput.sendWheel(ev.value);
			break;
		default:
			std::printf("unknown_rel: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			break;
		}
	}else if( ev.type == EV_KEY ){
		switch(ev.code) {
		case BTN_LEFT:
			std::printf("left: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			uinput.sendButton(BTN_LEFT, ev.value);
			break;
		case BTN_RIGHT:
			std::printf("right: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			uinput.sendButton(BTN_RIGHT, ev.value);
			break;
		case BTN_MIDDLE:
			std::printf("middle: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			uinput.sendButton(BTN_MIDDLE, ev.value);
			break;
		case BTN_SIDE:
			std::printf("side: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			uinput.sendButton(BTN_SIDE, ev.value);
			break;
		case BTN_EXTRA:
			std::printf("extra: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			uinput.sendButton(BTN_EXTRA, ev.value);
			break;
		case BTN_FORWARD:
			std::printf("forward: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			uinput.sendButton(BTN_FORWARD, ev.value);
			break;
		case BTN_BACK:
			std::printf("back: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			uinput.sendButton(BTN_BACK, ev.value);
			break;
		case BTN_TASK:
			std::printf("task: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			uinput.sendButton(BTN_TASK, ev.value);
			break;
		default:
			std::printf("unknown_key: %04x %04x %04x\n", ev.type, ev.code, ev.value);
			break;
		}
	}else if(EV_SYN == ev.type){
	}else{
		std::printf("unknown: %04x %04x %04x\n", ev.type, ev.code, ev.value);
	}
}

void RealMouse::process(VirtualMouse& uinput)
{
	struct ::input_event event;
	fd_set fdset, key_fdset;
	int maxfd = 0;

	FD_ZERO(&key_fdset);
	for(int fd : this->fds_) {
		FD_SET(fd, &key_fdset);
		maxfd = std::max(fd, maxfd);
	}

	while (1) {
		std::memcpy(&fdset, &key_fdset, sizeof(fd_set));
		int ret = select(maxfd + 1, &fdset, NULL, NULL, NULL);
		if (ret <= 0) {
			throw std::runtime_error("Failed to call #select");
		}
		for(int fd : this->fds_) {
			if (FD_ISSET(fd, &fdset) && read(fd, &event, sizeof(event)) == sizeof(event)){
				this->handle(uinput, event);
			}
		}
	}
}

}
