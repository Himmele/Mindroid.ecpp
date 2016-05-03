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
#include <new>
#include "mindroid/os/Message.h"
#include "mindroid/os/Handler.h"

namespace mindroid {

Message::Message() :
		what(0),
		arg1(0),
		arg2(0),
		obj(NULL),
		when(0xFFFFFFFFFFFFFFFF),
		target(NULL),
		nextMessage(NULL) {
}

Message::Message(Handler& handler) :
		what(0),
		arg1(0),
		arg2(0),
		obj(NULL),
		when(0xFFFFFFFFFFFFFFFF),
		target(&handler),
		nextMessage(NULL) {
}

Message::Message(Handler& handler, const int32_t what) :
		what(what),
		arg1(0),
		arg2(0),
		obj(NULL),
		when(0xFFFFFFFFFFFFFFFF),
		target(&handler),
		nextMessage(NULL) {
}

Message::Message(Handler& handler, const int32_t what, const int32_t arg1, const int32_t arg2) :
		what(what),
		arg1(arg1),
		arg2(arg2),
		obj(NULL),
		when(0xFFFFFFFFFFFFFFFF),
		target(&handler),
		nextMessage(NULL) {
}

Message::Message(Handler& handler, const int32_t what, void* const obj) :
		what(what),
		arg1(0),
		arg2(0),
		obj(obj),
		when(0xFFFFFFFFFFFFFFFF),
		target(&handler),
		nextMessage(NULL) {
}

Message& Message::operator=(const Message& message) {
	what = message.what;
	arg1 = message.arg1;
	arg2 = message.arg2;
	obj = message.obj;
	when = 0xFFFFFFFFFFFFFFFF;
	target = message.target;
	nextMessage = NULL;
	return *this;
}

Message* Message::obtain(Message& message, Handler& handler) {
	AutoLock autoLock;
	if (!message.isInUse()) {
		new (&message) Message(handler);
		return &message;
	} else {
		return NULL;
	}
}

Message* Message::obtain(Message& message, Handler& handler, const int32_t what) {
	AutoLock autoLock;
	if (!message.isInUse()) {
		new (&message) Message(handler, what);
		return &message;
	} else {
		return NULL;
	}
}

Message* Message::obtain(Message& message, Handler& handler, const int32_t what, const int32_t arg1, const int32_t arg2) {
	AutoLock autoLock;
	if (!message.isInUse()) {
		new (&message) Message(handler, what, arg1, arg2);
		return &message;
	} else {
		return NULL;
	}
}

Message* Message::obtain(Message& message, Handler& handler, const int32_t what, void* const obj) {
	AutoLock autoLock;
	if (!message.isInUse()) {
		new (&message) Message(handler, what, obj);
		return &message;
	} else {
		return NULL;
	}
}

void Message::clear() {
	AutoLock autoLock;
	what = 0;
	arg1 = 0;
	arg2 = 0;
	obj = NULL;
	when = 0xFFFFFFFFFFFFFFFF;
	target = NULL;
	nextMessage = NULL;
}

bool Message::sendToTarget() {
	if (target != NULL) {
		return target->sendMessage(*this);
	} else {
		return false;
	}
}

} /* namespace mindroid */
