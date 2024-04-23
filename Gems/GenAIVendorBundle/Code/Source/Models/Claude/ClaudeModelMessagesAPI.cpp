/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ClaudeModelMessagesAPI.h"
#include "ClaudeModelConfiguration.h"

#include <AzCore/Component/Component.h>
#include <AzCore/RTTI/RTTIMacros.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/std/containers/vector.h>
#include <AzCore/std/string/string.h>
#include <AzCore/std/utility/pair.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>
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

    GenAIFramework::ModelAPIRequest ClaudeModelMessagesAPI::PrepareRequest(const GenAIFramework::ModelAPIPrompt& prompt)
    {
        Aws::Utils::Json::JsonValue messagesArrayJson;
        Aws::Utils::Array<Aws::Utils::Json::JsonValue> messagesArray(prompt.size());

        for (int i = 0; i < prompt.size(); i++)
        {
            Aws::Utils::Json::JsonValue message;
            message.WithString("role", "user");
            message.WithString("content", AZStd::any_cast<AZStd::string>(prompt[i]).c_str());
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
        if (m_configuration.m_useSystemMessage)
        {
            jsonPrompt.WithString("system", m_configuration.m_systemMessage.c_str());
        }

        Aws::String jsonString = jsonPrompt.View().WriteReadable();
        AZ_Printf("ClaudeModelMessagesAPI", "Prepared request: %s", jsonString.c_str());
        return jsonString.c_str();
    }

    GenAIFramework::ModelAPIResponse ClaudeModelMessagesAPI::ExtractResult(const GenAIFramework::ModelAPIRequest& modelAPIRequest)
    {
        Aws::Utils::Json::JsonValue jsonPrompt(modelAPIRequest.c_str());

        if (jsonPrompt.WasParseSuccessful())
        {
            auto content = jsonPrompt.View().GetArray("content");
            AZStd::vector<AZStd::any> completion;

            for (int i = 0; i < content.GetLength(); i++)
            {
                auto message = content.GetItem(i);
                auto role = message.GetString("type");
                if (role == "text")
                {
                    completion.push_back(AZStd::any(static_cast<AZStd::string>(message.GetString("text").c_str())));
                }
            }

            return AZ::Success(completion);
        }
        else
        {
            return AZ::Failure(AZStd::string::format("Failed to parse the response %s", modelAPIRequest.c_str()));
        }
    };

} // namespace GenAIVendorBundle
