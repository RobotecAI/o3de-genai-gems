
#pragma once

#include "AzCore/Memory/Memory_fwd.h"
#include <AzCore/Memory/Memory.h>
#include <AzCore/Memory/SystemAllocator.h>
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
        using PromptConfiguration = PromptConfigurationType;

    protected:
        PromptConfiguration m_config;
    };
} // namespace AICore
