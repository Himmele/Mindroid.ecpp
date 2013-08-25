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
	Message(Handler& handler, int16_t what);
	Message(Handler& handler, int16_t what, int32_t arg1, int32_t arg2);
	Message(Handler& handler, int16_t what, void* obj);

	virtual ~Message() {}

    static bool obtain(Message& message, Handler& handler);
    static bool obtain(Message& message, Handler& handler, int16_t what);
    static bool obtain(Message& message, Handler& handler, int16_t what, int32_t arg1, int32_t arg2);
    static bool obtain(Message& message, Handler& handler, int16_t what, void* obj);

    Handler* getHandler() const {
    	return mHandler;
    }

    bool sendToTarget();

    int16_t what;
    int32_t arg1;
    int32_t arg2;
    void* obj;

protected:
    Message(const Message& message);

	void recycle();
    void clear();

private:
    inline uint64_t getExecTimestamp() {
    	AutoLock autoLock(mLock);
    	return mExecTimestamp;
    }

    mutable Lock mLock;
    uint64_t mExecTimestamp; // nanoseconds
    Handler* mHandler;
    Message* mNextMessage;

    friend class MessageQueue;
    friend class Looper;
};

} /* namespace mindroid */

#endif /* MINDROID_MESSAGE_H_ */
