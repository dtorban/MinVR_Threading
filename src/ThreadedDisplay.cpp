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

int frame = 0;

ThreadedDisplay::ThreadedDisplay() : frame(0) {
}

ThreadedDisplay::~ThreadedDisplay() {
	for (int f = 0; f < renderThreads.size(); f++)
	{
		delete renderThreads[f];
	}
	delete threadInfo.barrier;
}

void ThreadedDisplay::use(const MinVR::VRDisplayAction& action) {
}

void ThreadedDisplay::initialize() {
	threadInfo.threadAction = THREADACTION_NONE;
	threadInfo.numThreads = 10;
	threadInfo.numThreadsStarted = 0;
	threadInfo.numThreadsCompleted = 0;
	threadInfo.barrier = new Barrier(threadInfo.numThreads);

	for (int f = 0; f < threadInfo.numThreads; f++)
	{
		renderThreads.push_back(new RenderThread(this, &threadInfo));
	}

	VRDisplayDevice::initialize();
}

void ThreadedDisplay::finishRendering() {
	finishRenderingAllDisplays();

	// Wait for threads to finish rendering
	UniqueMutexLock endActionLock(threadInfo.endActionMutex);
	while (threadInfo.numThreadsCompleted < threadInfo.numThreads) {
		threadInfo.endActionCond.wait(endActionLock);
	}

	endActionLock.unlock();
}

void ThreadedDisplay::startRendering(const MinVR::VRRenderer& renderer, int x) {
	frame++;

	threadInfo.numThreadsStarted = 0;
	threadInfo.numThreadsCompleted = 0;

	threadInfo.startActionMutex.lock();
	threadInfo.threadAction = THREADACTION_RENDER;
	threadInfo.startActionCond.notify_all();
	threadInfo.startActionMutex.unlock();

	std::cout << "start rendering threaded " << frame << std::endl << std::flush;
	startRenderingAllDisplays(renderer, x);

	UniqueMutexLock startedActionLock(threadInfo.startedActionMutex);
	while (threadInfo.numThreadsStarted < threadInfo.numThreads) {
		std::cout << "blah - " << threadInfo.numThreadsStarted << std::endl;
		threadInfo.startedActionCond.wait(startedActionLock);
	}
	startedActionLock.unlock();
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


