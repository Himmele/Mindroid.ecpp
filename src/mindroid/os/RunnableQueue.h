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

#ifndef MINDROID_RUNNABLEQUEUE_H_
#define MINDROID_RUNNABLEQUEUE_H_

#include <stdint.h>
#include "mindroid/os/MessageQueue.h"
#include "mindroid/os/Message.h"
#include "mindroid/os/Handler.h"
#include "mindroid/lang/Object.h"

namespace mindroid {

class MessageQueue;
class Runnable;

class RunnableQueue : public Handler {
public:
	RunnableQueue(Looper& looper);
	virtual ~RunnableQueue();
	bool enqueueRunnable(Runnable& runnable, uint64_t when);
	bool removeRunnable(const Runnable* runnable);
	virtual void handleMessage(const Message& message);

private:
	Runnable* dequeueRunnable();

	MessageQueue& mMessageQueue;
	Message mMessage;

	static const int32_t MSG_RUNNABLE = 1;

	friend class Looper;

	NO_COPY_CONSTRUCTOR_AND_ASSIGNMENT_OPERATOR(RunnableQueue)
};

} /* namespace mindroid */

#endif /* MINDROID_RUNNABLEQUEUE_H_ */
