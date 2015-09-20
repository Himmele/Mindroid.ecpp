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

#ifndef MINDROID_MESSAGEQUEUE_H_
#define MINDROID_MESSAGEQUEUE_H_

#include <stdint.h>
#include "mindroid/os/Lock.h"
#include "mindroid/os/CondVar.h"
#include "mindroid/util/Utils.h"

namespace mindroid {

class Message;
class Handler;
class Runnable;

class MessageQueue
{
public:
	MessageQueue();
	~MessageQueue();
	bool enqueueMessage(Message& message, uint64_t execTimestamp);
	Message* dequeueMessage(Message& message);
	bool removeMessages(Handler* handler);
	bool removeMessages(Handler* handler, int32_t what);
	bool removeMessage(Handler* handler, const Message* message);

private:
	bool enqueueMessage(Message& message, uint64_t execTimestamp, bool notify);
	Message* dequeueMessage(Message& message, bool wait);
	Message* getNextMessage(uint64_t now, Message& message);
	void notify();
	void quit();

	Message* mHeadMessage;
	CondVar mCondVar;
	bool mQuiting;

	friend class Looper;
	friend class RunnableQueue;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(MessageQueue)
};

} /* namespace mindroid */

#endif /* MINDROID_MESSAGEQUEUE_H_ */
