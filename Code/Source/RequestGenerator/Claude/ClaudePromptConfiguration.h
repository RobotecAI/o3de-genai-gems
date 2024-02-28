
#pragma once

#include "AzCore/RTTI/RTTIMacros.h"
#include "AzCore/std/string/string.h"
#include <AzCore/RTTI/ReflectContext.h>

namespace AICore
{
    class ClaudePromptInputConfiguration
    {
    public:
        AZ_RTTI(ClaudePromptInputConfiguration, "{5a478ab3-8ac6-4c23-8768-03c2791d6b11}");

        ClaudePromptInputConfiguration() = default;
        virtual ~ClaudePromptInputConfiguration() = default;

        static void Reflect(AZ::ReflectContext* context);

        AZStd::optional<int> m_maxTokensToSample;
        AZStd::optional<float> m_temperature;
        AZStd::optional<float> m_topP;
        AZStd::optional<int> m_topK;
        AZStd::optional<AZStd::string> m_stopSequence;
    };

    class ClaudePromptInput
    {
    public:
        AZ_RTTI(ClaudePromptInput, "{345d0942-abc0-4334-96cb-fb109dc98a7b}");

        ClaudePromptInput() = default;
        virtual ~ClaudePromptInput() = default;

        AZStd::string m_prompt;
        AZStd::optional<ClaudePromptInputConfiguration> m_configuration;
    };

    class ClaudePromptOutput
    {
    public:
        AZ_RTTI(ClaudePromptOutput, "{e8ee9752-a1c2-4a3f-bd51-189253b8edac}");

        ClaudePromptOutput() = default;
        virtual ~ClaudePromptOutput() = default;

        AZStd::string m_completion;
        AZStd::string m_stopReason;
        AZStd::optional<AZStd::string> stop;
    };
} // namespace AICore
