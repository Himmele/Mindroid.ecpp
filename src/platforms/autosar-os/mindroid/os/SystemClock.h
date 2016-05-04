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

#ifndef MINDROID_SYSTEMCLOCK_H_
#define MINDROID_SYSTEMCLOCK_H_

#include <stdint.h>
#include "mindroid/lang/Object.h"

namespace mindroid {

class SystemClock {
public:
	static uint64_t monotonicTime(); // milliseconds

	static uint64_t realTime(); // milliseconds

private:
	SystemClock() {
	}

	NO_COPY_CONSTRUCTOR_AND_ASSIGNMENT_OPERATOR(SystemClock)
};

} /* namespace mindroid */

#endif /* MINDROID_SYSTEMCLOCK_H_ */
