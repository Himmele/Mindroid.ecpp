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

#ifndef MINDROID_MESSAGE_H_
#define MINDROID_MESSAGE_H_

#include <stdint.h>
#include "mindroid/os/Lock.h"

namespace mindroid {

class Runnable;
class Handler;

class Message {
public:
    Message();
	Message(Handler& handler);
	Message(Handler& handler, const int32_t what);
	Message(Handler& handler, const int32_t what, const int32_t arg1, const int32_t arg2);
	Message(Handler& handler, const int32_t what, void* const obj);

	Message& operator=(const Message& message);

    static Message* obtain(Message& message, Handler& handler);
    static Message* obtain(Message& message, Handler& handler, const int32_t what);
    static Message* obtain(Message& message, Handler& handler, const int32_t what, const int32_t arg1, const int32_t arg2);
    static Message* obtain(Message& message, Handler& handler, const int32_t what, void* const obj);

    Handler* getHandler() const {
    	return mHandler;
    }

    bool sendToTarget();

    int32_t what;
    int32_t arg1;
    int32_t arg2;
    void* obj;

private:
    inline uint64_t getExecTimestamp() {
		AutoLock autoLock(mLock);
		return mExecTimestamp;
	}

    inline void recycle() {
    	AutoLock autoLock(mLock);
    	mExecTimestamp = 0;
    	mNextMessage = NULL;
    }

    void clear();

    uint64_t mExecTimestamp; // nanoseconds
    Handler* mHandler;
    Message* mNextMessage;
    mutable Lock mLock;

    friend class MessageQueue;
    friend class RunnableQueue;
    friend class Looper;
};

} /* namespace mindroid */

#endif /* MINDROID_MESSAGE_H_ */
