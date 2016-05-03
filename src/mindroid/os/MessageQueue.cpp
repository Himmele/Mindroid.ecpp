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

#include "mindroid/os/MessageQueue.h"
#include "mindroid/os/Message.h"
#include "mindroid/os/SystemClock.h"
#include "mindroid/lang/Runnable.h"

namespace mindroid {

MessageQueue::MessageQueue() :
		mHeadMessage(NULL),
		mCondition(),
		mQuitting(false) {
}

MessageQueue::~MessageQueue() {
}

bool MessageQueue::enqueueMessage(Message& message, uint64_t when) {
	return enqueueMessage(message, when, true);
}

bool MessageQueue::enqueueMessage(Message& message, uint64_t when, bool signal) {
	if (message.target == NULL) {
		return false;
	}

	{
		AutoLock autoLock;
		if (mQuitting) {
			return false;
		}
		if (message.isInUse()) {
			return false;
		}
		message.markInUse();
		message.when = when;
		Message* curMessage = mHeadMessage;
		if (curMessage == NULL || when < curMessage->when) {
			message.nextMessage = curMessage;
			mHeadMessage = &message;
		} else {
			Message* prevMessage = NULL;
			while (curMessage != NULL && curMessage->when <= when) {
				prevMessage = curMessage;
				curMessage = curMessage->nextMessage;
			}
			message.nextMessage = prevMessage->nextMessage;
			prevMessage->nextMessage = &message;
		}
	}
	if (signal) {
		mCondition.signal();
	}
	return true;
}

Message* MessageQueue::dequeueMessage(Message& message) {
	return dequeueMessage(message, true);
}

Message* MessageQueue::dequeueMessage(Message& message, bool wait) {
	while (true) {
		uint64_t now = SystemClock::monotonicTime();

		AutoLock autoLock;
		if (mQuitting) {
			return NULL;
		}

		if (getNextMessage(now, message) != NULL) {
			return &message;
		}

		if (wait) {
			if (mHeadMessage != NULL) {
				if (mHeadMessage->when - now > 0) {
					uint32_t timeout = (mHeadMessage->when - now); // milliseconds
					mCondition.await(timeout);
				}
			} else {
				mCondition.await();
			}
		} else {
			return NULL;
		}
	}
}

Message* MessageQueue::getNextMessage(uint64_t now, Message& message) {
	Message* nextMessage = mHeadMessage;
	if (nextMessage != NULL) {
		if (now >= nextMessage->when) {
			mHeadMessage = nextMessage->nextMessage;
			message = *nextMessage;
			nextMessage->recycle();
			return &message;
		}
	}
	return NULL;
}

bool MessageQueue::removeMessages(Handler* handler) {
	if (handler == NULL) {
		return false;
	}

	bool foundMessage = false;

	Lock::lock();

	Message* curMessage = mHeadMessage;
	// Remove all matching messages at the front of the message queue.
	while (curMessage != NULL && curMessage->target == handler) {
		foundMessage = true;
		Message* nextMessage = curMessage->nextMessage;
		mHeadMessage = nextMessage;
		curMessage->recycle();
		curMessage = nextMessage;
	}

	// Remove all matching messages after the front of the message queue.
	while (curMessage != NULL) {
		Message* nextMessage = curMessage->nextMessage;
		if (nextMessage != NULL) {
			if (nextMessage->target == handler) {
				foundMessage = true;
				Message* nextButOneMessage = nextMessage->nextMessage;
				nextMessage->recycle();
				curMessage->nextMessage = nextButOneMessage;
				continue;
			}
		}
		curMessage = nextMessage;
	}

	Lock::unlock();

	return foundMessage;
}

bool MessageQueue::removeMessages(Handler* handler, int32_t what) {
	if (handler == NULL) {
		return false;
	}

	bool foundMessage = false;

	Lock::lock();

	Message* curMessage = mHeadMessage;
	// Remove all matching messages at the front of the message queue.
	while (curMessage != NULL && curMessage->target == handler && curMessage->what == what) {
		foundMessage = true;
		Message* nextMessage = curMessage->nextMessage;
		mHeadMessage = nextMessage;
		curMessage->recycle();
		curMessage = nextMessage;
	}

	// Remove all matching messages after the front of the message queue.
	while (curMessage != NULL) {
		Message* nextMessage = curMessage->nextMessage;
		if (nextMessage != NULL) {
			if (nextMessage->target == handler && nextMessage->what == what) {
				foundMessage = true;
				Message* nextButOneMessage = nextMessage->nextMessage;
				nextMessage->recycle();
				curMessage->nextMessage = nextButOneMessage;
				continue;
			}
		}
		curMessage = nextMessage;
	}

	Lock::unlock();

	return foundMessage;
}

bool MessageQueue::removeMessage(Handler* handler, const Message* message) {
	if (handler == NULL || message == NULL) {
		return false;
	}

	bool foundMessage = false;

	Lock::lock();

	Message* curMessage = mHeadMessage;
	// Remove a matching message at the front of the message queue.
	if (curMessage != NULL && curMessage->target == handler && curMessage == message) {
		foundMessage = true;
		Message* nextMessage = curMessage->nextMessage;
		mHeadMessage = nextMessage;
		curMessage->recycle();
		curMessage = nextMessage;
	}

	// Remove a matching message after the front of the message queue.
	if (!foundMessage) {
		while (curMessage != NULL) {
			Message* nextMessage = curMessage->nextMessage;
			if (nextMessage != NULL) {
				if (nextMessage->target == handler && nextMessage == message) {
					foundMessage = true;
					Message* nextButOneMessage = nextMessage->nextMessage;
					nextMessage->recycle();
					curMessage->nextMessage = nextButOneMessage;
					break;
				}
			}
			curMessage = nextMessage;
		}
	}

	Lock::unlock();

	return foundMessage;
}

void MessageQueue::signal() {
	mCondition.signal();
}

void MessageQueue::quit() {
	AutoLock autoLock;
	if (mQuitting) {
		return;
	}
	mQuitting = true;
}

} /* namespace mindroid */
