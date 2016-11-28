#include "ServiceManager.h"

using namespace mindroid;

const char* const ServiceManager::LOG_TAG = "ServiceManager";

ServiceManager::ServiceManager() :
        mHello(mWorld),
        mWorld(mHello) {
}

void ServiceManager::onCreate() {
    mHello.onCreate();
    mWorld.onCreate();
}

void ServiceManager::onDestroy() {
    mWorld.onDestroy();
    mHello.onDestroy();
}
