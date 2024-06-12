/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "ChatFeature.h"

#include <AzCore/std/containers/vector.h>
#include <GenAIFramework/Communication/AIModelAgentBus.h>
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

namespace GenAIFramework
{
    ChatFeature::ChatFeature(AZ::u64 agentId, AZ::u64 conversationId)
        : FeatureBase(agentId, conversationId)
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
            { AZStd::any(
                AZStd::string("You are an AI assistant for the Open 3D Engine. You are here to help you with any questions regarding the "
                              "workings of the engine. Guidance on using specific features, troubleshooting issues, or understanding "
                              "the best practices are some things that you will help with.")) }
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

        for (const auto& responseItem : response.GetValue().second)
        {
            if (responseItem.is<AZStd::string>())
            {
                AZStd::string responseString = AZStd::any_cast<AZStd::string>(responseItem);
                AZStd::vector<AZStd::string> summary = {};
                ConversationNotificationBus::Event(
                    m_conversationId, &ConversationNotificationBus::Events::OnFeatureResponse, responseString, summary);
            }
        }
    }

} // namespace GenAIFramework
