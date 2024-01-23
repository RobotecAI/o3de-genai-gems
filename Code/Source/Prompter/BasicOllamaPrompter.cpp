
#include "AzCore/std/string/string.h"
#include <AICore/Prompter/BasicOllamaPrompter.h>

namespace AICore
{
    BasicOllamaPrompter::BasicOllamaPrompter(AZStd::string url, OllamaBasicRequestConfiguration config)
        : m_requester(url)
        , m_generator(config)
    {
    }

    void BasicOllamaPrompter::Prompt(AZStd::string prompt, AZStd::function<void(AZStd::string, bool)> callback)
    {
        auto request = m_generator.EncodePrompt(prompt);
        m_requester.Request(
            request,
            [](const Aws::Utils::Json::JsonView& response, bool success)
            {
                // if (success)
                // {
                //     auto result = m_generator.DecodeRequest(response);
                //     callback(result, true);
                // }
                // else
                // {
                //     callback(AZStd::string(), false);
                // }
            });
    }
} // namespace AICore