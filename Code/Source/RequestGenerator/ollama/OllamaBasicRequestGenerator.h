
#pragma once

#include <AICore/RequestGenerator/RequestGenerator.h>
#include <AzCore/std/string/string.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include "OllamaBasicPromptConfiguration.h"

namespace AICore
{
    class OllamaBasicRequestGenerator : public RequestGenerator<OllamaBasicPromptConfiguration, Aws::Utils::Json::JsonValue, AZStd::string>
    {
    public:
        OllamaBasicRequestGenerator(OllamaBasicPromptConfiguration config);
        ~OllamaBasicRequestGenerator() = default;

        Aws::Utils::Json::JsonValue EncodePrompt(AZStd::string prompt) override;
        AZStd::string DecodeRequest(Aws::Utils::Json::JsonValue request) override;
    };
} // namespace AICore
