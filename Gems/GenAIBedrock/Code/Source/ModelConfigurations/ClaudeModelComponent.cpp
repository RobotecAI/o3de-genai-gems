/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ClaudeModelComponent.h"
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>
#include <ModelConfigurations/ClaudeModelConfiguration.h>

#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <sstream>

namespace GenAIBedrock
{
    ClaudeModelComponent::ClaudeModelComponent(const ClaudeModelConfiguration& config)
        : m_configuration(config)
    {
    }

    void ClaudeModelComponent::Reflect(AZ::ReflectContext* context)
    {
        ClaudeModelConfiguration::Reflect(context);

        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudeModelComponent, AZ::Component>()->Version(1)->Field(
                "Configuration", &ClaudeModelComponent::m_configuration);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudeModelComponent>("Claude Model", "Generates prompts and extracts results for the Claude model")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelComponent::m_configuration,
                        "Configuration",
                        "The configuration attached to the prompts")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterModelConfiguration<ClaudeModelComponent>();
        }
    }

    void ClaudeModelComponent::Activate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusConnect(GetEntityId());
    }

    void ClaudeModelComponent::Deactivate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusDisconnect();
    }

    GenAIFramework::ModelAPIRequest ClaudeModelComponent::PrepareRequest(const AZStd::string& prompt)
    {
        std::stringstream oss;
        oss << "\n\nHuman: " << prompt.c_str() << "\n\nAssistant: ";
        Aws::Utils::Json::JsonValue jsonPrompt;
        jsonPrompt.WithString("prompt", oss.str().c_str());
        jsonPrompt.WithInteger("max_tokens_to_sample", m_configuration.m_maxTokensToSample);

        if (!m_configuration.m_useDefaultTemperature)
        {
            jsonPrompt.WithDouble("temperature", m_configuration.m_temperature);
        }
        if (!m_configuration.m_useDefaultTopP)
        {
            jsonPrompt.WithDouble("top_p", m_configuration.m_topP);
        }
        if (!m_configuration.m_useDefaultTopK)
        {
            jsonPrompt.WithInteger("top_k", m_configuration.m_topK);
        }
        if (!m_configuration.m_useDefaultStopSequence)
        {
            jsonPrompt.WithString("stop_sequence", m_configuration.m_stopSequence.c_str());
        }

        Aws::String jsonString = jsonPrompt.View().WriteReadable();
        AZ_Printf("ClaudeModelComponent", "Prepared request: %s", jsonString.c_str());
        return jsonString.c_str();
    }

    AZ::Outcome<AZStd::string, AZStd::string> ClaudeModelComponent::ExtractResult(const GenAIFramework::ModelAPIResponse& modelAPIResponse)
    {
        if (modelAPIResponse.IsSuccess())
        {
            AZ_Printf("ClaudeModelComponent", "Extracted result: %s", modelAPIResponse.GetValue().c_str());
            Aws::Utils::Json::JsonValue jsonPrompt(modelAPIResponse.GetValue().c_str());
            if (jsonPrompt.WasParseSuccessful())
            {
                auto view = jsonPrompt.View();
                return AZ::Success(view.GetString("completion").c_str());
            }
            else
            {
                return AZ::Failure(AZStd::string::format("Failed to parse the response %s", modelAPIResponse.GetValue().c_str()));
            }
        }
        else
        {
            return AZ::Failure(modelAPIResponse.GetError());
        }
    };
} // namespace GenAIBedrock
