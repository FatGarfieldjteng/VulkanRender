#include "Queue.h"
#include "Device.h"

Queue::Queue()
{

}

Queue::~Queue()
{
   
}

void Queue::acquireQueue(Device* device)
{
	device->acquireQueue(&mGraphicsQueue);
}