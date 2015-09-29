#include "mindroid/app/Process.h"
#include "mindroid/os/Looper.h"
#include "mindroid/os/Thread.h"
#include "mindroid/os/Handler.h"
#include "ServiceManager.h"
#include <new>

using namespace mindroid;

static uint8_t sProcessData[sizeof(Process)] __attribute__((aligned (8)));
static uint8_t sServiceManagerData[sizeof(ServiceManager)] __attribute__((aligned (8)));

int main() {
	Looper::prepare();
	Process* process = new (sProcessData) Process(*Looper::myLooper());
	ServiceManager* serviceManager = new (sServiceManagerData) ServiceManager();

	process->startService(*serviceManager);

	Looper::loop();

	process->stopService(*serviceManager);

	return 0;
}

