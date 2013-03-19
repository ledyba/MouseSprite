/*
 * Self.cpp
 *
 *	Created on: Mar 17, 2013
 *			Author: psi
 */

#include "Self.h"
#include <unistd.h>
#include <fcntl.h>
#include <utility>
#include <stdexcept>
#include <sys/file.h>

namespace mspr {

Self::Self()
{
	char buf[1024];
	int len = readlink("/proc/self/exe", buf, sizeof(buf) - 1);
	if (len > -1) {
		buf[len] = '\0';
	} else {
		throw std::runtime_error("failed to read /proc/self/exec");
	}

	int const fd = open(buf, O_RDONLY);
	if (fd <= 0) {
		throw std::runtime_error("failed to read self.");
	}

	this->fd_ = flock(fd, LOCK_EX | LOCK_NB);
	if (this->fd_ != 0) {
		throw std::runtime_error("failed to read self.");
	}
}

Self::~Self() noexcept
{
	flock(this->fd_, LOCK_UN);
	close(this->fd_);
}

}
