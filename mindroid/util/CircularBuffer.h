/*
 * Copyright (C) 2014 Daniel Himmelein
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
#include <sys/uio.h>
#include "mindroid/util/Utils.h"

namespace mindroid {

template<uint16_t SIZE>
class CircularBuffer {
public:
	CircularBuffer() :
			mReadIndex(0),
			mWriteIndex(0),
			mPeakSize(0) {
	}

	int32_t front(void* data, uint16_t size);
	int32_t frontv(const struct iovec* iov, uint16_t iovcnt);
	int32_t pop(void* data, uint16_t size);
	int32_t popv(const struct iovec* iov, uint16_t iovcnt);
	bool push(const void* data, uint16_t size);
	bool pushv(const struct iovec* iov, uint16_t iovcnt);
	uint16_t capacity() {
		return SIZE;
	}
	uint16_t peakSize() const {
		return mPeakSize;
	}

	bool empty() const {
		return mReadIndex == mWriteIndex;
	}

	bool full() const {
		return (mWriteIndex + 1) % SIZE == mReadIndex;
	}

	void clear() {
		mReadIndex = 0;
		mWriteIndex = 0;
		mPeakSize = 0;
	}

protected:
	void readData(uint16_t readIndex, uint8_t* data, uint16_t size);
	void writeData(uint16_t writeIndex, const uint8_t* data, uint16_t size);

private:
	void calcPeakSize(uint16_t readIndex, uint16_t writeIndex);

	uint8_t mBuffer[SIZE];
	uint16_t mReadIndex;
	uint16_t mWriteIndex;
	uint16_t mPeakSize;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(CircularBuffer)
};

template<uint16_t SIZE>
int32_t CircularBuffer<SIZE>::front(void* data, uint16_t size) {
	if (data == NULL) {
		return 0;
	}

	const uint16_t readIndex = mReadIndex;
	const uint16_t writeIndex = mWriteIndex;

	if (readIndex == writeIndex) {
		return 0;
	}

	uint16_t dataSize;
	readData(readIndex, (uint8_t*) &dataSize, 2);
	if (size > dataSize) {
		size = dataSize;
	}
	readData((readIndex + 2) % SIZE, (uint8_t*) data, size);
	return size;
}

template<uint16_t SIZE>
int32_t CircularBuffer<SIZE>::frontv(const struct iovec* iov, uint16_t iovcnt) {
	if (iov == NULL) {
		return 0;
	}

	const uint16_t readIndex = mReadIndex;
	const uint16_t writeIndex = mWriteIndex;

	if (readIndex == writeIndex) {
		return 0;
	}

	uint16_t size = 0;
	for (uint16_t i = 0; i < iovcnt; i++) {
		if (iov[i].iov_base != NULL) {
			size += iov[i].iov_len;
		}
	}

	uint16_t dataSize;
	readData(readIndex, (uint8_t*) &dataSize, 2);
	if (size > dataSize) {
		size = dataSize;
	}
	uint16_t offset = 0;
	for (uint16_t i = 0; i < iovcnt; i++) {
		if (iov[i].iov_base != NULL && iov[i].iov_len != 0) {
			uint16_t count = ((size - offset) >= iov[i].iov_len) ? iov[i].iov_len : (size - offset);
			readData((readIndex + 2 + offset) % SIZE, (uint8_t*) iov[i].iov_base, count);
			offset += iov[i].iov_len;
			if (offset == size) {
				break;
			}
		}
	}
	return size;
}

template<uint16_t SIZE>
int32_t CircularBuffer<SIZE>::pop(void* data, uint16_t size) {
	const uint16_t readIndex = mReadIndex;
	const uint16_t writeIndex = mWriteIndex;

	if (readIndex == writeIndex) {
		return 0;
	}

	uint16_t dataSize;
	readData(readIndex, (uint8_t*) &dataSize, 2);
	if (data != NULL) {
		if (size >= dataSize) {
			uint16_t newReadIndex = (readIndex + dataSize + 2) % SIZE;
			readData((readIndex + 2) % SIZE, (uint8_t*) data, dataSize);
			mReadIndex = newReadIndex;
			return dataSize;
		} else {
			return -dataSize;
		}
	} else {
		uint16_t newReadIndex = (readIndex + dataSize + 2) % SIZE;
		mReadIndex = newReadIndex;
		return dataSize;
	}
}

template<uint16_t SIZE>
int32_t CircularBuffer<SIZE>::popv(const struct iovec* iov, uint16_t iovcnt) {
	const uint16_t readIndex = mReadIndex;
	const uint16_t writeIndex = mWriteIndex;

	if (readIndex == writeIndex) {
		return 0;
	}

	uint16_t size = 0;
	for (uint16_t i = 0; i < iovcnt; i++) {
		if (iov[i].iov_base != NULL) {
			size += iov[i].iov_len;
		}
	}

	uint16_t dataSize;
	readData(readIndex, (uint8_t*) &dataSize, 2);
	if (iov != NULL) {
		if (size >= dataSize) {
			uint16_t newReadIndex = (readIndex + dataSize + 2) % SIZE;
			uint16_t offset = 0;
			for (uint16_t i = 0; i < iovcnt; i++) {
				if (iov[i].iov_base != NULL && iov[i].iov_len != 0) {
					uint16_t count = ((dataSize - offset) >= iov[i].iov_len) ? iov[i].iov_len : (dataSize - offset);
					readData((readIndex + 2 + offset) % SIZE, (uint8_t*) iov[i].iov_base, count);
					offset += iov[i].iov_len;
					if (offset == dataSize) {
						break;
					}
				}
			}
			mReadIndex = newReadIndex;
			return dataSize;
		} else {
			return -dataSize;
		}
	} else {
		uint16_t newReadIndex = (readIndex + dataSize + 2) % SIZE;
		mReadIndex = newReadIndex;
		return dataSize;
	}
}

template<uint16_t SIZE>
bool CircularBuffer<SIZE>::push(const void* data, uint16_t size) {
	if (data == NULL) {
		return (size == 0);
	}
	if (size == 0) {
		return true;
	}
	if (SIZE <= (size + 2)) {
		return false;
	}

	const uint16_t readIndex = mReadIndex;
	const uint16_t writeIndex = mWriteIndex;

	bool hasFreeSpace;
	if (writeIndex >= readIndex) {
		hasFreeSpace = (SIZE - (writeIndex - readIndex)) > (size + 2);
	} else {
		hasFreeSpace = (readIndex - writeIndex) > (size + 2);
	}

	if (hasFreeSpace) {
		uint16_t newWriteIndex = (writeIndex + size + 2) % SIZE;
		writeData(writeIndex, (uint8_t*) &size, 2);
		writeData((writeIndex + 2) % SIZE, (uint8_t*) data, size);
		mWriteIndex = newWriteIndex;
		calcPeakSize(readIndex, newWriteIndex);
		return true;
	} else {
		return false;
	}
}

template<uint16_t SIZE>
bool CircularBuffer<SIZE>::pushv(const struct iovec* iov, uint16_t iovcnt) {
	if (iov == NULL) {
		return false;
	}
	uint16_t size = 0;
	for (uint16_t i = 0; i < iovcnt; i++) {
		if (iov[i].iov_base != NULL) {
			size += iov[i].iov_len;
		}
	}
	if (size == 0) {
		return true;
	}
	if (SIZE <= (size + 2)) {
		return false;
	}

	const uint16_t readIndex = mReadIndex;
	const uint16_t writeIndex = mWriteIndex;

	bool hasFreeSpace;
	if (writeIndex >= readIndex) {
		hasFreeSpace = (SIZE - (writeIndex - readIndex)) > (size + 2);
	} else {
		hasFreeSpace = (readIndex - writeIndex) > (size + 2);
	}

	if (hasFreeSpace) {
		uint16_t newWriteIndex = (writeIndex + size + 2) % SIZE;
		writeData(writeIndex, (uint8_t*) &size, 2);
		uint16_t offset = 0;
		for (uint16_t i = 0; i < iovcnt; i++) {
			if (iov[i].iov_base != NULL && iov[i].iov_len != 0) {
				writeData((writeIndex + 2 + offset) % SIZE, (uint8_t*) iov[i].iov_base, iov[i].iov_len);
				offset += iov[i].iov_len;
			}
		}
		mWriteIndex = newWriteIndex;
		calcPeakSize(readIndex, newWriteIndex);
		return true;
	} else {
		return false;
	}
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
void CircularBuffer<SIZE>::writeData(uint16_t writeIndex, const uint8_t* data, uint16_t size) {
	if (writeIndex + size < SIZE) {
		memcpy(mBuffer + writeIndex, data, size);
	} else {
		uint16_t partialSize = (SIZE - writeIndex);
		memcpy(mBuffer + writeIndex, data, partialSize);
		memcpy(mBuffer, data + partialSize, size - partialSize);
	}
}

template<uint16_t SIZE>
void CircularBuffer<SIZE>::calcPeakSize(uint16_t readIndex, uint16_t writeIndex) {
	uint16_t peakSize;
	if (writeIndex >= readIndex) {
		peakSize = (writeIndex - readIndex);
	} else {
		peakSize = (SIZE - (readIndex - writeIndex));
	}
	if (peakSize > mPeakSize) {
		mPeakSize = peakSize;
	}
}

} /* namespace mindroid */

#endif /* MINDROID_CIRCULARBUFFER_H_ */
