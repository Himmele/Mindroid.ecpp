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

#ifndef MINDROID_CONTEXT_H_
#define MINDROID_CONTEXT_H_

#include <stdint.h>

namespace mindroid {

class Handler;

class Context {
public:
	Handler* bindService(const char* name);
	void unbindService(const char* name);
};

} /* namespace mindroid */

#endif /* MINDROID_CONTEXT_H_ */
