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

#include "mindroid/util/Log.h"

extern "C" int vsnprintf(char* msg, size_t size, const char* format, va_list args);

namespace mindroid {

Logger Log::DEFAULT_LOGGER;
Logger* Log::sLogger = &DEFAULT_LOGGER;

int Log::v(const char* tag, const char* format, ...) {
    char msg[LOG_RECORD_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, LOG_RECORD_SIZE, format, args);
    va_end(args);

    return println(LOG_ID_MAIN, VERBOSE, tag, msg);
}

int Log::d(const char* tag, const char* format, ...) {
    char msg[LOG_RECORD_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, LOG_RECORD_SIZE, format, args);
    va_end(args);

    return println(LOG_ID_MAIN, DEBUG, tag, msg);
}

int Log::i(const char* tag, const char* format, ...) {
    char msg[LOG_RECORD_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, LOG_RECORD_SIZE, format, args);
    va_end(args);

    return println(LOG_ID_MAIN, INFO, tag, msg);
}

int Log::w(const char* tag, const char* format, ...) {
    char msg[LOG_RECORD_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, LOG_RECORD_SIZE, format, args);
    va_end(args);

    return println(LOG_ID_MAIN, WARN, tag, msg);
}

int Log::e(const char* tag, const char* format, ...) {
    char msg[LOG_RECORD_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, LOG_RECORD_SIZE, format, args);
    va_end(args);

    return println(LOG_ID_MAIN, ERROR, tag, msg);
}

int Log::wtf(const char* tag, const char* format, ...) {
    char msg[LOG_RECORD_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, LOG_RECORD_SIZE, format, args);
    va_end(args);

    return println(LOG_ID_MAIN, WTF, tag, msg);
}

int Log::println(int logId, int priority, const char* tag, const char* msg) {
    return sLogger->println(logId, priority, tag, msg);
}

void Log::setLogger(Logger& logger) {
    sLogger = &logger;
}

} /* namespace mindroid */
