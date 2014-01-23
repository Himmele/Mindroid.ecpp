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

#ifndef MINDROID_PROCESS_H_
#define MINDROID_PROCESS_H_

#include <stdint.h>
#include "mindroid/app/Service.h"
#include "mindroid/os/Looper.h"
#include "mindroid/os/Handler.h"
#include "mindroid/os/Lock.h"
#include "mindroid/util/Utils.h"
#include "mindroid/Mindroid.h"

namespace mindroid {

class Process
{
public:
	Process(Looper& looper);
	void startService(Service& service);
	void stopService(Service& service);
	Looper& getMainLooper() { return mLooper; }

private:
	class MainHandler : public Handler
	{
	public:
		static const int16_t START_SERVICE = 1;
		static const int16_t STOP_SERVICE = 2;

		MainHandler(Process& process) : Handler(process.getMainLooper()), mProcess(process) {}
		virtual void handleMessage(const Message& message);

	private:
		Process& mProcess;

		NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(MainHandler)
	};

	struct ServiceRecord
	{
		Service* service;
		bool running;
	};

	Looper& mLooper;
	MainHandler mMainHandler;
	ServiceRecord mServices[MAX_NUM_SERVICES];
	mindroid::Message mMessages[MAX_NUM_SERVICES];
	Lock mLock;

    NO_COPY_CTOR_AND_ASSIGNMENT_OPERATOR(Process)
};

} /* namespace mindroid */

#endif /* MINDROID_PROCESS_H_ */
