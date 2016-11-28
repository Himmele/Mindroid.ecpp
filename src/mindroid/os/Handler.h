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
#include "mindroid/lang/Runnable.h"
#include "mindroid/lang/Object.h"
#include "mindroid/os/Message.h"
#include "mindroid/util/Assert.h"

namespace mindroid {

class Lock;
class Looper;
class MessageQueue;
class RunnableQueue;

class Handler {
public:
    Handler();
    Handler(Looper& looper);
    virtual ~Handler() {
    }

    Message* obtainMessage(Message& message) {
        return Message::obtain(message, *this);
    }

    Message* obtainMessage(Message& message, const int32_t what) {
        return Message::obtain(message, *this, what);
    }

    Message* obtainMessage(Message& message, const int32_t what, const int32_t arg1, const int32_t arg2) {
        return Message::obtain(message, *this, what, arg1, arg2);
    }

    Message* obtainMessage(Message& message, const int32_t what, void* const obj) {
        return Message::obtain(message, *this, what, obj);
    }

    Message& grabMessage(Message& message) {
        AutoLock autoLock;
        removeMessage(message);
        Assert::assertNotNull(Message::obtain(message, *this));
        return message;
    }

    Message& grabMessage(Message& message, const int32_t what) {
        AutoLock autoLock;
        removeMessage(message);
        Assert::assertNotNull(Message::obtain(message, *this, what));
        return message;
    }

    Message& grabMessage(Message& message, const int32_t what, const int32_t arg1, const int32_t arg2) {
        AutoLock autoLock;
        removeMessage(message);
        Assert::assertNotNull(Message::obtain(message, *this, what, arg1, arg2));
        return message;
    }

    Message& grabMessage(Message& message, const int32_t what, void* const obj) {
        AutoLock autoLock;
        removeMessage(message);
        Assert::assertNotNull(Message::obtain(message, *this, what, obj));
        return message;
    }

    void dispatchMessage(Message& message) {
        handleMessage(message);
    }

    virtual void handleMessage(const Message& message);
    bool sendMessage(Message& message);
    bool sendMessageDelayed(Message& message, uint32_t delay);
    bool sendMessageAtTime(Message& message, uint64_t uptimeMillis);
    bool post(Runnable& runnable);
    bool postDelayed(Runnable& runnable, uint32_t delay);
    bool postAtTime(Runnable& runnable, uint64_t uptimeMillis);
    bool removeMessages();
    bool removeMessages(int32_t what);
    bool removeMessage(const Message& message);
    bool removeCallbacks(const Runnable& runnable);

private:
    MessageQueue* mMessageQueue;
    RunnableQueue* mRunnableQueue;

    NO_COPY_CONSTRUCTOR_AND_ASSIGNMENT_OPERATOR(Handler)
};

} /* namespace mindroid */

#endif /* MINDROID_HANDLER_H_ */
