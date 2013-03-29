/*
 * Copyright (C) 2013 Daniel Himmelein
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
#include "mindroid/util/Utils.h"
#include "mindroid/os/Lock.h"

namespace mindroid {

template<typename INT>
class AtomicInteger
{
public:
	AtomicInteger();
	AtomicInteger(INT initialValue);
	INT	get();
	void set(INT newValue);

private:
	INT mValue;
	Lock mLock;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(AtomicInteger)
};

template<typename INT>
AtomicInteger<INT>::AtomicInteger() {
	AutoLock autoLock(mLock);
	mValue = 0;
}

template<typename INT>
AtomicInteger<INT>::AtomicInteger(INT initialValue) {
	AutoLock autoLock(mLock);
	mValue = initialValue;
}

template<typename INT>
INT	AtomicInteger<INT>::get() {
	AutoLock autoLock(mLock);
	return mValue;
}

template<typename INT>
void AtomicInteger<INT>::set(INT newValue) {
	AutoLock autoLock(mLock);
	mValue = newValue;
}

} /* namespace mindroid */

#endif /* MINDROID_ATOMICINTEGER_H_ */
