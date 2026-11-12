#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "lib/Foundation/Foundation.hpp"
#include "lib/Metal/Metal.hpp"
#include "lib/QuartzCore/QuartzCore.hpp"

#include <iostream>
#include <chrono>

    // babies first fma
    const char* shaderSrc = R"(
    #include <metal_stdlib>
    using namespace metal;

    kernel void doubleValues(device float* data [[buffer(0)]],
                            uint id [[thread_position_in_grid]]) {
        data[id] = data[id] * 2.0;
    }
    )";

int main() {

    // https://github.com/ocornut/imgui/blob/master/backends/imgui_impl_metal.mm

    auto now = std::chrono::high_resolution_clock::now();
    auto nowSecs = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
    auto tenSecsFromNow = nowSecs + std::chrono::seconds(3);

    // Because metal-cpp is a thin wrapper around objective-c you inherit all of it's memory quirks. 
    // Mainly its reference count based release model, though I like to think of it as a C++ destructor.
    // XCode inserts this on entry point, it is somewhat hidden there but needed here in C++ land.
    NS::AutoreleasePool* mainPool = NS::AutoreleasePool::alloc()->init();

    // This is important because, for example, the next line of code initializes a Metal device.
    // It looks like it gives a Metal device, but it also allocate a bunch bundles, dictionaries, description sets.
    // If these objects don't have a pool, they will never be released- A leak.

    // Device init system (ev&doe you really only have one gpu on all mac devices released >2020 LAWL)
    MTL::Device* device = MTL::CreateSystemDefaultDevice();
    // A per application command queue.
    MTL::CommandQueue* commandQueue = device->newCommandQueue(); 

    const NS::UInteger imgWidth = 600;
    const NS::UInteger imgHeight = 800;

    MTL::TextureDescriptor* textureDesc = MTL::TextureDescriptor::alloc()->init();

    textureDesc->setWidth(imgWidth);
    textureDesc->setHeight(imgHeight);
    textureDesc->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    textureDesc->setUsage(MTL::TextureUsageRenderTarget);
    textureDesc->setStorageMode(MTL::StorageModeShared);

    MTL::Texture* texture = device->newTexture(textureDesc);

    MTL::RenderPassDescriptor* descriptor = MTL::RenderPassDescriptor::alloc()->init();

    while (nowSecs <= tenSecsFromNow) {

        // These autorelease pools nest by the way!!!
        NS::AutoreleasePool* loopPool = NS::AutoreleasePool::alloc()->init();
    
        // Per frame
        MTL::CommandBuffer* commandBuffer = commandQueue->commandBuffer();
        // Per pass, this is like a scope
        MTL::RenderCommandEncoder* commandEncoder = commandBuffer->renderCommandEncoder();

        // hereeeee

        commandEncoder->endEncoding();
        commandBuffer->commit();
        commandBuffer->waitUntilCompleted();

        // Insanely important to have a pool because we are creating a buffer and encoder obj-c object.
        // each iteration, each frame, each pass.
        loopPool->release();
        
        now = std::chrono::high_resolution_clock::now();
        nowSecs = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
    }  

    mainPool->release();

    std::cout << "hello world" << std::endl;

    return 0;

}