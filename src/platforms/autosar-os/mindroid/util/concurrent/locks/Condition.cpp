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

#include "mindroid/util/concurrent/locks/Condition.h"
#include "mindroid/util/concurrent/locks/Lock.h"
#include "mindroid/util/Assert.h"

namespace mindroid {

Condition::Condition() :
        mTaskId(NULL),
        mAlarmId(NULL),
        mEventId(NULL) {
}

Condition::Condition(TaskType taskId, AlarmType alarmId, EventMaskType eventId) :
        mTaskId(taskId),
        mAlarmId(alarmId),
        mEventId(eventId) {
}

void Condition::await() {
    Lock::unlock();
    Schedule();
    WaitEvent(mEventId);
    EventMaskType event;
    GetEvent(mTaskId, &event);
    ClearEvent(event);
    Lock::lock();
}

void Condition::await(uint32_t timeout) {
    Lock::unlock();
    Assert::assertTrue(timeout > 0);
    SetRelAlarm(mAlarmId, timeout, 0);
    Schedule();
    WaitEvent(mEventId);
    CancelAlarm(mAlarmId);
    EventMaskType event;
    GetEvent(mTaskId, &event);
    ClearEvent(event);
    Lock::lock();
}

void Condition::signal() {
    SetEvent(mTaskId, mEventId);
}

void Condition::signalAll() {
    bool notYetImplemented = true;
    Assert::assertTrue(!notYetImplemented);
}

} /* namespace mindroid */
