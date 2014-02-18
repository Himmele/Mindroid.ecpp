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

#include "mindroid/os/Looper.h"
#include "mindroid/os/Handler.h"
#include "mindroid/os/Message.h"
#include "mindroid/util/Assert.h"
#include <new>

namespace mindroid {

pthread_once_t Looper::sTlsOneTimeInitializer = PTHREAD_ONCE_INIT;
pthread_key_t Looper::sTlsKey;
uint8_t Looper::sLooperHeapMemory[MAX_NUM_LOOPERS * sizeof(Looper)];
Looper* Looper::sLoopers[] = { 0L };
int Looper::sNumLoopers = 0;
Lock Looper::sLock;

Looper::Looper() :
		mMessageQueue(),
		mRunnableQueue(*this) {
}

void Looper::init() {
	pthread_key_create(&sTlsKey, Looper::finalize);
}

void Looper::finalize(void* looper) {
}

bool Looper::prepare() {
	pthread_once(&sTlsOneTimeInitializer, Looper::init);
	Looper* looper = (Looper*) pthread_getspecific(sTlsKey);
	if (looper == NULL) {
		AutoLock autoLock(sLock);
		Assert::assertTrue(sNumLoopers < MAX_NUM_LOOPERS);
		int i = sNumLoopers;
		Looper* looper = reinterpret_cast<Looper*>(sLooperHeapMemory + i * sizeof(Looper));
		new (looper) Looper();
		if (looper == NULL) {
			return false;
		} else {
			if (pthread_setspecific(sTlsKey, looper) != 0) {
				return false;
			} else {
				sLoopers[i] = looper;
				sNumLoopers++;
				return true;
			}
		}
	} else {
		// There should be only one Looper per thread.
		return false;
	}
}

Looper* Looper::myLooper() {
	pthread_once(&sTlsOneTimeInitializer, Looper::init);
	Looper* looper = (Looper*) pthread_getspecific(sTlsKey);
	return looper;
}

void Looper::loop() {
	Looper* me = myLooper();
	if (me != NULL) {
		MessageQueue& mq = me->mMessageQueue;
		RunnableQueue& rq = me->mRunnableQueue;
		while (true) {
			Message* message = mq.dequeueMessage(me->mMessage);
			if (message == NULL) {
				return;
			}
			Handler* handler = message->mHandler;
			message->mHandler = NULL;
			handler->dispatchMessage(*message);
		}
	}
}

void Looper::quit() {
	mMessageQueue.quit();
}

} /* namespace mindroid */
