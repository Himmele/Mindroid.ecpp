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
	Message(Handler& handler, int32_t what);
	Message(Handler& handler, int32_t what, int32_t arg1, int32_t arg2);
	Message(Handler& handler, Runnable& callback);

	virtual ~Message() {}

    static bool obtain(Message& message, Handler& handler);
    static bool obtain(Message& message, Handler& handler, int32_t what);
    static bool obtain(Message& message, Handler& handler, int32_t what, int32_t arg1, int32_t arg2);
    static bool obtain(Message& message, Handler& handler, Runnable& callback);

    void setHandler(Handler& handler) {
    	mHandler = &handler;
    }

    Handler* getHandler() {
    	return mHandler;
    }

    Runnable* getCallback() {
    	return mCallback;
    }

    bool ready() {
    	AutoLock autoLock(mLock);
    	return mExecTimestamp == 0;
    }

    void setExecTimestamp(uint64_t execTimestamp) {
		AutoLock autoLock(mLock);
		mExecTimestamp = execTimestamp;
	}

    bool sendToTarget();

    int32_t what;
    int32_t arg1;
    int32_t arg2;
    void* obj;

protected:
    Message(bool quitMessage);
    void clear();

private:
    Lock mLock;
    uint64_t mExecTimestamp; // nanoseconds
    Handler* mHandler;
    Runnable* mCallback;
    Message* mNextMessage;

    friend class MessageQueue;
    friend class Looper;
};

} /* namespace mindroid */

#endif /* MINDROID_MESSAGE_H_ */
