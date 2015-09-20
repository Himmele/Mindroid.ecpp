/*
 * Copyright (C) 2011 Daniel Himmelein
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MYDROID_LOOPERTHREAD_H_
#define MYDROID_LOOPERTHREAD_H_

#include <stdint.h>
#include <new>
#include "mindroid/util/Utils.h"
#include "mindroid/os/Thread.h"
#include "mindroid/os/Looper.h"
#include "mindroid/os/Handler.h"
#include "mindroid/os/Lock.h"
#include "mindroid/os/CondVar.h"

namespace mindroid {

/*
 * Handy class for starting a new Thread that has a Looper and a Handler.
 * See LooperThreadExample.cpp for details.
 * T must be a subclass of class Handler.
 */
template<class T /*extends Handler*/>
class LooperThread : public Thread {
public:
	LooperThread() :
			mLooper(NULL),
			mHandler(NULL),
			mCondVar(),
			mIsDone(false) {
	}

	virtual ~LooperThread() {
	}

	virtual void run() {
		Looper::prepare();
		Lock::lock();
		mLooper = Looper::myLooper();
		mHandler = new (mHandlerData) T();
		mCondVar.notifyAll();
		Lock::unlock();
		Looper::loop();
		Lock::lock();
		mIsDone = true;
		mHandler->removeMessages();
		mLooper = NULL;
		mHandler = NULL;
		Lock::unlock();
	}

	Looper* getLooper() {
		AutoLock autoLock;
		if (!mIsDone && mLooper == NULL) {
			mCondVar.wait();
		}
		return mLooper;
	}

	T* getHandler() {
		AutoLock autoLock;
		if (!mIsDone && mHandler == NULL) {
			mCondVar.wait();
		}
		return mHandler;
	}

private:
	Looper* mLooper;
	uint8_t mHandlerData[sizeof(T)] __attribute__((aligned (8)));
	T* mHandler;
	CondVar mCondVar;
	bool mIsDone;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(LooperThread)
};

} /* namespace mindroid */

#endif /* MYDROID_LOOPERTHREAD_H_ */
