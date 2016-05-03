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
#include "mindroid/util/concurrent/locks/Lock.h"
#include "mindroid/util/concurrent/locks/Condition.h"
#include "mindroid/lang/Object.h"

namespace mindroid {

class Message;
class Handler;
class Runnable;

class MessageQueue {
public:
	MessageQueue();
	~MessageQueue();
	bool enqueueMessage(Message& message, uint64_t when);
	Message* dequeueMessage(Message& message);
	bool removeMessages(Handler* handler);
	bool removeMessages(Handler* handler, int32_t what);
	bool removeMessage(Handler* handler, const Message* message);

private:
	bool enqueueMessage(Message& message, uint64_t when, bool signal);
	Message* dequeueMessage(Message& message, bool wait);
	Message* getNextMessage(uint64_t now, Message& message);
	void signal();
	void quit();

	Message* mHeadMessage;
	Condition mCondition;
	bool mQuitting;

	friend class Looper;
	friend class RunnableQueue;

	NO_COPY_CONSTRUCTOR_AND_ASSIGNMENT_OPERATOR(MessageQueue)
};

} /* namespace mindroid */

#endif /* MINDROID_MESSAGEQUEUE_H_ */
