
#pragma once

#include <AICore/RequestGenerator/ollama/OllamaBasicRequestConfiguration.h>

#include <AzCore/std/any.h>
#include <AzCore/std/string/string.h>
#include <aws/core/utils/json/JsonSerializer.h>

namespace AICore
{
    class OllamaBasicRequestGenerator
    {
    public:
        OllamaBasicRequestGenerator(OllamaBasicRequestConfiguration config);
        ~OllamaBasicRequestGenerator() = default;

        Aws::Utils::Json::JsonView EncodePrompt(const AZStd::string& prompt);

        AZStd::string DecodeRequest(const Aws::Utils::Json::JsonView& request);

    private:
        OllamaBasicRequestConfiguration m_config;
    };
} // namespace AICore