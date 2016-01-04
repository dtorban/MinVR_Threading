/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef RENDERTHREAD_H_
#define RENDERTHREAD_H_

#include "display/VRDisplayDevice.h"
#include "Thread.h"

namespace MinVR {

class RenderThread {
public:
	enum RenderThreadAction { NONE, ACTION, RENDER, EXIT };

	RenderThread(VRDisplayDevice* display);
	virtual ~RenderThread();

	void render();

	void setAction(const VRDisplayAction* action) {
		this->action = action;
	}

	void setRenderer(const VRRenderer* renderer) {
		this->renderer = renderer;
	}

	void setThreadAction(RenderThreadAction threadAction) {
		this->threadAction = threadAction;
	}

private:
	VRDisplayDevice* display;
	const VRDisplayAction* action;
	const VRRenderer* renderer;
	RenderThreadAction threadAction;
	Thread* _thread;
};

} /* namespace MinVR */

#endif /* RENDERTHREAD_H_ */
