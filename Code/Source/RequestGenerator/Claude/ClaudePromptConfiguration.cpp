
#include "ClaudePromptConfiguration.h"
<<<<<<< HEAD
#include "AzCore/Component/ComponentBus.h"
#include "AzCore/Serialization/EditContextConstants.inl"
=======
>>>>>>> 4657333 (Requester and generator)
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace AICore
{
    void ClaudePromptInputConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
<<<<<<< HEAD
            serializeContext->Class<ClaudePromptInputConfiguration, AZ::ComponentConfig>()
                ->Version(0)
=======
            serializeContext->Class<ClaudePromptInputConfiguration>()
>>>>>>> 4657333 (Requester and generator)
                ->Field("maxTokensToSample", &ClaudePromptInputConfiguration::m_maxTokensToSample)
                ->Field("temperature", &ClaudePromptInputConfiguration::m_temperature)
                ->Field("topP", &ClaudePromptInputConfiguration::m_topP)
                ->Field("topK", &ClaudePromptInputConfiguration::m_topK)
<<<<<<< HEAD
                ->Field("stopSequence", &ClaudePromptInputConfiguration::m_stopSequence)
                ->Field("useDefaultTemperature", &ClaudePromptInputConfiguration::m_useDefaultTemperature)
                ->Field("useDefaultTopP", &ClaudePromptInputConfiguration::m_useDefaultTopP)
                ->Field("useDefaultTopK", &ClaudePromptInputConfiguration::m_useDefaultTopK)
                ->Field("useDefaultStopSequence", &ClaudePromptInputConfiguration::m_useDefaultStopSequence);
=======
                ->Field("stopSequence", &ClaudePromptInputConfiguration::m_stopSequence);
>>>>>>> 4657333 (Requester and generator)

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
<<<<<<< HEAD
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &ClaudePromptInputConfiguration::m_useDefaultTemperature)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptInputConfiguration::m_useDefaultTemperature,
                        "Use Default Temperature",
                        "Use the default temperature")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
=======
>>>>>>> 4657333 (Requester and generator)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptInputConfiguration::m_topP,
                        "Top P",
                        "Use nucleus sampling. In nucleus sampling, Anthropic Claude computes the cumulative distribution over all the "
                        "options for each subsequent token in decreasing probability order and cuts it off once it reaches a particular "
                        "probability specified by top_p. You should alter either temperature or top_p, but not both.")
<<<<<<< HEAD
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &ClaudePromptInputConfiguration::m_useDefaultTopP)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptInputConfiguration::m_useDefaultTopP,
                        "Use Default Top P",
                        "Use the default top P")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
=======
>>>>>>> 4657333 (Requester and generator)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptInputConfiguration::m_topK,
                        "Top K",
                        "Only sample from the top K options for each subsequent token")
<<<<<<< HEAD
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &ClaudePromptInputConfiguration::m_useDefaultTopK)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptInputConfiguration::m_useDefaultTopK,
                        "Use Default Top K",
                        "Use the default top K")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
=======
>>>>>>> 4657333 (Requester and generator)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptInputConfiguration::m_stopSequence,
                        "Stop Sequence",
<<<<<<< HEAD
                        "Sequences that will cause the model to stop generating.")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &ClaudePromptInputConfiguration::m_useDefaultStopSequence)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudePromptInputConfiguration::m_useDefaultStopSequence,
                        "Use Default Stop Sequence",
                        "Use the default stop sequence")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues);
=======
                        "Sequences that will cause the model to stop generating.");
>>>>>>> 4657333 (Requester and generator)
            }
        }
    }
} // namespace AICore