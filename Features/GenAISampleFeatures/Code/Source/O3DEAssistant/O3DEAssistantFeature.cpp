/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "O3DEAssistantFeature.h"
#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Feature/AIAgentBus.h>
#include <GenAIFramework/Feature/ConversationBus.h>
#include <GenAIFramework/Feature/CppFeatureBase.h>
#include <GenAIFramework/SystemRegistrationContext/SystemRegistrationContext.h>

#include <AzCore/std/containers/vector.h>

namespace GenAISampleFeatures
{
    O3DEAssistantFeature::O3DEAssistantFeature(AZ::u64 agentId, AZ::u64 conversationId)
        : CppFeatureBase(agentId, conversationId)
    {
    }

    void O3DEAssistantFeature::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<O3DEAssistantFeature, GenAIFramework::FeatureBase>()->Version(0);
        }

        if (auto registrationContext = azrtti_cast<GenAIFramework::SystemRegistrationContext*>(context))
        {
            registrationContext->RegisterFeature<O3DEAssistantFeature>("O3DE Assistant");
        }
    }

    void O3DEAssistantFeature::OnNewMessage(const AZStd::string& message)
    {
        // Init AI Messages with the history
        GenAIFramework::AIMessages messages;
        GenAIFramework::AIAgentRequestBus::EventResult(messages, m_agentId, &GenAIFramework::AIAgentRequestBus::Events::GetHistory);

        // System message is stored in the history; it should be added otherwise
        if (messages.empty())
        {
            GenAIFramework::AIMessage systemMessage = {
                GenAIFramework::Role::System,
                { AZStd::any(AZStd::string(
                    "You are an AI assistant for the Open 3D Engine. You are here to help you with any questions regarding the "
                    "workings of the engine. Guidance on using specific features, troubleshooting issues, or understanding "
                    "the best practices are some things that you will help with.")) }
            };
            messages.push_back(systemMessage);
        }

        GenAIFramework::AIMessage newMessage = { GenAIFramework::Role::User, { AZStd::any(message) } };
        messages.push_back(newMessage);

        GenAIFramework::AIAgentRequestBus::Event(m_agentId, &GenAIFramework::AIAgentRequestBus::Events::SendPrompt, messages);
    }

    void O3DEAssistantFeature::OnAIResponse(GenAIFramework::ModelAPIExtractedResponse response)
    {
        if (!response.IsSuccess())
        {
            AZStd::string errorMessage = response.GetError().c_str();
            AZStd::vector<AZStd::string> summary = {};
            GenAIFramework::ConversationNotificationBus::Event(
                m_conversationId, &GenAIFramework::ConversationNotificationBus::Events::OnFeatureResponse, errorMessage, summary);
            return;
        }

        for (const auto& responseItem : response.GetValue().second)
        {
            if (responseItem.is<AZStd::string>())
            {
                AZStd::string responseString = AZStd::any_cast<AZStd::string>(responseItem);
                AZStd::vector<AZStd::string> summary = {};
                GenAIFramework::ConversationNotificationBus::Event(
                    m_conversationId, &GenAIFramework::ConversationNotificationBus::Events::OnFeatureResponse, responseString, summary);
            }
        }
    }

} // namespace GenAISampleFeatures
