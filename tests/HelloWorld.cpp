#include <stdio.h>
#include "mindroid/os/Message.h"
#include "mindroid/os/LooperThread.h"
#include "mindroid/os/Closure.h"
#include "mindroid/util/Log.h"
#include "mindroid/util/Assert.h"

using namespace mindroid;

static const char* LOG_TAG = "HelloWorld";

static const int SAY_HELLO = 0;
static const int SAY_WORLD = 1;

class HelloHandler : public Handler {
public:
	HelloHandler(Handler& worldHandler) :
			mWorldHandler(worldHandler) {
	}

	virtual void handleMessage(const Message& msg) {
		switch (msg.what) {
		case SAY_HELLO:
			printf("Hello ");
			mWorldHandler.obtainMessage(mMessage, SAY_WORLD);
			mMessage.obj = this;
			mMessage.sendToTarget();
			break;
		}
	}

private:
	Handler& mWorldHandler;
	Message mMessage;
};

class WorldHandler : public Handler {
public:
	WorldHandler() {
	}

	virtual void handleMessage(const Message& msg) {
		switch (msg.what) {
		case SAY_WORLD:
			printf("World!\n");
			Handler* helloHandler = (Handler*) msg.obj;
			helloHandler->obtainMessage(mMessage, SAY_HELLO);
			helloHandler->sendMessageDelayed(mMessage, 1000);
			break;
		}
	}

private:
	Message mMessage;
};

static uint8_t sHelloHandler[sizeof(HelloHandler)];
static uint8_t sWorldHandler[sizeof(WorldHandler)];

class TestRunnable1 : public Runnable
{
public:
	virtual void run() {
		Log::i(LOG_TAG, "TestRunnable1::run by Looper 0x%x", Looper::myLooper());
		Assert::assertTrue(false);
	}
};

class TestRunnable2 : public Runnable
{
public:
	virtual void run() {
		Log::i(LOG_TAG, "TestRunnable2::run by Looper 0x%x", Looper::myLooper());
	}
};

class Test
{
public:
	void test(int32_t value) {
		Log::i(LOG_TAG, "Test::test with value %d by Looper 0x%x",
				value, Looper::myLooper());
	}
};

static Closure1<Test, int32_t> sClosure;

int main() {
	Message message;
	TestRunnable1 testRunnable1;
	TestRunnable2 testRunnable2;
	Test test;
	obtainClosure(sClosure, test, &Test::test, 42);

	Looper::prepare();
	Handler* worldHandler = new (sHelloHandler) WorldHandler();
	Handler* helloHandler = new (sWorldHandler) HelloHandler(*worldHandler);
	helloHandler->obtainMessage(message, SAY_HELLO);
	helloHandler->postDelayed(sClosure, 4000);
	helloHandler->postDelayed(testRunnable1, 1000);
	helloHandler->removeCallbacks(testRunnable1);
	helloHandler->post(testRunnable2);
	message.sendToTarget();
	Looper::loop();

	return 0;
}
