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

#ifndef MINDROID_THREAD_H_
#define MINDROID_THREAD_H_

#include <cmsis_os.h>
#include "mindroid/lang/Object.h"
#include "mindroid/lang/Runnable.h"

namespace mindroid {

class Thread : public Runnable {
public:
	Thread();
	Thread(Runnable* runnable);
	virtual ~Thread() {
	}
	virtual void run() {
	}
	bool start();
	static void sleep(uint32_t milliseconds);
	void interrupt();
	bool isInterrupted() const;
	void join() const;

private:
	static void exec(const void* args);

	Runnable* mRunnable;
	osThreadId mThreadId;
	// osThreadDef macro in cmsis_os.h
	osThreadDef_t mThread;
	bool mInterrupted;

	NO_COPY_CONSTRUCTOR_AND_ASSIGNMENT_OPERATOR(Thread)
};

} /* namespace mindroid */

#endif /* MINDROID_THREAD_H_ */
