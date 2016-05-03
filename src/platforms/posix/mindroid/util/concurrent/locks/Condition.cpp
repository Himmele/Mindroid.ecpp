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

#include <stdlib.h>
#include "mindroid/util/concurrent/locks/Lock.h"
#include "mindroid/util/concurrent/locks/Condition.h"
#include "mindroid/os/SystemClock.h"

namespace mindroid {

Condition::Condition() {
	pthread_condattr_init(&mConditionAttributes);
	pthread_condattr_setclock(&mConditionAttributes, CLOCK_MONOTONIC);
	pthread_cond_init(&mCondition, &mConditionAttributes);
}

Condition::~Condition() {
	pthread_cond_destroy(&mCondition);
	pthread_condattr_destroy(&mConditionAttributes);
}

void Condition::await() {
	pthread_cond_wait(&mCondition, Lock::getLock());
}

void Condition::await(uint32_t timeout) {
	uint64_t timeoutTimestamp = (SystemClock::monotonicTime() + timeout) * 1000000LL;
	timespec absTimestamp;
	absTimestamp.tv_sec = timeoutTimestamp / 1000000000LL;
	absTimestamp.tv_nsec = timeoutTimestamp % 1000000000LL;
	await(absTimestamp);
}

void Condition::await(timespec& absTimestamp) {
	pthread_cond_timedwait(&mCondition, Lock::getLock(), &absTimestamp);
}

void Condition::signal() {
	pthread_cond_signal(&mCondition);
}

void Condition::signalAll() {
	pthread_cond_broadcast(&mCondition);
}

} /* namespace mindroid */
