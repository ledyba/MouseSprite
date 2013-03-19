/*
 * Self.h
 *
 *  Created on: Mar 17, 2013
 *      Author: psi
 */

#pragma once

namespace mspr {

class Self {
private:
	int fd_;
public:
	Self();
	~Self() noexcept;
};

}
