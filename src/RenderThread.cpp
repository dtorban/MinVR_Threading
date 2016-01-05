/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "RenderThread.h"
#include <iostream>

namespace MinVR {

RenderThread::RenderThread(VRDisplayDevice* display, RenderThreadInfo* threadInfo)
	: threadInfo(threadInfo), frame(0) {
	// TODO Auto-generated constructor stub
	_thread = new Thread(&RenderThread::render, this);
}

RenderThread::~RenderThread() {
	// TODO Auto-generated destructor stub
	if (_thread) {
		_thread->join();
	}

	delete _thread;
}

void RenderThread::render() {
	threadInfo->barrier->wait();

	while (true)
	{
		frame++;

		// Wait for the main thread to signal that it's ok to start rendering
		UniqueMutexLock startActionLock(threadInfo->startActionMutex);
		while (threadInfo->threadAction == THREADACTION_NONE) {
			threadInfo->startActionCond.wait(startActionLock);
		}
		if (threadAction == THREADACTION_TERMINATE) {
			// RENDERING_TERMINATE is a special flag used to quit the application and cleanup all the threads nicely
			startActionLock.unlock();
			return;
		}

		if (threadInfo->threadAction == THREADACTION_NONE)
		{
			std::cout << "NONE" << std::endl << std::flush;
		}
		else if (threadInfo->threadAction == THREADACTION_RENDER)
		{
			std::cout << "RENDER " << frame << std::endl << std::flush;
		}
		else if (threadInfo->threadAction == THREADACTION_ACTION)
		{
			std::cout << "ACTION" << std::endl << std::flush;
		}

		threadInfo->startedActionMutex.lock();
		threadInfo->numThreadsStarted++;
		if (threadInfo->numThreadsStarted >= threadInfo->numThreads)
		{
			threadInfo->threadAction = THREADACTION_NONE;
		}
		startActionLock.unlock();

		threadInfo->startedActionCond.notify_all();
		threadInfo->startedActionMutex.unlock();

		threadInfo->barrier->wait();

		threadInfo->endActionMutex.lock();
		threadInfo->numThreadsCompleted++;
		threadInfo->endActionCond.notify_all();
		threadInfo->endActionMutex.unlock();
	}
}

} /* namespace MinVR */
