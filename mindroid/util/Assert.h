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

#ifndef MINDROID_ASSERT_H_
#define MINDROID_ASSERT_H_

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "mindroid/util/Utils.h"

namespace mindroid {

class Assert
{
public:
	static void assertTrue(bool condition) {
		assert(condition);
	}
	static void assertTrue(const char* message, bool condition);

	static void assertFalse(bool condition) {
		assert(!condition);
	}
	static void assertFalse(const char* message, bool condition);

	static void assertNull(void* ptr) {
		assert(ptr == NULL);
	}
	static void assertNull(const char* message, void* ptr);

	static void assertNotNull(void* ptr) {
		assert(ptr != NULL);
	}
	static void assertNotNull(const char* message, void* ptr);

	static void assertEquals(bool expected, bool actual) {
		assert(expected == actual);
	}
	static void assertEquals(const char* message, bool expected, bool actual);

	static void assertEquals(int8_t expected, int8_t actual) {
		assert(expected == actual);
	}
	static void assertEquals(const char* message, int8_t expected, int8_t actual);

	static void assertEquals(uint8_t expected, uint8_t actual) {
		assert(expected == actual);
	}
	static void assertEquals(const char* message, uint8_t expected, uint8_t actual);

	static void assertEquals(int16_t expected, int16_t actual) {
		assert(expected == actual);
	}
	static void assertEquals(const char* message, int16_t expected, int16_t actual);

	static void assertEquals(uint16_t expected, uint16_t actual) {
		assert(expected == actual);
	}
	static void assertEquals(const char* message, uint16_t expected, uint16_t actual);

	static void assertEquals(int32_t expected, int32_t actual) {
		assert(expected == actual);
	}
	static void assertEquals(const char* message, int32_t expected, int32_t actual);

	static void assertEquals(uint32_t expected, uint32_t actual) {
		assert(expected == actual);
	}
	static void assertEquals(const char* message, uint32_t expected, uint32_t actual);

	static void assertEquals(int64_t expected, int64_t actual) {
		assert(expected == actual);
	}
	static void assertEquals(const char* message, int64_t expected, int64_t actual);

	static void assertEquals(uint64_t expected, uint64_t actual) {
		assert(expected == actual);
	}
	static void assertEquals(const char* message, uint64_t expected, uint64_t actual);

	static void assertEquals(void* expected, void* actual) {
		assert(expected == actual);
	}
	static void assertEquals(const char* message, void* expected, void* actual);

private:
	Assert() {}

	static const char* LOG_TAG;

	NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Assert)
};

} /* namespace mindroid */

#endif /* MINDROID_ASSERT_H_ */
