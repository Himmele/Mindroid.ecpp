/*
 * Copyright (C) 2014 Daniel Himmelein
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

#include "mindroid/lang/Runnable.h"
#include "mindroid/os/RunnableQueue.h"
#include "mindroid/os/Looper.h"
#include "mindroid/os/SystemClock.h"
#include "mindroid/util/concurrent/locks/Lock.h"

namespace mindroid {

RunnableQueue::RunnableQueue(Looper& looper) :
		Handler(looper),
		mMessageQueue(looper.myMessageQueue()),
		mMessage(*this, MSG_RUNNABLE) {
}

RunnableQueue::~RunnableQueue() {
}

bool RunnableQueue::enqueueRunnable(Runnable& runnable, uint64_t when) {
	bool signal = false;
	{
		AutoLock autoLock;
		if (runnable.isInUse()) {
			return false;
		}
		runnable.markInUse();
		runnable.when = when;
		Runnable* curRunnable = (Runnable*) mMessage.obj;
		if (curRunnable == NULL || when < curRunnable->when) {
			runnable.nextRunnable = curRunnable;
			mMessage.obj = &runnable;

			mMessageQueue.removeMessage(this, &mMessage);
			mMessageQueue.enqueueMessage(mMessage, when, false);
			signal = true;
		} else {
			Runnable* prevRunnable = NULL;
			while (curRunnable != NULL && curRunnable->when <= when) {
				prevRunnable = curRunnable;
				curRunnable = curRunnable->nextRunnable;
			}
			runnable.nextRunnable = prevRunnable->nextRunnable;
			prevRunnable->nextRunnable = &runnable;
		}
	}
	if (signal) {
		mMessageQueue.signal();
	}
	return true;
}

Runnable* RunnableQueue::dequeueRunnable() {
	Runnable* runnable = NULL;
	uint64_t now = SystemClock::monotonicTime();
	bool signal = false;

	{
		AutoLock autoLock;
		Runnable* nextRunnable = (Runnable*) mMessage.obj;
		if (nextRunnable != NULL) {
			if (now >= nextRunnable->when) {
				mMessage.obj = nextRunnable->nextRunnable;
				runnable = nextRunnable;
				runnable->recycle();
			}

			Runnable* headRunnable = (Runnable*) mMessage.obj;
			if (headRunnable != NULL) {
				mMessageQueue.enqueueMessage(mMessage, headRunnable->when, false);
				signal = true;
			}
		}
	}
	if (signal) {
		mMessageQueue.signal();
	}
	return runnable;
}

bool RunnableQueue::removeRunnable(const Runnable* runnable) {
	if (runnable == NULL) {
		return false;
	}

	bool foundRunnable = false;
	bool signal = false;

	{
		AutoLock autoLock;
		Runnable* curRunnable = (Runnable*) mMessage.obj;
		// Remove a matching runnable at the front of the runnable queue.
		if (curRunnable != NULL && curRunnable == runnable) {
			foundRunnable = true;
			Runnable* nextRunnable = curRunnable->nextRunnable;
			mMessage.obj = nextRunnable;
			curRunnable->recycle();
			curRunnable = nextRunnable;

			Runnable* headRunnable = (Runnable*) mMessage.obj;
			if (headRunnable != NULL) {
				mMessageQueue.removeMessage(this, &mMessage);
				mMessageQueue.enqueueMessage(mMessage, headRunnable->when, false);
				signal = true;
			}
		}

		// Remove a matching runnable after the front of the runnable queue.
		if (!foundRunnable) {
			while (curRunnable != NULL) {
				Runnable* nextRunnable = curRunnable->nextRunnable;
				if (nextRunnable != NULL) {
					if (nextRunnable == runnable) {
						foundRunnable = true;
						Runnable* nextButOneRunnable = nextRunnable->nextRunnable;
						nextRunnable->recycle();
						curRunnable->nextRunnable = nextButOneRunnable;
						break;
					}
				}
				curRunnable = nextRunnable;
			}
		}
	}
	if (signal) {
		mMessageQueue.signal();
	}

	return foundRunnable;
}

void RunnableQueue::handleMessage(const Message& message) {
	if (message.what == MSG_RUNNABLE) {
		Runnable* runnable = dequeueRunnable();
		if (runnable != NULL) {
			runnable->run();
		}
	}
}

} /* namespace mindroid */
