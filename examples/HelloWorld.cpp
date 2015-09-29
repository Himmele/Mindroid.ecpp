#include "HelloWorld.h"
#include "mindroid/util/Log.h"

using namespace mindroid;

const char* const Hello::LOG_TAG = "Hello";
const char* const World::LOG_TAG = "World";

Hello::Hello(World& world) :
		mWorld(world) {
}

void Hello::onCreate() {
	sayHello();
}

void Hello::onDestroy() {
}

void Hello::handleMessage(const Message& msg) {
	switch (msg.what) {
	case SAY_HELLO:
		Log::i(LOG_TAG, "Hello");
		mWorld.sayWorld();
		break;
	}
}

void Hello::sayHello() {
	obtainMessage(mMessage, SAY_HELLO);
	mMessage.sendToTarget();
}

void Hello::sayHello(uint32_t delay) {
	obtainMessage(mMessage, SAY_HELLO);
	sendMessageDelayed(mMessage, delay);
}

World::World(Hello& hello) :
		mHello(hello) {
}

void World::onCreate() {
}

void World::onDestroy() {
}

void World::sayWorld() {
	obtainClosure(mSayWorld, *this, &World::say, "World");
	postDelayed(mSayWorld, 100);
}

void World::say(const char* world) {
	Log::i(LOG_TAG, world);
	mHello.sayHello(1000);
}
