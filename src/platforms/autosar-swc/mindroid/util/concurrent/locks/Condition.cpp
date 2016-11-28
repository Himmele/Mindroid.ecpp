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

#include "mindroid/util/concurrent/locks/Condition.h"
#include "mindroid/util/concurrent/locks/Lock.h"
#include "mindroid/util/Assert.h"

namespace mindroid {

Condition::Condition() {
}

void Condition::await() {
    bool runtimeException = true;
    Assert::assertTrue(!runtimeException);
}

void Condition::await(uint32_t timeout) {
    bool runtimeException = true;
    Assert::assertTrue(!runtimeException);
}

void Condition::signal() {
}

void Condition::signalAll() {
}

} /* namespace mindroid */
