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

#include <pthread.h>
#include "mindroid/os/MessageQueue.h"
#include "mindroid/os/Message.h"
#include "mindroid/os/Clock.h"
#include "mindroid/os/Lock.h"
#include "mindroid/os/Runnable.h"

namespace mindroid {

MessageQueue::MessageQueue() :
	mHeadMessage(NULL),
	mCondVar(mCondVarLock),
	mLockMessageQueue(false) {
}

MessageQueue::~MessageQueue() {
}

bool MessageQueue::enqueueMessage(Message& message, uint64_t execTimestamp) {
	AutoLock autoLock(mCondVarLock);
	if (!message.ready()) {
		return false;
	}
	if (mLockMessageQueue) {
		return false;
	} else if (message.mHandler == NULL) {
		mLockMessageQueue = true;
	}
	message.setExecTimestamp(execTimestamp);
	Message* curMessage = mHeadMessage;
	if (curMessage == NULL || execTimestamp == 0 || execTimestamp < curMessage->mExecTimestamp) {
		message.mNextMessage = curMessage;
		mHeadMessage = &message;
		mCondVar.notify();
	} else {
		Message* prevMessage = NULL;
		while (curMessage != NULL && curMessage->mExecTimestamp <= execTimestamp) {
			prevMessage = curMessage;
			curMessage = curMessage->mNextMessage;
		}
		message.mNextMessage = prevMessage->mNextMessage;
		prevMessage->mNextMessage = &message;
		mCondVar.notify();
	}
	return true;
}

Message& MessageQueue::dequeueMessage() {
	while (true) {
		AutoLock autoLock(mCondVarLock);
		uint64_t now = Clock::monotonicTime();
		Message* message = getNextMessage(now);
		if (message != NULL) {
			return *message;
		}

		if (mHeadMessage != NULL) {
			if (mHeadMessage->mExecTimestamp - now > 0) {
				timespec absExecTimestamp;
				absExecTimestamp.tv_sec = mHeadMessage->mExecTimestamp / 1000000000LL;
				absExecTimestamp.tv_nsec = mHeadMessage->mExecTimestamp % 1000000000LL;
				mCondVar.wait(absExecTimestamp);
			}
		} else {
			mCondVar.wait();
		}
	}
}

Message* MessageQueue::getNextMessage(uint64_t now) {
	Message* message = mHeadMessage;
	if (message != NULL) {
		if (now >= message->mExecTimestamp) {
			mHeadMessage = message->mNextMessage;
			return message;
		}
	}
	return NULL;
}

bool MessageQueue::removeMessages(Handler* handler, int32_t what) {
	if (handler == NULL) {
		return false;
	}

	bool foundMessage = false;

	mCondVarLock.lock();

	Message* curMessage = mHeadMessage;
	// remove all matching messages at the front of the message queue.
	while (curMessage != NULL && curMessage->mHandler == handler && curMessage->what == what) {
		foundMessage = true;
		Message* nextMessage = curMessage->mNextMessage;
		mHeadMessage = nextMessage;
		curMessage->setExecTimestamp(0);
		curMessage = nextMessage;
	}

	// remove all matching messages after the front of the message queue.
	while (curMessage != NULL) {
		Message* nextMessage = curMessage->mNextMessage;
		if (nextMessage != NULL) {
			if (nextMessage->mHandler == handler && nextMessage->what == what) {
				foundMessage = true;
				Message* nextButOneMessage = nextMessage->mNextMessage;
				nextMessage->setExecTimestamp(0);
				curMessage->mNextMessage = nextButOneMessage;
				continue;
			}
		}
		curMessage = nextMessage;
	}

	mCondVarLock.unlock();

	return foundMessage;
}

bool MessageQueue::removeCallbacks(Handler* handler, Runnable* runnable) {
	if (handler == NULL || runnable == NULL) {
		return false;
	}

	bool foundRunnable = false;

	mCondVarLock.lock();

	Message* curMessage = mHeadMessage;
	// remove all matching messages at the front of the message queue.
	while (curMessage != NULL && curMessage->mHandler == handler && curMessage->mCallback == runnable) {
		foundRunnable = true;
		Message* nextMessage = curMessage->mNextMessage;
		mHeadMessage = nextMessage;
		curMessage->setExecTimestamp(0);
		curMessage = nextMessage;
	}

	// remove all matching messages after the front of the message queue.
	while (curMessage != NULL) {
		Message* nextMessage = curMessage->mNextMessage;
		if (nextMessage != NULL) {
			if (nextMessage->mHandler == handler && nextMessage->mCallback == runnable) {
				foundRunnable = true;
				Message* nextButOneMessage = nextMessage->mNextMessage;
				nextMessage->setExecTimestamp(0);
				curMessage->mNextMessage = nextButOneMessage;
				continue;
			}
		}
		curMessage = nextMessage;
	}

	mCondVarLock.unlock();

	return foundRunnable;
}

bool MessageQueue::removeCallbacksAndMessages(Handler* handler) {
	if (handler == NULL) {
		return false;
	}

	bool foundSomething = false;

	mCondVarLock.lock();

	Message* curMessage = mHeadMessage;
	// remove all matching messages at the front of the message queue.
	while (curMessage != NULL && curMessage->mHandler == handler) {
		foundSomething = true;
		Message* nextMessage = curMessage->mNextMessage;
		mHeadMessage = nextMessage;
		curMessage->setExecTimestamp(0);
		curMessage = nextMessage;
	}

	// remove all matching messages after the front of the message queue.
	while (curMessage != NULL) {
		Message* nextMessage = curMessage->mNextMessage;
		if (nextMessage != NULL) {
			if (nextMessage->mHandler == handler) {
				foundSomething = true;
				Message* nextButOneMessage = nextMessage->mNextMessage;
				nextMessage->setExecTimestamp(0);
				curMessage->mNextMessage = nextButOneMessage;
				continue;
			}
		}
		curMessage = nextMessage;
	}

	mCondVarLock.unlock();

	return foundSomething;
}

} /* namespace mindroid */
