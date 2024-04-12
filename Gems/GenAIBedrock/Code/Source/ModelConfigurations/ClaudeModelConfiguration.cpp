/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ClaudeModelConfiguration.h"

#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

namespace GenAIBedrock
{
    void ClaudeModelConfiguration::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudeModelConfiguration, AZ::ComponentConfig>()
                ->Version(0)
                ->Field("maxTokensToSample", &ClaudeModelConfiguration::m_maxTokensToSample)
                ->Field("temperature", &ClaudeModelConfiguration::m_temperature)
                ->Field("topP", &ClaudeModelConfiguration::m_topP)
                ->Field("topK", &ClaudeModelConfiguration::m_topK)
                ->Field("stopSequence", &ClaudeModelConfiguration::m_stopSequence)
                ->Field("useDefaultTemperature", &ClaudeModelConfiguration::m_useDefaultTemperature)
                ->Field("useDefaultTopP", &ClaudeModelConfiguration::m_useDefaultTopP)
                ->Field("useDefaultTopK", &ClaudeModelConfiguration::m_useDefaultTopK)
                ->Field("useDefaultStopSequence", &ClaudeModelConfiguration::m_useDefaultStopSequence);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudeModelConfiguration>("Claude Model Configuration", "Configuration for the Claude model")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelConfiguration::m_maxTokensToSample,
                        "Max Tokens To Sample",
                        "The maximum number of tokens to generate before stopping")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelConfiguration::m_temperature,
                        "Temperature",
                        "The amount of randomness injected into the response.")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &ClaudeModelConfiguration::m_useDefaultTemperature)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelConfiguration::m_useDefaultTemperature,
                        "Use Default Temperature",
                        "Use the default temperature")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelConfiguration::m_topP,
                        "Top P",
                        "Use nucleus sampling. In nucleus sampling, Anthropic Claude computes the cumulative distribution over all the "
                        "options for each subsequent token in decreasing probability order and cuts it off once it reaches a particular "
                        "probability specified by top_p. You should alter either temperature or top_p, but not both.")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &ClaudeModelConfiguration::m_useDefaultTopP)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelConfiguration::m_useDefaultTopP,
                        "Use Default Top P",
                        "Use the default top P")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelConfiguration::m_topK,
                        "Top K",
                        "Only sample from the top K options for each subsequent token")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &ClaudeModelConfiguration::m_useDefaultTopK)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelConfiguration::m_useDefaultTopK,
                        "Use Default Top K",
                        "Use the default top K")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelConfiguration::m_stopSequence,
                        "Stop Sequence",
                        "Sequences that will cause the model to stop generating.")
                    ->Attribute(AZ::Edit::Attributes::ReadOnly, &ClaudeModelConfiguration::m_useDefaultStopSequence)
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelConfiguration::m_useDefaultStopSequence,
                        "Use Default Stop Sequence",
                        "Use the default stop sequence")
                    ->Attribute(AZ::Edit::Attributes::ChangeNotify, AZ::Edit::PropertyRefreshLevels::AttributesAndValues);
            }
        }
    }
} // namespace GenAIBedrock
