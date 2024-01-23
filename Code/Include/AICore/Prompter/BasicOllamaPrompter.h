
#pragma once

#include <AICore/Communication/JSONHttp/BasicJSONRequester.h>
#include <AICore/RequestGenerator/ollama/OllamaBasicRequestConfiguration.h>
#include <AICore/RequestGenerator/ollama/OllamaBasicRequestGenerator.h>

#include <AzCore/std/functional.h>
#include <AzCore/std/functional_basic.h>

namespace AICore
{
    class BasicOllamaPrompter
    {
    public:
        BasicOllamaPrompter(AZStd::string url, OllamaBasicRequestConfiguration config);
        ~BasicOllamaPrompter() = default;

        void Prompt(AZStd::string prompt, AZStd::function<void(AZStd::string, bool)> callback);

    private:
        BasicJSONRequester m_requester;
        OllamaBasicRequestGenerator m_generator;
    };
} // namespace AICore