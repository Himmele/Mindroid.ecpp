/*
 * Copyright (C) 2013 Daniel Himmelein
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

#include "mindroid/app/Process.h"
#include "mindroid/os/Lock.h"
#include "mindroid/util/Assert.h"
#include <string.h>

namespace mindroid {

Process::Process(Looper& looper) :
		mLooper(looper),
		mMainHandler(*this) {
	memset(mServices, 0, sizeof(mServices));
}

void Process::startService(Service& service) {
	int32_t id = -1;
	bool running;

	{
		AutoLock autoLock;

		for (int32_t i = 0; i < MAX_NUM_SERVICES; i++) {
			if ((id < 0) && (mServices[i].service == NULL)) {
				id = i;
			} else if (mServices[i].service == &service) {
				id = i;
				break;
			}
		}
		Assert::assertTrue(id >= 0);

		if (mServices[id].service == NULL) {
			mServices[id].service = &service;
		}

		running = mServices[id].running;
		if (!running) {
			mServices[id].running = true;
		}
	}

	if (!running) {
		Message& message = mMessages[id];
		if (mMainHandler.obtainMessage(message, MainHandler::START_SERVICE)) {
			message.arg1 = id;
			message.obj = &service;
			mMainHandler.sendMessage(message);
		}
	}
}

void Process::stopService(Service& service) {
	int32_t id = -1;
	bool running;

	{
		AutoLock autoLock;

		for (int32_t i = 0; i < MAX_NUM_SERVICES; i++) {
			if ((id < 0) && (mServices[i].service == NULL)) {
				id = i;
			} else if (mServices[i].service == &service) {
				id = i;
				break;
			}
		}
		Assert::assertTrue((id >= 0) && (mServices[id].service != NULL));

		running = mServices[id].running;
		if (running) {
			mServices[id].running = false;
		}
	}

	if (running) {
		Message& message = mMessages[id];
		if (mMainHandler.obtainMessage(message, MainHandler::STOP_SERVICE)) {
			message.arg1 = id;
			message.obj = &service;
			mMainHandler.sendMessage(message);
		}
	}
}

void Process::MainHandler::handleMessage(const Message& message) {
	switch (message.what) {
	case START_SERVICE: {
		Service* service = (Service*) message.obj;
		service->onCreate();

		int32_t id = message.arg1;
		bool running;
		{
			AutoLock autoLock;
			running = mProcess.mServices[id].running;
		}

		if (!running) {
			Message& msg = mProcess.mMessages[id];
			removeMessage(msg);
			Assert::assertNotNull(obtainMessage(msg, MainHandler::STOP_SERVICE));
			msg.arg1 = id;
			msg.obj = &service;
			sendMessage(msg);
		}
	} break;
	case STOP_SERVICE: {
		Service* service = (Service*) message.obj;
		service->onDestroy();

		int32_t id = message.arg1;
		bool running;
		{
			AutoLock autoLock;
			running = mProcess.mServices[id].running;
		}

		if (running) {
			Message& msg = mProcess.mMessages[id];
			removeMessage(msg);
			Assert::assertNotNull(obtainMessage(msg, MainHandler::START_SERVICE));
			msg.arg1 = id;
			msg.obj = &service;
			sendMessage(msg);
		}
	} break;
	}
}

} /* namespace mindroid */
