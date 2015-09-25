#include "mindroid/os/Looper.h"
#include "mindroid/os/LooperThread.h"
#include "mindroid/os/Handler.h"
#include "mindroid/os/Thread.h"
#include "mindroid/os/Message.h"
#include "mindroid/os/Closure.h"
#include "mindroid/os/Clock.h"
#include "mindroid/util/Buffer.h"
#include "mindroid/util/Log.h"
#include "mindroid/util/Assert.h"

using namespace mindroid;

static const char* LOG_TAG = "Mindroid";

class Handler1 : public Handler
{
public:
	virtual void handleMessage(const Message& message) {
		switch (message.what) {
		case 4:
		case 5: {
			Runnable* runnable = (Runnable*) message.obj;
			runnable->run();
			break;
		}
		default:
			Log::i(LOG_TAG, "Handler1::handleMessage 0x%x with ID %d by Looper 0x%x",
					&message, message.what, Looper::myLooper());
		}
	}
};

class Handler2 : public Handler
{
public:
	Handler2() {
		obtainMessage(mMessage, 3);
	}

	virtual void handleMessage(const Message& message) {
		Log::i(LOG_TAG, "Handler2::handleMessage 0x%x with ID %d by Looper 0x%x",
				&message, message.what, Looper::myLooper());
	}

	void test() {
		sendMessage(mMessage);
	}

private:
	Message mMessage;
};

class Handler3 : public Handler
{
public:
	Handler3(Looper& looper) : Handler(looper), mMessage(*this, 4) {}

	virtual void handleMessage(const Message& message) {
		Log::i(LOG_TAG, "Handler3::handleMessage 0x%x with ID %d by Looper 0x%x",
				&message, message.what, Looper::myLooper());
		sendMessageDelayed(mMessage, 4000);
		Log::i(LOG_TAG, "Time: %lld", Clock::monotonicTime());
	}

	void test() {
		bool rc = sendMessageDelayed(mMessage, 1000);
	}

private:
	Message mMessage;
};

class Runnable1 : public Runnable
{
public:
	virtual void run() {
		Log::i(LOG_TAG, "Runnable1::run by Looper 0x%x", Looper::myLooper());
		Assert::assertTrue(false);
	}
};

class Test
{
public:
	void test(int32_t value) {
		Log::i(LOG_TAG, "Closure1::test with value %d by Looper 0x%x",
				value, Looper::myLooper());
	}
};

LooperThread<Handler1> sLooperThread1;
LooperThread<Handler2> sLooperThread2;

int main() {
	Log::i(LOG_TAG, "Test");

	sLooperThread1.start();
	Handler* handler1 = sLooperThread1.getHandler();
	sLooperThread2.start();
	Handler2* handler2 = sLooperThread2.getHandler();

	Thread::sleep(200);

	Closure1<Test, int32_t> closure1;
	Message message1;
	handler1->obtainMessage(message1, 4, &closure1);
	Runnable1 runnable1;
	Message message2;
	handler1->obtainMessage(message2, 5, &runnable1);

	Handler3 handler3(*sLooperThread2.getLooper());
	handler3.test();

	Test test;
	int i = 0;
	while (i < 10) {
		obtainClosure(closure1, test, &Test::test, i++);
		handler1->sendMessageDelayed(message2, 100);
		handler1->removeMessage(message2);
		handler1->sendMessageDelayed(message1, 500);

		handler2->test();

		Thread::sleep(1000);
	}

	sLooperThread1.getLooper()->quit();
	sLooperThread1.getLooper()->quit();

	return 0;
}
