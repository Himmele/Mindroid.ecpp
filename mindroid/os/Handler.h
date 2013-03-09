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

#ifndef MINDROID_HANDLER_H_
#define MINDROID_HANDLER_H_

#include <stdint.h>
#include "mindroid/os/Utils.h"
#include "mindroid/os/Message.h"
#include "mindroid/os/Runnable.h"

namespace mindroid {

class Looper;
class MessageQueue;
class Lock;

class Handler
{
public:
	Handler();
	Handler(Looper& looper);
	virtual ~Handler();

	bool obtainMessage(Message& message) {
		return Message::obtain(message, *this);
	}

	bool obtainMessage(Message& message, int32_t what) {
		return Message::obtain(message, *this, what);
	}

	bool obtainMessage(Message& message, int32_t what, int32_t arg1, int32_t arg2) {
		return Message::obtain(message, *this, what, arg1, arg2);
	}

	bool obtainMessage(Message& message, Runnable& callback) {
		return Message::obtain(message, *this, callback);
	}

	void dispatchMessage(Message& message);
	virtual void handleMessage(Message& message);
	bool sendMessage(Message& message);
	bool sendMessageDelayed(Message& message, uint32_t delay);
	bool sendMessageAtTime(Message& message, uint64_t execTimestamp);
	bool post(Runnable& runnable);
	bool postDelayed(Runnable& runnable, uint32_t delay);
	bool postAtTime(Runnable& runnable, uint64_t execTimestamp);
	bool removeMessages(int32_t what);
	bool removeCallbacks(Runnable& runnable);
	bool removeCallbacksAndMessages();

private:
	void handleCallback(Message& message) {
		message.getCallback()->run();
    }

	MessageQueue* mMessageQueue;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Handler)
};

} /* namespace mindroid */

#endif /* MINDROID_HANDLER_H_ */
