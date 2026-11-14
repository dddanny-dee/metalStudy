#ifndef INSTANCE_H
#define INSTANCE_H

#include "lib/Foundation/Foundation.hpp"
#include "lib/Metal/Metal.hpp"
#include "lib/QuartzCore/QuartzCore.hpp"

class MetalInstance {
public:
    MetalInstance();
    ~MetalInstance();

    MTL::Device* device;
    MTL::CommandQueue* cmdq;
private:
    NS::AutoreleasePool* pool;
};

#endif