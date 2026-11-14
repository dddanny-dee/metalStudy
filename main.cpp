#include "instance.h"

#include <iostream>
#include <chrono>

const NS::UInteger imgWidth = 600;
const NS::UInteger imgHeight = 800;

int main() {

    MetalInstance instance;

    auto now = std::chrono::high_resolution_clock::now();
    auto nowSecs = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
    auto tenSecsFromNow = nowSecs + std::chrono::seconds(3);

    MTL::TextureDescriptor* textureDesc = MTL::TextureDescriptor::alloc()->init();
    textureDesc->setWidth(imgWidth);
    textureDesc->setHeight(imgHeight);
    textureDesc->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    textureDesc->setUsage(MTL::TextureUsageRenderTarget);
    textureDesc->setStorageMode(MTL::StorageModeShared);

    MTL::Texture* texture = instance.device->newTexture(textureDesc);
    textureDesc->release();

    MTL::RenderPassDescriptor* rpDescriptor = MTL::RenderPassDescriptor::alloc()->init();

    while (nowSecs <= tenSecsFromNow) {

        // These autorelease pools nest by the way!!!
        NS::AutoreleasePool* loopPool = NS::AutoreleasePool::alloc()->init();
    
        // Per frame
        MTL::CommandBuffer* commandBuffer = instance.cmdq->commandBuffer();
        // Per pass, this is like a scope
        MTL::RenderCommandEncoder* commandEncoder = commandBuffer->renderCommandEncoder(rpDescriptor);

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

    std::cout << "hello world" << std::endl;

    return 0;

}