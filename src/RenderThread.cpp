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

RenderThread::RenderThread(VRDisplayDevice* display) : threadAction(NONE) {
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
	while (true)
	{
		if (threadAction == NONE)
		{
			std::cout << "NONE" << std::endl;
		}
		else if (threadAction == RENDER)
		{
			std::cout << "RENDER" << std::endl;
		}

	}
}

} /* namespace MinVR */
