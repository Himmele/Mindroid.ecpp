#include <stdio.h>
#include "mindroid/os/Message.h"
#include "mindroid/os/LooperThread.h"

using namespace mindroid;

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

int main() {
	Message message;

	Looper::prepare();
	Handler* worldHandler = new (sHelloHandler) WorldHandler();
	Handler* helloHandler = new (sWorldHandler) HelloHandler(*worldHandler);
	helloHandler->obtainMessage(message, SAY_HELLO);
	message.sendToTarget();
	Looper::loop();

	return 0;
}
