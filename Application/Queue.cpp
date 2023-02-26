#include "Queue.h"
#include "Device.h"

Queue::Queue(Type type)
	:mType(type)
{

}

Queue::~Queue()
{
   
}

void Queue::acquireQueue(Device* device)
{
	device->acquireQueue(mType, &mQueue);
}

Queue::Type Queue::getType()
{
	return mType;
}