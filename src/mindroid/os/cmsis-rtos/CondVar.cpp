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

#include "mindroid/os/CondVar.h"
#include "mindroid/os/Lock.h"
#include "mindroid/os/Clock.h"
#include "mindroid/util/Assert.h"

namespace mindroid {

CondVar::CondVar() {
}

void CondVar::wait() {
	Lock::unlock();
	mSemaphore.wait();
	Lock::lock();
}

void CondVar::wait(uint32_t timeout) {
	Lock::unlock();
	Assert::assertTrue(timeout > 0);
	mSemaphore.wait(timeout);
	Lock::lock();
}

void CondVar::notify() {
	mSemaphore.signal();
}

void CondVar::notifyAll() {
	bool notYetImplemented = true;
	Assert::assertTrue(!notYetImplemented);
}

} /* namespace mindroid */
