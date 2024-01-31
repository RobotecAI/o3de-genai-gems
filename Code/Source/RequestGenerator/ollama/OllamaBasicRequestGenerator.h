
#pragma once

#include <AICore/RequestGenerator/RequestGenerator.h>
#include <AzCore/std/string/string.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include "OllamaBasicPromptConfiguration.h"
#include <AzCore/RTTI/RTTI.h>

namespace AICore
{
    class OllamaBasicRequestGenerator : public RequestGenerator<OllamaBasicPromptConfiguration, Aws::Utils::Json::JsonValue, AZStd::string>
    {
    public:
        // AZ_RTTI(OllamaBasicRequestGenerator, "{5d67a8fb-5684-4e69-b382-9f130ae463b6}", RequestGenerator<OllamaBasicPromptConfiguration,
        // Aws::Utils::Json::JsonValue, AZStd::string>); AZ_CLASS_ALLOCATOR(OllamaBasicRequestGenerator, AZ::SystemAllocator);
        OllamaBasicRequestGenerator(OllamaBasicPromptConfiguration config);
        ~OllamaBasicRequestGenerator() = default;

        Aws::Utils::Json::JsonValue EncodePrompt(AZStd::string prompt) override;
        AZStd::string DecodeRequest(Aws::Utils::Json::JsonValue request) override;
    };
} // namespace AICore
