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
#include "mindroid/os/CondVar.h"
#include "mindroid/os/Lock.h"
#include "mindroid/os/Clock.h"

namespace mindroid {

CondVar::CondVar() {
	pthread_condattr_init(&mCondVarAttributes);
	pthread_condattr_setclock(&mCondVarAttributes, CLOCK_MONOTONIC);
	pthread_cond_init(&mCondVar, &mCondVarAttributes);
}

CondVar::~CondVar() {
	pthread_cond_destroy(&mCondVar);
	pthread_condattr_destroy(&mCondVarAttributes);
}

void CondVar::wait() {
	pthread_cond_wait(&mCondVar, Lock::getLock());
}

void CondVar::wait(uint32_t timeout) {
	uint64_t timeoutTimestamp = (Clock::monotonicTime() + timeout) * 1000000LL;
	timespec absTimestamp;
	absTimestamp.tv_sec = timeoutTimestamp / 1000000000LL;
	absTimestamp.tv_nsec = timeoutTimestamp % 1000000000LL;
	wait(absTimestamp);
}

void CondVar::wait(timespec& absTimestamp) {
	pthread_cond_timedwait(&mCondVar, Lock::getLock());
}

void CondVar::notify() {
	pthread_cond_signal(&mCondVar);
}

void CondVar::notifyAll() {
	pthread_cond_broadcast(&mCondVar);
}

} /* namespace mindroid */
