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

#include "mindroid/os/Lock.h"
#include "mindroid/util/Assert.h"
#include <cmsis_os.h>

namespace mindroid {

uint32_t Lock::sCounter = 0;

bool Lock::lock() {
	if (sCounter == 0) {
		__disable_irq();
	}
	sCounter++;
	return true;
}

void Lock::unlock() {
	Assert::assertTrue(sCounter != 0);

	sCounter--;
	if (sCounter == 0) {
		__enable_irq();
	}
}

} /* namespace mindroid */
