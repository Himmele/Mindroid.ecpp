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

#ifndef MINDROID_SERVICE_H_
#define MINDROID_SERVICE_H_

#include <stddef.h>
#include "mindroid/app/Context.h"

namespace mindroid {

class Service : public Context {
public:
	virtual ~Service() {}

	/**
	 * Called by the system when the service is first created. Do not call this method directly.
	 */
	virtual void onCreate() {
	}

	/**
	 * Called by the system when the service is started. Do not call this method directly.
	 */
	virtual void onStart(int startId) {
	}

	/**
	 * Called by the system to notify a Service that it is no longer used and is being removed.  The
	 * service should clean up any resources it holds (threads, registered
	 * receivers, etc) at this point.  Upon return, there will be no more calls
	 * in to this Service object and it is effectively dead. Do not call this method directly.
	 */
	virtual void onDestroy() {
	}

	/**
	 * Return the communication channel to the service.  May return null if
	 * clients can not bind to the service.
	 *
	 * @return Return a Handler through which clients can call on to the service.
	 */
	virtual Handler* onBind() {
		return NULL;
	}

	virtual void onUnbind() {
	}
};

} /* namespace mindroid */

#endif /* MINDROID_SERVICE_H_ */
