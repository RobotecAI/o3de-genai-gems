/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ClaudeModelTextCompletions.h"
#include "ClaudeModelConfiguration.h"
#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/any.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <sstream>

namespace GenAIVendorBundle
{
    ClaudeModelTextCompletions::ClaudeModelTextCompletions(const ClaudeModelConfiguration& config)
        : m_configuration(config)
    {
    }

    void ClaudeModelTextCompletions::Reflect(AZ::ReflectContext* context)
    {
        ClaudeModelConfiguration::Reflect(context);

        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudeModelTextCompletions, AZ::Component>()->Version(1)->Field(
                "Default Configuration", &ClaudeModelTextCompletions::m_configuration);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudeModelTextCompletions>("Claude Text Completions Model", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelTextCompletions::m_configuration,
                        "Default Configuration",
                        "The default configuration to use when generating prompts")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly)
                    ->UIElement(AZ::Edit::UIHandlers::Label, "Reset to default configuration");
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterModelConfiguration<ClaudeModelTextCompletions>();
        }
    }

    void ClaudeModelTextCompletions::Activate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusConnect(GetEntityId());
    }

    void ClaudeModelTextCompletions::Deactivate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusDisconnect();
    }

    GenAIFramework::ModelAPIRequest ClaudeModelTextCompletions::PrepareRequest(const GenAIFramework::AIMessages& prompt)
    {
        std::stringstream oss;

        for (const auto& element : prompt)
        {
            if (element.first == GenAIFramework::Role::User || element.first == GenAIFramework::Role::System)
            {
                oss << "\n\nHuman: ";
            }
            else if (element.first == GenAIFramework::Role::Assistant)
            {
                oss << "\n\nAssistant: ";
            }
            for (const auto& promptPart : element.second)
            {
                if (promptPart.is<AZStd::string>())
                {
                    oss << AZStd::any_cast<AZStd::string>(promptPart).c_str();
                }
            }
        }

        oss << "\n\nAssistant: ";

        Aws::Utils::Json::JsonValue jsonPrompt;
        jsonPrompt.WithString("prompt", oss.str().c_str());

        jsonPrompt.WithInteger("max_tokens_to_sample", m_configuration.m_maxTokensToSample);

        jsonPrompt.WithString("anthropic_version", m_configuration.m_anthropicVersion.c_str());

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
        AZ_Printf("ClaudeModelTextCompletions", "Prepared request: %s", jsonString.c_str());
        return jsonString.c_str();
    }

    GenAIFramework::ModelAPIExtractedResponse ClaudeModelTextCompletions::ExtractResult(
        const GenAIFramework::ModelAPIResponse& modelAPIResponse)
    {
        if (!modelAPIResponse.IsSuccess())
        {
            return AZ::Failure(AZStd::string::format("Failed to get a response from the model: %s", modelAPIResponse.GetError().c_str()));
        }
        Aws::Utils::Json::JsonValue jsonPrompt(modelAPIResponse.GetValue().c_str());
        if (jsonPrompt.WasParseSuccessful())
        {
            auto view = jsonPrompt.View();
            auto completion = AZStd::any(AZStd::string(view.GetString("completion").c_str()));
            GenAIFramework::AIContent completionsVector;
            completionsVector.push_back(completion);
            GenAIFramework::AIMessage response = { GenAIFramework::Role::Assistant, completionsVector };
            return AZ::Success(response);
        }
        else
        {
            return AZ::Failure(AZStd::string::format("Failed to parse the response %s", modelAPIResponse.GetValue().c_str()));
        }
    };

    AZ::Outcome<void, AZStd::string> ClaudeModelTextCompletions::SetModelParameter(
        const AZ::Name& parameterName, const AZStd::string& parameterValue)
    {
        return m_configuration.SetModelParameter(parameterName, parameterValue);
    }
} // namespace GenAIVendorBundle
