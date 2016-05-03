#ifndef HELLOWORLD_H_
#define HELLOWORLD_H_

#include <stdint.h>
#include "mindroid/app/Service.h"
#include "mindroid/lang/Closure.h"
#include "mindroid/lang/Object.h"
#include "mindroid/os/Handler.h"
#include "mindroid/os/Message.h"

class World;

class Hello : public mindroid::Service, public mindroid::Handler {
public:
	Hello(World& world);
	virtual void onCreate();
	virtual void onDestroy();
	virtual void handleMessage(const mindroid::Message& message);
	void sayHello();
	void sayHello(uint32_t delay);

private:
	static const char* const LOG_TAG;
	static const uint32_t SAY_HELLO = 0;

	World& mWorld;
	mindroid::Message mMessage;

	NO_COPY_CONSTRUCTOR_AND_ASSIGNMENT_OPERATOR(Hello)
};

class World : public mindroid::Service, public mindroid::Handler {
public:
	World(Hello& hello);
	virtual void onCreate();
	virtual void onDestroy();
	void sayWorld();
	void say(const char* world);

private:
	static const char* const LOG_TAG;

	Hello& mHello;
	mindroid::Closure1<World, const char*> mSayWorld;

	NO_COPY_CONSTRUCTOR_AND_ASSIGNMENT_OPERATOR(World)
};

#endif /* HELLOWORLD_H_ */

