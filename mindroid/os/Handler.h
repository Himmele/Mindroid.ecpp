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
#include "mindroid/os/Message.h"
#include "mindroid/os/Runnable.h"
#include "mindroid/util/Utils.h"

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

	bool obtainMessage(Message& message, int16_t what) {
		return Message::obtain(message, *this, what);
	}

	bool obtainMessage(Message& message, int16_t what, int32_t arg1, int32_t arg2) {
		return Message::obtain(message, *this, what, arg1, arg2);
	}

	bool obtainMessage(Message& message, int16_t what, void* obj) {
		return Message::obtain(message, *this, what, obj);
	}

	void dispatchMessage(Message& message) {
		handleMessage(message);
	}

	virtual void handleMessage(const Message& message);
	bool sendMessage(Message& message);
	bool sendMessageDelayed(Message& message, uint32_t delay);
	bool sendMessageAtTime(Message& message, uint64_t execTimestamp);
	bool removeMessages();
	bool removeMessage(Message& message);
	bool removeMessages(int16_t what);

private:
	MessageQueue* mMessageQueue;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Handler)
};

} /* namespace mindroid */

#endif /* MINDROID_HANDLER_H_ */
