#ifndef AUTOSAROS_H_
#define AUTOSAROS_H_

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AUTOSAR_OS
#define AUTOSAR_OS
#endif

typedef unsigned char StatusType;

typedef unsigned short CounterType;
typedef unsigned int TickType;
typedef TickType *TickRefType;

typedef unsigned short TaskType;
typedef TaskType *TaskRefType;

typedef unsigned short AlarmType;

typedef unsigned int EventMaskType;
typedef EventMaskType *EventMaskRefType;

extern void SuspendAllInterrupts(void);
extern void ResumeAllInterrupts(void);
extern void ResumeOSInterrupts(void);
extern void SuspendOSInterrupts(void);

extern StatusType GetCounterValue(CounterType counter, TickRefType tickRef);

extern StatusType GetTaskID(TaskRefType taskIdRef);
extern StatusType TerminateTask(void);

extern StatusType GetAlarm(AlarmType almId, TickRefType tickRef);
extern StatusType SetRelAlarm(AlarmType almId, TickType increment, TickType cycle);
extern StatusType SetAbsAlarm(AlarmType almId, TickType start, TickType cycle);
extern StatusType CancelAlarm(AlarmType almId);

extern StatusType SetEvent(TaskType taskId, EventMaskType mask);
extern StatusType ClearEvent(EventMaskType mask);
extern StatusType WaitEvent(EventMaskType mask);
extern StatusType GetEvent(TaskType taskId, EventMaskRefType maskRef);

extern StatusType Schedule(void);

CounterType counter_1_ms;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AUTOSAROS_H_ */
