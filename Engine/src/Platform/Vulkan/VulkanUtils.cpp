#include "anepch.h"
#include "VulkanUtils.h"

#include <fstream>

#include "VulkanCommon.h"

namespace VulkanUtils
{
    void TransitionImage(const VkCommandBuffer cmd, const VkImage image, const VkImageLayout currentLayout, const VkImageLayout newLayout)
    {
        VkImageMemoryBarrier2 imageBarrier = { .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2 };
        imageBarrier.pNext = nullptr;

        imageBarrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
        imageBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
        imageBarrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
        imageBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_READ_BIT | VK_ACCESS_2_MEMORY_WRITE_BIT;

        imageBarrier.oldLayout = currentLayout;
        imageBarrier.newLayout = newLayout;

        VkImageAspectFlags aspectMask = (newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
        VkImageSubresourceRange subImage = { };
        subImage.aspectMask = aspectMask;
        subImage.baseMipLevel = 0;
        subImage.levelCount = VK_REMAINING_MIP_LEVELS;
        subImage.baseArrayLayer = 0;
        subImage.layerCount = VK_REMAINING_ARRAY_LAYERS;

        imageBarrier.subresourceRange = subImage;
        imageBarrier.image = image;

        VkDependencyInfo depInfo = { };
        depInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
        depInfo.pNext = nullptr;

        depInfo.imageMemoryBarrierCount = 1;
        depInfo.pImageMemoryBarriers = &imageBarrier;

        vkCmdPipelineBarrier2(cmd, &depInfo);
    }

    void CopyImageToImage(const VkCommandBuffer cmd, const VkImage source, const VkImage destination, const VkExtent2D srcSize, const VkExtent2D dstSize)
    {
        VkImageBlit2 blitRegion = { .sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2, .pNext = nullptr };

        blitRegion.srcOffsets[1].x = srcSize.width;
        blitRegion.srcOffsets[1].y = srcSize.height;
        blitRegion.srcOffsets[1].z = 1;

        blitRegion.dstOffsets[1].x = dstSize.width;
        blitRegion.dstOffsets[1].y = dstSize.height;
        blitRegion.dstOffsets[1].z = 1;

        blitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blitRegion.srcSubresource.baseArrayLayer = 0;
        blitRegion.srcSubresource.layerCount = 1;
        blitRegion.srcSubresource.mipLevel = 0;

        blitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blitRegion.dstSubresource.baseArrayLayer = 0;
        blitRegion.dstSubresource.layerCount = 1;
        blitRegion.dstSubresource.mipLevel = 0;

        VkBlitImageInfo2 blitInfo = { .sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2, .pNext = nullptr };
        blitInfo.dstImage = destination;
        blitInfo.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        blitInfo.srcImage = source;
        blitInfo.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        blitInfo.filter = VK_FILTER_LINEAR;
        blitInfo.regionCount = 1;
        blitInfo.pRegions = &blitRegion;

        vkCmdBlitImage2(cmd, &blitInfo);
    }

    bool LoadShaderModule(const char* filePath, const VkDevice device, const VkAllocationCallbacks* allocator, VkShaderModule* outShaderModule)
    {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) return false;

        // File size in bytes.
        const size_t fileSize = file.tellg();

        std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

        file.seekg(0);

        file.read((char*)buffer.data(), fileSize);

        file.close();

        VkShaderModuleCreateInfo createInfo = { .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, .pNext = nullptr };
        createInfo.codeSize = buffer.size() * sizeof(uint32_t);
        createInfo.pCode = buffer.data();

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, allocator, &shaderModule) != VK_SUCCESS)
        {
            return false;
        }

        *outShaderModule = shaderModule;
        return true;
    }

    bool LoadShaderModule(const Slang::ComPtr<slang::IBlob>& program, const VkDevice device, const VkAllocationCallbacks* allocator, VkShaderModule* outShaderModule)
    {
        VkShaderModuleCreateInfo createInfo = { .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO, .pNext = nullptr };
        createInfo.codeSize = program->getBufferSize();
        createInfo.pCode = static_cast<const uint32_t*>(program->getBufferPointer());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, allocator, &shaderModule) != VK_SUCCESS)
        {
            return false;
        }

        *outShaderModule = shaderModule;
        return true;
    }

    void LoadSlangShaders(const char* moduleName, const VkDevice device, const VkAllocationCallbacks* allocator, VkShaderModule* vertShader, VkShaderModule* fragShader)
    {
        using namespace Slang;

        ComPtr<slang::IGlobalSession> slangGlobalSession;
        createGlobalSession(slangGlobalSession.writeRef());

        slang::TargetDesc targetDesc = { };
        targetDesc.format = SLANG_SPIRV;
        targetDesc.profile = slangGlobalSession->findProfile("glsl_440");
        targetDesc.flags = SLANG_TARGET_FLAG_GENERATE_SPIRV_DIRECTLY;

        slang::SessionDesc sessionDesc = { };
        sessionDesc.targets = &targetDesc;
        sessionDesc.targetCount = 1;
        sessionDesc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR;
        const char* paths[] = { ASSET_PATH_SHADERS };
        sessionDesc.searchPaths = paths;
        sessionDesc.searchPathCount = 1;

        ComPtr<slang::ISession> session;
        slangGlobalSession->createSession(sessionDesc, session.writeRef());

        ComPtr<slang::IBlob> spirvVertProgram;
        ComPtr<slang::IBlob> spirvFragProgram;
        ComPtr<slang::IBlob> diagnosticBlob;
        slang::IModule* slangModule = session->loadModule(moduleName, diagnosticBlob.writeRef());

        if (!slangModule)
        {
            ANE_ELOG_ERROR("Error when loading shader module: {}", moduleName);
        }

        ComPtr<slang::IEntryPoint> vertEntry;
        slangModule->findEntryPointByName("vertexMain", vertEntry.writeRef());

        slang::IComponentType* components[] = { slangModule, vertEntry };
        ComPtr<slang::IComponentType> program;
        {
            ComPtr<slang::IBlob> diagnosticsBlob;
            SlangResult result = session->createCompositeComponentType(
                components,
                2,
                program.writeRef(),
                diagnosticsBlob.writeRef());
        }

        {
            ComPtr<slang::IBlob> diagnosticsBlob;
            SlangResult result = program->getEntryPointCode(
                0, 0, spirvVertProgram.writeRef(), diagnosticsBlob.writeRef());
        }

        ComPtr<slang::IEntryPoint> fragEntry;
        slangModule->findEntryPointByName("fragmentMain", fragEntry.writeRef());

        slang::IComponentType* components2[] = { slangModule, fragEntry };
        ComPtr<slang::IComponentType> program2;
        {
            ComPtr<slang::IBlob> diagnosticsBlob;
            SlangResult result = session->createCompositeComponentType(
                components2,
                2,
                program2.writeRef(),
                diagnosticsBlob.writeRef());
        }

        {
            ComPtr<slang::IBlob> diagnosticsBlob;
            SlangResult result = program2->getEntryPointCode(
                0, 0, spirvFragProgram.writeRef(), diagnosticsBlob.writeRef());
        }

        if (!LoadShaderModule(spirvVertProgram, device, allocator, vertShader))
        {
            ANE_ELOG_ERROR("Error when building vertex shader module: {}", slangModule->getName());
        }

        if (!LoadShaderModule(spirvFragProgram, device, allocator, fragShader))
        {
            ANE_ELOG_ERROR("Error when building fragment shader module: {}", slangModule->getName());
        }
    }
}
