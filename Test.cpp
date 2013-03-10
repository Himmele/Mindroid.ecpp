#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "mindroid/os/Looper.h"
#include "mindroid/os/Handler.h"
#include "mindroid/os/Thread.h"
#include "mindroid/os/Message.h"
#include "mindroid/os/Closure.h"
#include "mindroid/os/Clock.h"

using namespace mindroid;

class Handler3;
class Handler4;
Looper* sLooper1 = NULL;
Looper* sLooper2 = NULL;
Handler* sHandler1 = NULL;
Handler* sHandler2 = NULL;
Handler3* sHandler3 = NULL;
Handler4* sHandler4 = NULL;

class Handler2 : public Handler
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
			printf("Handler2::handleMessage 0x%x with ID %d by Looper 0x%x\n",
					&message, message.what, Looper::myLooper());
		}
	}
};

class Handler3 : public Handler
{
public:
	Handler3() {
		obtainMessage(mMessage, 3);
	}

	virtual void handleMessage(const Message& message) {
		printf("Handler3::handleMessage 0x%x with ID %d by Looper 0x%x\n",
				&message, message.what, Looper::myLooper());
	}

	void test() {
		sendMessage(mMessage);
	}

private:
	Message mMessage;
};

class Handler4 : public Handler
{
public:
	Handler4(Looper& looper) : Handler(looper), mMessage(*this, 4) {}

	virtual void handleMessage(const Message& message) {
		printf("Handler4::handleMessage 0x%x with ID %d by Looper 0x%x\n",
				&message, message.what, Looper::myLooper());
		sendMessageDelayed(mMessage, 4000);
		printf("Time: %lld\n", Clock::monotonicTime());
	}

	void test() {
		bool rc = sendMessageDelayed(mMessage, 1000);
	}

private:
	Message mMessage;
};

class Thread1 : public Thread
{
public:
	virtual void run() {
		Looper::prepare();
		sLooper1 = Looper::myLooper();
		sHandler3 = new Handler3();
		printf("Looper 1 uses thread %d\n", (int32_t) pthread_self());
		Looper::loop();
	}
};

class Thread2 : public Thread
{
public:
	virtual void run() {
		Looper::prepare();
		sLooper2 = Looper::myLooper();
		sHandler2 = new Handler2();
		printf("Looper 2 uses thread %d\n", (int32_t) pthread_self());
		Looper::loop();
	}
};

class Runnable1 : public Runnable
{
public:
	virtual void run() {
		printf("Runnable1::run by Looper 0x%x\n", Looper::myLooper());
		assert(false);
	}
};

class Test
{
public:
	void test(int32_t value) {
		printf("Closure1::test with value %d by Looper 0x%x\n",
				value, Looper::myLooper());
	}
};

Thread1 sThread1;
Thread2 sThread2;
Test sTest;

int main() {
	sThread1.start();
	sThread2.start();

	Thread::sleep(200);

	sHandler4 = new Handler4(*sLooper2);
	Closure1<Test, int32_t> closure1;
	Message message1;
	sHandler2->obtainMessage(message1, 4, &closure1);
	Runnable1 runnable1;
	Message message2;
	sHandler2->obtainMessage(message2, 5, &runnable1);

	sHandler4->test();
	int i = 0;
	while (true) {
		obtainClosure(closure1, sTest, &Test::test, i++);
		sHandler2->sendMessageDelayed(message2, 100);
		sHandler2->removeMessage(message2);
		sHandler2->sendMessageDelayed(message1, 500);

		sHandler3->test();

		Thread::sleep(1000);
	}

	sLooper1->quit();
	sThread1.join();
	sLooper2->quit();
	sThread2.join();

	return 0;
}
