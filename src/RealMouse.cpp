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
#include "Translator.h"
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

RealMouse::~RealMouse() noexcept
{
	if(this->grabbed_){
		this->ungrab();
	}
}

void RealMouse::attach(Translator& tr, VirtualMouse& virt)
{
	fd_set fdset, key_fdset;
	int maxfd = 0;

	FD_ZERO(&key_fdset);
	for(int fd : this->fds_) {
		FD_SET(fd, &key_fdset);
		maxfd = std::max(fd, maxfd);
	}

	struct ::input_event ev;
	struct ::timeval timeout;
	while (1) {
		std::memcpy(&fdset, &key_fdset, sizeof(fd_set));
		timeout.tv_sec = 0;
		timeout.tv_usec = 50 * 1000;
		int ret = select(maxfd + 1, &fdset, NULL, NULL, &timeout);
		if (ret > 0) {
			for(int fd : this->fds_) {
				if (FD_ISSET(fd, &fdset) && read(fd, &ev, sizeof(ev)) == sizeof(ev)){
					tr.handle(virt, ev);
				}
			}
		}else if(ret == 0){
			tr.timeout(virt);
		}else{
			throw std::runtime_error("Failed to call #select");
		}
	}
}

}
