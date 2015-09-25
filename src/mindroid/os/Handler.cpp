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

#include <stddef.h>
#include "mindroid/os/Handler.h"
#include "mindroid/os/Message.h"
#include "mindroid/os/MessageQueue.h"
#include "mindroid/os/RunnableQueue.h"
#include "mindroid/os/Looper.h"
#include "mindroid/os/Clock.h"
#include "mindroid/util/Assert.h"

namespace mindroid {

Handler::Handler() {
	Looper* looper = Looper::myLooper();
	Assert::assertNotNull(looper);
	mMessageQueue = &looper->myMessageQueue();
	mRunnableQueue = &looper->myRunnableQueue();
}

Handler::Handler(Looper& looper) {
	mMessageQueue = &looper.myMessageQueue();
	mRunnableQueue = &looper.myRunnableQueue();
}

Handler::~Handler() {
}

void Handler::handleMessage(const Message& message) {
}

bool Handler::sendMessage(Message& message) {
	return sendMessageAtTime(message, Clock::monotonicTime());
}

bool Handler::sendMessageDelayed(Message& message, uint32_t delay) {
	return sendMessageAtTime(message, Clock::monotonicTime() + delay);
}

bool Handler::sendMessageAtTime(Message& message, uint64_t execTimestamp) {
	if (message.getHandler() == this) {
		return mMessageQueue->enqueueMessage(message, execTimestamp);
	} else {
		return false;
	}
}

bool Handler::post(Runnable& runnable) {
	return postAtTime(runnable, Clock::monotonicTime());
}

bool Handler::postDelayed(Runnable& runnable, uint32_t delay) {
	return postAtTime(runnable, Clock::monotonicTime() + delay);
}

bool Handler::postAtTime(Runnable& runnable, uint64_t execTimestamp) {
	return mRunnableQueue->enqueueRunnable(runnable, execTimestamp);
}

bool Handler::removeMessages() {
	return mMessageQueue->removeMessages(this);
}

bool Handler::removeMessages(int32_t what) {
	return mMessageQueue->removeMessages(this, what);
}

bool Handler::removeMessage(const Message& message) {
	return mMessageQueue->removeMessage(this, &message);
}

bool Handler::removeCallbacks(const Runnable& runnable) {
	return mRunnableQueue->removeRunnable(&runnable);
}

} /* namespace mindroid */
