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

#include <stddef.h>
#include <assert.h>
#include <new>
#include "mindroid/os/Message.h"
#include "mindroid/os/Handler.h"
#include "mindroid/os/Lock.h"

namespace mindroid {

Message::Message() :
		what(0),
		arg1(0),
		arg2(0),
		obj(NULL),
		mExecTimestamp(0),
		mHandler(NULL),
		mCallback(NULL),
		mNextMessage(NULL) {
}

Message::Message(bool quitMessage) :
		what(0),
		arg1(0),
		arg2(0),
		obj(NULL),
		mExecTimestamp(0),
		mHandler(NULL),
		mCallback(NULL),
		mNextMessage(NULL) {
}

Message::Message(Handler& handler) :
		what(0),
		arg1(0),
		arg2(0),
		obj(NULL),
		mExecTimestamp(0),
		mHandler(&handler),
		mCallback(NULL),
		mNextMessage(NULL) {
}

Message::Message(Handler& handler, int32_t what) :
		what(what),
		arg1(0),
		arg2(0),
		obj(NULL),
		mExecTimestamp(0),
		mHandler(&handler),
		mCallback(NULL),
		mNextMessage(NULL) {
}

Message::Message(Handler& handler, int32_t what, int32_t arg1, int32_t arg2) :
		what(what),
		arg1(arg1),
		arg2(arg2),
		obj(NULL),
		mExecTimestamp(0),
		mHandler(&handler),
		mCallback(NULL),
		mNextMessage(NULL) {
}

Message::Message(Handler& handler, Runnable& callback) :
		what(what),
		arg1(0),
		arg2(0),
		obj(NULL),
		mExecTimestamp(0),
		mHandler(&handler),
		mCallback(&callback),
		mNextMessage(NULL) {
}

bool Message::obtain(Message& message, Handler& handler) {
	if (message.ready()) {
		new (&message) Message(handler);
		return true;
	} else {
		return false;
	}
}

bool Message::obtain(Message& message, Handler& handler, int32_t what) {
	if (message.ready()) {
		new (&message) Message(handler, what);
		return true;
	} else {
		return false;
	}
}

bool Message::obtain(Message& message, Handler& handler, int32_t what, int32_t arg1, int32_t arg2) {
	if (message.ready()) {
		new (&message) Message(handler, what, arg1, arg2);
		return true;
	} else {
		return false;
	}
}

bool Message::obtain(Message& message, Handler& handler, Runnable& callback) {
	if (message.ready()) {
		new (&message) Message(handler, callback);
		return true;
	} else {
		return false;
	}
}

void Message::clear() {
	what = 0;
	arg1 = 0;
	arg2 = 0;
	obj = NULL;
	setExecTimestamp(0);
	mHandler = NULL;
	mCallback = NULL;
	mNextMessage = NULL;
}

bool Message::sendToTarget() {
	if (mHandler != NULL) {
		return mHandler->sendMessage(*this);
	} else {
		return false;
	}
}

} /* namespace mindroid */
