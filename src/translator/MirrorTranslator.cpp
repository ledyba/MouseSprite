/*
 * MirrorTranslator.cpp
 *
 *  Created on: Mar 19, 2013
 *      Author: psi
 */

#include "MirrorTranslator.h"
#include <linux/input.h>
#include "../VirtualMouse.h"
#include <cstdio>

namespace mspr {

MirrorTranslator::MirrorTranslator()
{
}

MirrorTranslator::~MirrorTranslator() noexcept
{
}

void MirrorTranslator::timeout(VirtualMouse& virt)
{

}

void MirrorTranslator::handle(VirtualMouse& virt, struct ::input_event& ev)
{
//	if( ev.type == EV_REL ){
//		switch(ev.code) {
//		case REL_X:
//			std::printf("relx: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			virt.sendRelX(ev.value);
//			break;
//		case REL_Y:
//			std::printf("rely: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			virt.sendRelY(ev.value);
//			break;
//		case REL_WHEEL:
//			std::printf("wheel: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			virt.sendWheel(ev.value);
//			break;
//		default:
//			std::printf("unknown_rel: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			break;
//		}
//	}else if( ev.type == EV_KEY ){
//		switch(ev.code) {
//		case BTN_LEFT:
//			std::printf("left: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			virt.sendButton(BTN_LEFT, ev.value);
//			break;
//		case BTN_RIGHT:
//			std::printf("right: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			virt.sendButton(BTN_RIGHT, ev.value);
//			break;
//		case BTN_MIDDLE:
//			std::printf("middle: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			virt.sendButton(BTN_MIDDLE, ev.value);
//			break;
//		case BTN_SIDE:
//			std::printf("side: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			virt.sendButton(BTN_SIDE, ev.value);
//			break;
//		case BTN_EXTRA:
//			std::printf("extra: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			virt.sendButton(BTN_EXTRA, ev.value);
//			break;
//		case BTN_FORWARD:
//			std::printf("forward: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			virt.sendButton(BTN_FORWARD, ev.value);
//			break;
//		case BTN_BACK:
//			std::printf("back: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			virt.sendButton(BTN_BACK, ev.value);
//			break;
//		case BTN_TASK:
//			std::printf("task: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			virt.sendButton(BTN_TASK, ev.value);
//			break;
//		default:
//			std::printf("unknown_key: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//			break;
//		}
//	}else if(EV_SYN == ev.type){
//	}else{
//		std::printf("unknown: %04x %04x %04x\n", ev.type, ev.code, ev.value);
//	}
	virt.transfer(ev);
}


}
