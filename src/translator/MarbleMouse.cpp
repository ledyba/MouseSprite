/*
 * MarbleMouse.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: psi
 */

#include "MarbleMouse.h"
#include <linux/input.h>
#include "../VirtualMouse.h"
#include <cstdio>

namespace mspr {

static const int kScrollLimit = 30;

MarbleMouse::MarbleMouse()
:state_(NORMAL)
,scrollIntegrate_(0)
{

}

MarbleMouse::~MarbleMouse() noexcept
{
}

void MarbleMouse::timeout(VirtualMouse& virt)
{
	switch(this->state_) {
	case NORMAL:
		break;
	case CLICK_RESERVED:
		this->consumeReserved(virt);
		break;
	case SCROLL_RESERVED:
		this->transState(SCROLLING);
		break;
	case SCROLLING:
		break;
	}
}

void MarbleMouse::consumeReserved(VirtualMouse& virt)
{
	for( struct input_event const& ev : this->reserved_ ) {
		virt.transfer(ev);
	}
	this->reserved_.clear();
	transState(NORMAL);
}

void MarbleMouse::transState(enum State const& to)
{
	this->state_ = to;
}

void MarbleMouse::initScroll()
{
	this->transState(SCROLLING);
	this->reserved_.clear();
	this->scrollIntegrate_ = 0;
}

void MarbleMouse::scroll(VirtualMouse& virt, struct ::input_event& ev)
{
	this->scrollIntegrate_ += ev.value;
	int send = 0;
	while( this->scrollIntegrate_ > kScrollLimit ) {
		this->scrollIntegrate_-=kScrollLimit;
		--send;
	}
	while( this->scrollIntegrate_ < -kScrollLimit ) {
		this->scrollIntegrate_+=kScrollLimit;
		++send;
	}
	if(send) virt.sendWheel(ev, send);
}

void MarbleMouse::handle(VirtualMouse& virt, struct ::input_event& ev)
{
	if( ev.type == EV_REL ) {
		switch(this->state_) {
		case NORMAL:
			virt.transfer(ev);
			break;
		case CLICK_RESERVED:
			this->consumeReserved(virt);
			break;
		case SCROLL_RESERVED:
			this->initScroll();
			if( ev.code == REL_Y ) {
				this->scroll(virt, ev);
			}
			break;
		case SCROLLING:
			if( ev.code == REL_Y ) {
				this->scroll(virt, ev);
			}
			break;
		}
	}else if( ev.type == EV_KEY ){
		switch(ev.code) {
		case BTN_LEFT:
		case BTN_RIGHT:
			if( ev.value > 0 ) { //on
				switch(this->state_) {
				case NORMAL:
					transState(CLICK_RESERVED);
					this->reserved_.push_back(ev);
					break;
				case CLICK_RESERVED:
					transState(SCROLL_RESERVED);
					this->reserved_.clear();
					this->reserved_.push_back({ .time=ev.time, .type=EV_KEY, .code=BTN_MIDDLE, .value=1 });
					this->reserved_.push_back({ .time=ev.time, .type=EV_KEY, .code=BTN_MIDDLE, .value=0 });
					break;
				case SCROLL_RESERVED:
				case SCROLLING:
					this->consumeReserved(virt);
					break;
				}
			}else{ //off
				switch(this->state_) {
				case NORMAL:
					virt.transfer(ev);
					break;
				case SCROLLING:
				case CLICK_RESERVED:
				case SCROLL_RESERVED:
					this->consumeReserved(virt);
					virt.transfer(ev);
					break;
				}
			}
			break;
		default:
			virt.transfer(ev);
			break;
		}
	}
}

}
