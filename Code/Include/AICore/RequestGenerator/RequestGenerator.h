
#pragma once

#include <AzCore/Memory/Memory.h>
#include <AzCore/Memory/Memory_fwd.h>
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/RTTI/TemplateInfo.h>
#include <AzCore/RTTI/TypeInfo.h>
#include <AzCore/std/string/string.h>

namespace AICore
{
    template<typename PromptConfigurationType, typename RequestType, typename PromptType>
    class RequestGenerator
    {
    public:
        RequestGenerator(PromptConfigurationType config)
            : m_config(config){};
        virtual ~RequestGenerator() = default;

        virtual RequestType EncodePrompt(PromptType prompt) = 0;
        virtual PromptType DecodeRequest(RequestType request) = 0;

        using Request = RequestType;
        using Prompt = PromptType;

    protected:
        PromptConfiguration m_config;
    };

} // namespace AICore

// namespace AZ
// {
//     AZ_TYPE_INFO_TEMPLATE(
//         AICore::RequestGenerator,
//         "{0bcac4e2-09a9-4702-98dd-ef37006664a7}",
//         AZ_TYPE_INFO_TYPENAME,
//         AZ_TYPE_INFO_TYPENAME,
//         AZ_TYPE_INFO_TYPENAME);
// }