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
#include "mindroid/os/Lock.h"

namespace mindroid {

class Runnable
{
public:
	Runnable() : mExecTimestamp(0), mNextRunnable(NULL) {}
	virtual ~Runnable() {}
	virtual void run() = 0;

	inline bool isInUse() {
		AutoLock autoLock(mLock);
		return mExecTimestamp != 0;
	}

	static const int32_t MSG_RUNNABLE = 0xFFFFFFFF;

private:
	inline void recycle() {
		AutoLock autoLock(mLock);
		mExecTimestamp = 0;
		mNextRunnable = NULL;
	}

	uint64_t mExecTimestamp; // nanoseconds
	Runnable* mNextRunnable;
	mutable Lock mLock;

	friend class RunnableQueue;
};

} /* namespace mindroid */

#endif /* MINDROID_RUNNABLE_H_ */
