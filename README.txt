==== Message lifecycle ====

The mExecTimestamp field of class message is 0 as long as a message is not enqueued in a MessageQueue.
While mExecTimestamp is not equal to 0 you cannot re-obtain the message or re-send it to the message queue.
Before a message is handled by a Looper a copy of the message is created and the original message is recycled
(its mExecTimestamp field is set to 0) so that it can be reused and no events are lost during processing the message.


==== Message object usage ====

if (Handler::obtainMessage(message, 1)) {
	message.arg1 = 123;
	message.sendToTarget();
} 

...

message.sendToTarget();
