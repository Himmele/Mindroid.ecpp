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

#ifndef MINDROID_RUNNABLE_H_
#define MINDROID_RUNNABLE_H_

#include <stdint.h>
#include <stddef.h>
#include "mindroid/util/concurrent/locks/Lock.h"

namespace mindroid {

class Runnable {
public:
    Runnable() :
            when(0xFFFFFFFFFFFFFFFF),
            nextRunnable(NULL) {
    }

    virtual ~Runnable() {
    }

    virtual void run() = 0;

    inline bool isInUse() {
        AutoLock autoLock;
        return when != 0xFFFFFFFFFFFFFFFF;
    }

private:
    inline void markInUse() {
    }

    inline void recycle() {
        AutoLock autoLock;
        when = 0xFFFFFFFFFFFFFFFF;
        nextRunnable = NULL;
    }

    uint64_t when; // milliseconds
    Runnable* nextRunnable;

    friend class RunnableQueue;
};

} /* namespace mindroid */

#endif /* MINDROID_RUNNABLE_H_ */
