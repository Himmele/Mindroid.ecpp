/*
 * Copyright (C) 2010 The Android Open Source Project
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

#ifndef MINDROID_BUFFER_H_
#define MINDROID_BUFFER_H_

#include <stdint.h>
#include "mindroid/lang/Object.h"
#include "mindroid/util/Assert.h"

namespace mindroid {

template<size_t CAPACITY>
class Buffer {
public:
	Buffer() :
			mOffset(0),
			mSize(0) {
	}

	uint8_t* base() {
		return (uint8_t*) mData;
	}

	uint8_t* data() {
		return (uint8_t*) mData + mOffset;
	}

	size_t capacity() const {
		return CAPACITY;
	}

	size_t size() const {
		return mSize;
	}

	size_t offset() const {
		return mOffset;
	}

	void setRange(size_t offset, size_t size) {
		Assert::assertTrue(offset <= CAPACITY);
		Assert::assertTrue(size <= CAPACITY);

		mOffset = offset;
		mSize = size;
	}

	void clear() {
		mOffset = 0;
		mSize = 0;
	}

private:
	uint8_t mData[CAPACITY];
	size_t mOffset;
	size_t mSize;

	NO_COPY_CONSTRUCTOR_AND_ASSIGNMENT_OPERATOR(Buffer)
};

} /* namespace mindroid */

#endif /* MINDROID_BUFFER_H_ */
