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

#ifndef MINDROID_CIRCULARBUFFER_H_
#define MINDROID_CIRCULARBUFFER_H_

#include <stdint.h>
#include <string.h>
#include "mindroid/util/Utils.h"

namespace mindroid {

template<uint16_t SIZE>
class CircularBuffer
{
public:
	CircularBuffer() :
			mReadIndex(0),
			mWriteIndex(0) {
	}

	bool pop(void* data, uint16_t size);
	bool push(void* data, uint16_t size);

	bool empty() const {
		return mReadIndex == mWriteIndex;
	}

	bool full() const {
		return (mWriteIndex + 1) % SIZE == mReadIndex;
	}

	void reset() {
		mReadIndex = 0;
		mWriteIndex = 0;
	}

protected:
	void readData(uint16_t readIndex, uint8_t* data, uint16_t size);
	void writeData(uint16_t writeIndex, uint8_t* data, uint16_t size);

	bool hasFreeSpace(uint16_t size) const {
		if (mWriteIndex >= mReadIndex) {
			return (SIZE - (mWriteIndex - mReadIndex) > size);
		} else {
			return (mReadIndex - mWriteIndex > size);
		}
	}

private:
	uint8_t mBuffer[SIZE];
	uint16_t mReadIndex;
	uint16_t mWriteIndex;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(CircularBuffer)
};

template<uint16_t SIZE>
bool CircularBuffer<SIZE>::pop(void* data, uint16_t size) {
	if (empty()) {
		return false;
	}

	uint16_t dataSize;
	readData(mReadIndex, (uint8_t*) &dataSize, 2);
	if (size >= dataSize) {
		readData(mReadIndex + 2, (uint8_t*) data, dataSize);
		mReadIndex = (mReadIndex + size + 2) % SIZE;
		return true;
	} else {
		return false;
	}
}

template<uint16_t SIZE>
bool CircularBuffer<SIZE>::push(void* data, uint16_t size) {
	if ((size + 2) >= SIZE) {
		return false;
	}
	if (!hasFreeSpace(size + 2)) {
		return false;
	}

	writeData(mWriteIndex, (uint8_t*) &size, 2);
	writeData(mWriteIndex + 2, (uint8_t*) data, size);
	mWriteIndex = (mWriteIndex + size + 2) % SIZE;
	return true;
}

template<uint16_t SIZE>
void CircularBuffer<SIZE>::readData(uint16_t readIndex, uint8_t* data, uint16_t size) {
	if (readIndex + size < SIZE) {
		memcpy(data, mBuffer + readIndex, size);
	} else {
		uint16_t partialSize = (SIZE - readIndex);
		memcpy(data, mBuffer + readIndex, partialSize);
		memcpy(data + partialSize, mBuffer, size - partialSize);
	}
}

template<uint16_t SIZE>
void CircularBuffer<SIZE>::writeData(uint16_t writeIndex, uint8_t* data, uint16_t size) {
	if (writeIndex + size < SIZE) {
		memcpy(mBuffer + writeIndex, data, size);
	} else {
		uint16_t partialSize = (SIZE - writeIndex);
		memcpy(mBuffer + writeIndex, data, partialSize);
		memcpy(mBuffer, data + partialSize, size - partialSize);
	}
}

} /* namespace mindroid */

#endif /* MINDROID_CIRCULARBUFFER_H_ */
