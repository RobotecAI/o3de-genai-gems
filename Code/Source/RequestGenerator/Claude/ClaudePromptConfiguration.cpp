
#include "ClaudePromptConfiguration.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace AICore
{
    void ClaudePromptInputConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudePromptInputConfiguration>()
                ->Field("maxTokensToSample", &ClaudePromptInputConfiguration::m_maxTokensToSample)
                ->Field("temperature", &ClaudePromptInputConfiguration::m_temperature)
                ->Field("topP", &ClaudePromptInputConfiguration::m_topP)
                ->Field("topK", &ClaudePromptInputConfiguration::m_topK)
                ->Field("stopSequence", &ClaudePromptInputConfiguration::m_stopSequence);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudePromptInputConfiguration>("Claude Prompt Configuration", "Configuration for the Claude prompt")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AICore")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptInputConfiguration::m_maxTokensToSample,
                        "Max Tokens To Sample",
                        "The maximum number of tokens to generate before stopping")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptInputConfiguration::m_temperature,
                        "Temperature",
                        "The amount of randomness injected into the response.")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptInputConfiguration::m_topP,
                        "Top P",
                        "Use nucleus sampling. In nucleus sampling, Anthropic Claude computes the cumulative distribution over all the "
                        "options for each subsequent token in decreasing probability order and cuts it off once it reaches a particular "
                        "probability specified by top_p. You should alter either temperature or top_p, but not both.")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptInputConfiguration::m_topK,
                        "Top K",
                        "Only sample from the top K options for each subsequent token")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptInputConfiguration::m_stopSequence,
                        "Stop Sequence",
                        "Sequences that will cause the model to stop generating.");
            }
        }
    }
} // namespace AICore