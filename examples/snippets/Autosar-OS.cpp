#include "mindroid/os/Process.h"
#include "mindroid/os/Looper.h"
#include "ServiceManager.h"
#include "autosar-os.h"
#include <new>

using namespace mindroid;

static uint8_t sProcessData[sizeof(Process)];
static uint8_t sServiceManagerData[sizeof(ServiceManager)];

TASK(mainTask) {
    Looper::prepare(mainTask, mainTaskAlarm, mainTaskEvent);

    Process* process = new (sProcessData) Process(*Looper::myLooper());
    ServiceManager* serviceManager = new (sServiceManagerData) ServiceManager();

    process->startService(*serviceManager);

    Looper::loop();

    process->stopService(*serviceManager);

    TerminateTask();
}

int main(void) {
    StartOS(OSDEFAULTAPPMODE);

    while (true) {
    }
}
