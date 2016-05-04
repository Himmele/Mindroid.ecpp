/*
 * Copyright (C) 2016 Daniel Himmelein
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

#ifndef MINDROID_ATOMICINTEGER_H_
#define MINDROID_ATOMICINTEGER_H_

#include <stdint.h>
#include "mindroid/lang/Object.h"
#include "mindroid/util/concurrent/locks/Lock.h"

namespace mindroid {

class AtomicShort {
public:
	AtomicShort() : mValue(0) {
	}
	AtomicShort(uint16_t initialValue) : mValue(initialValue) {
	}

	inline uint16_t get() const {
		return mValue;
	}

	inline uint16_t unsafeGet() const {
		return mValue;
	}

	inline void set(uint16_t newValue) {
		mValue = newValue;
	}

private:
	volatile uint16_t mValue;

	NO_COPY_CONSTRUCTOR_AND_ASSIGNMENT_OPERATOR(AtomicShort)
};

class AtomicInteger {
public:
	AtomicInteger() : mValue(0) {
	}
	AtomicInteger(uint32_t initialValue) : mValue(initialValue) {
	}

	inline uint32_t get() const {
		return mValue;
	}

	inline uint32_t unsafeGet() const {
		return mValue;
	}

	inline void set(uint32_t newValue) {
		mValue = newValue;
	}

private:
	volatile uint32_t mValue;

	NO_COPY_CONSTRUCTOR_AND_ASSIGNMENT_OPERATOR(AtomicInteger)
};

} /* namespace mindroid */

#endif /* MINDROID_ATOMICINTEGER_H_ */
