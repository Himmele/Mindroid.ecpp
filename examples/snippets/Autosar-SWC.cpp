#include "mindroid/os/Process.h"
#include "mindroid/os/Looper.h"
#include "mindroid/os/Clock.h"
#include "ServiceManager.h"
#include "autosar-os.h"

static mindroid::Looper sLooper;
static mindroid::Process sProcess(sLooper);
static ServiceManager sServiceManager;
static bool sRunning = false;

void start(void) {
	if (sRunning) {
		return;
	}

	sProcess.startService(sServiceManager);
	sRunning = true;

	// Ensure ServiceManager::onCreate
	sLooper.loop(42);
}

void shutdown(void) {
	if (!sRunning) {
		return;
	}

	sProcess.stopService(sServiceManager);
	sRunning = false;

	// Ensure ServiceManager::onDestroy
	sLooper.loop(42);
}

void cyclic_100ms(void) {
	if (!sRunning) {
		return;
	}

	mindroid::Clock::tick(100);
	sLooper.loop(42);
}
