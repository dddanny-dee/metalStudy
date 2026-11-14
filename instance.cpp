#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "instance.h"

MetalInstance::MetalInstance() {
    // Because metal-cpp is a thin wrapper around objective-c you inherit all of it's memory quirks. 
    // Mainly its reference count based release model, though I like to think of it as a C++ destructor.
    // XCode inserts this on entry point, it is somewhat hidden there but needed here in C++ land.
    pool = NS::AutoreleasePool::alloc()->init();

    // This is important because, for example, the next line of code initializes a Metal device.
    // It looks like it gives a Metal device, but it also allocate a bunch bundles, dictionaries, description sets.
    // If these objects don't have a pool, they will never be released- A leak.

    // Device init system (ev&doe you really only have one gpu on all mac devices released >2020 LAWL)
    device = MTL::CreateSystemDefaultDevice();
    // A per application command queue.
    cmdq = device->newCommandQueue();
}

MetalInstance::~MetalInstance()
{
    pool->release();
}