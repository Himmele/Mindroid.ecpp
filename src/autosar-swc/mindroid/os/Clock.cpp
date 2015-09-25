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

#include "mindroid/os/Clock.h"
#include "mindroid/os/Lock.h"

namespace mindroid {

uint32_t Clock::sNow = 0;

uint64_t Clock::monotonicTime() {
	static uint32_t base = 0;
	static uint32_t offset = 0;

	Lock::lock();
	uint32_t now = sNow;
	if (now == 0 && base == 0) {
		now = 1;
	}
	if (offset > now) {
		base++;
	}
	offset = now;
	uint64_t time = ((uint64_t) base << 32) | offset;
	Lock::unlock();
	return time;
}

uint64_t Clock::realTime() {
	return monotonicTime();
}

void Clock::tick(uint32_t value) {
	sNow += value;
}

} /* namespace mindroid */
