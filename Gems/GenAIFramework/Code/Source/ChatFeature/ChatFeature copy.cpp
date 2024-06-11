/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ChatFeature.h"
#include "AzCore/std/containers/vector.h"
#include "GenAIFramework/Communication/AIModelAgentBus.h"
#include "GenAIFramework/Communication/AIModelRequestBus.h"
#include "GenAIFramework/Feature/ConversationBus.h"

#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <iostream>

namespace GenAIFramework
{
    ChatFeature::ChatFeature(AZ::u64 agentId, AZ::u64 conversationId)
        : FeatureBase(agentId, conversationId)
    {
    }

    ChatFeature::~ChatFeature()
    {
    }

    void ChatFeature::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<ChatFeature, FeatureBase>()->Version(0);
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterFeature<ChatFeature>("Chat");
        }
    }

    void ChatFeature::OnNewMessage(const AZStd::string& message)
    {
        AIHistory history;
        AIModelAgentBus::EventResult(history, m_agentId, &AIModelAgentBus::Events::GetHistory);

        AIMessages messages = history;
        AIMessage newMessage = { Role::User, { AZStd::any(message) } };
        messages.push_back(newMessage);

        AIMessage systemMessage = {
            Role::System,
            { AZStd::any(AZStd::string(
                "Hello! I am your AI assistant for the Open 3D Engine. I am here to help you with any questions you have regarding the "
                "workings of the engine. Whether you need guidance on using specific features, troubleshooting issues, or understanding "
                "the best practices, feel free to ask. Let's create something amazing together with Open 3D Engine!")) }
        };
        messages.push_back(systemMessage);

        AIModelAgentBus::Event(m_agentId, &AIModelAgentBus::Events::SendPrompt, messages);
    }

    void ChatFeature::OnPromptResponse(ModelAPIExtractedResponse response)
    {
        if (!response.IsSuccess())
        {
            AZStd::string errorMessage = response.GetError().c_str();
            AZStd::vector<AZStd::string> summary = {};
            ConversationNotificationBus::Event(
                m_conversationId, &ConversationNotificationBus::Events::OnFeatureResponse, errorMessage, summary);
            return;
        }

        std::cout << "ChatFeature::OnPromptResponse" << std::endl;

        for (const auto& responseItem : response.GetValue().second)
        {
            if (responseItem.is<AZStd::string>())
            {
                AZStd::string responseString = AZStd::any_cast<AZStd::string>(responseItem);
                AZStd::vector<AZStd::string> summary = {};
                std::cout << ConversationNotificationBus::HasHandlers(m_conversationId) << std::endl;
                std::cout << "ID: " << m_conversationId << std::endl;
                ConversationNotificationBus::Event(
                    m_conversationId, &ConversationNotificationBus::Events::OnFeatureResponse, responseString, summary);
            }
            else
            {
                std::cout << "No string" << std::endl;
            }
        }
    }

} // namespace GenAIFramework
