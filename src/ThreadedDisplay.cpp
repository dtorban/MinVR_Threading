/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "ThreadedDisplay.h"
#include <iostream>

namespace MinVR {

ThreadedDisplay::ThreadedDisplay() {
}

ThreadedDisplay::~ThreadedDisplay() {
	delete renderThread;
}

void ThreadedDisplay::use(const MinVR::VRDisplayAction& action) {
}

void ThreadedDisplay::initialize() {
	renderThread = new RenderThread(this);
	VRDisplayDevice::initialize();
}

void ThreadedDisplay::finishRendering() {
	finishRenderingAllDisplays();
}

void ThreadedDisplay::startRendering(const MinVR::VRRenderer& renderer, int x) {
	std::cout << "start rendering threaded" << std::endl;
	startRenderingAllDisplays(renderer, x);
}

ThreadedDisplayFactory::ThreadedDisplayFactory() {
}

ThreadedDisplayFactory::~ThreadedDisplayFactory() {
}

VRDisplayDevice* ThreadedDisplayFactory::createDisplay(
		const std::string type, const std::string name, VRDataIndex& config,
		VRDisplayDeviceFactory* factory) {
	if (type == "thread_group")
	{
		return new ThreadedDisplay();
	}
	return NULL;
}

} /* namespace MinVR */


