/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ClaudeModelMessagesAPI.h"
#include "ClaudeModelConfiguration.h"
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>

#include <aws/core/utils/Array.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/memory/stl/AWSVector.h>

namespace GenAIVendorBundle
{
    ClaudeModelMessagesAPI::ClaudeModelMessagesAPI(const ClaudeModelConfiguration& config)
        : m_configuration(config)
    {
    }

    void ClaudeModelMessagesAPI::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ClaudeModelMessagesAPI, AZ::Component>()->Version(1)->Field(
                "defaultConfiguration", &ClaudeModelMessagesAPI::m_configuration);

            if (auto* editContext = serializeContext->GetEditContext())
            {
                editContext->Class<ClaudeModelMessagesAPI>("Claude Messages API Model", "")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AZ::Edit::Attributes::Category, "AI")
                    ->DataElement(
                        AZ::Edit::UIHandlers::Default,
                        &ClaudeModelMessagesAPI::m_configuration,
                        "Default Configuration",
                        "The default configuration to use when generating prompts")
                    ->Attribute(AZ::Edit::Attributes::Visibility, AZ::Edit::PropertyVisibility::ShowChildrenOnly);
            }
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterModelConfiguration<ClaudeModelMessagesAPI>();
        }
    }

    void ClaudeModelMessagesAPI::Activate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusConnect(GetEntityId());
    }

    void ClaudeModelMessagesAPI::Deactivate()
    {
        GenAIFramework::AIModelRequestBus::Handler::BusDisconnect();
    }

    GenAIFramework::ModelAPIRequest ClaudeModelMessagesAPI::PrepareRequest(const GenAIFramework::AIMessages& prompt)
    {
        Aws::Utils::Json::JsonValue messagesArrayJson;
        Aws::Utils::Array<Aws::Utils::Json::JsonValue> messagesArray(prompt.size());

        AZStd::string systemMessage = "";

        for (int i = 0; i < prompt.size(); i++)
        {
            Aws::Utils::Json::JsonValue message;
            switch (prompt[i].first)
            {
            case GenAIFramework::Role::System:
                for (int j = 0; j < prompt[i].second.size(); j++)
                {
                    if (prompt[i].second[j].is<AZStd::string>())
                    {
                        systemMessage += AZStd::any_cast<AZStd::string>(prompt[i].second[j]);
                    }
                }
                break;

            case GenAIFramework::Role::User:
                message.WithString("role", "user");
                break;

            case GenAIFramework::Role::Assistant:
                message.WithString("role", "assistant");
                break;
            default:
                AZ_Error("ModelAPIRequest", false, "Unsupported Role selected");
                break;
            }
            Aws::Utils::Array<Aws::Utils::Json::JsonValue> contentArray(prompt[i].second.size());
            for (int j = 0; j < prompt[i].second.size(); j++)
            {
                if (prompt[i].second[j].is<AZStd::string>())
                {
                    Aws::Utils::Json::JsonValue content;
                    content.WithString("type", "text");
                    content.WithString("text", AZStd::any_cast<AZStd::string>(prompt[i].second[j]).c_str());
                    contentArray[j] = content;
                }
            }

            message.WithArray("content", contentArray);
            messagesArray[i] = message;
        }
        messagesArrayJson.AsArray(messagesArray);

        Aws::Utils::Json::JsonValue jsonPrompt;
        jsonPrompt.WithObject("messages", messagesArrayJson);

        jsonPrompt.WithInteger("max_tokens", m_configuration.m_maxTokensToSample);
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
        // Override the system message if it is in the prompt
        if (!systemMessage.empty())
        {
            jsonPrompt.WithString("system", systemMessage.c_str());
        }
        else if (m_configuration.m_useSystemMessage)
        {
            jsonPrompt.WithString("system", m_configuration.m_systemMessage.c_str());
        }

        Aws::String jsonString = jsonPrompt.View().WriteReadable();
        AZ_Printf("ClaudeModelMessagesAPI", "Prepared request: %s", jsonString.c_str());
        return jsonString.c_str();
    }

    GenAIFramework::ModelAPIExtractedResponse ClaudeModelMessagesAPI::ExtractResult(
        const GenAIFramework::ModelAPIResponse& modelAPIResponse)
    {
        if (!modelAPIResponse.IsSuccess())
        {
            return AZ::Failure(AZStd::string::format("Failed to get a response from the model: %s", modelAPIResponse.GetError().c_str()));
        }

        Aws::Utils::Json::JsonValue jsonPrompt(modelAPIResponse.GetValue().c_str());

        if (jsonPrompt.WasParseSuccessful())
        {
            auto content = jsonPrompt.View().GetArray("content");
            GenAIFramework::AIContent completion;

            for (int i = 0; i < content.GetLength(); i++)
            {
                auto message = content.GetItem(i);
                auto role = message.GetString("type");
                if (role == "text")
                {
                    completion.push_back(AZStd::any(static_cast<AZStd::string>(message.GetString("text").c_str())));
                }
            }

            GenAIFramework::AIMessage response = { GenAIFramework::Role::Assistant, completion };

            return AZ::Success(response);
        }
        else
        {
            return AZ::Failure(AZStd::string::format("Failed to parse the response %s", modelAPIResponse.GetValue().c_str()));
        }
    };

    AZ::Outcome<void, AZStd::string> ClaudeModelMessagesAPI::SetModelParameter(
        const AZ::Name& parameterName, const AZStd::string& parameterValue)
    {
        return m_configuration.SetModelParameter(parameterName, parameterValue);
    }

} // namespace GenAIVendorBundle
