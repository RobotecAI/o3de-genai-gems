/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <GenAIFramework/Communication/AIModelRequestBus.h>
#include <GenAIFramework/Feature/JsonUtils.h>
#include <GenAIFramework/GenAIFrameworkTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/RTTI/RTTI.h>

namespace GenAIFramework
{
    class AIAgentRequests : public AZ::EBusTraits
    {
    public:
        AZ_RTTI(AIAgentRequests, AIAgentRequestsTypeId);
        virtual ~AIAgentRequests() = default;

        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::u64;

        //! Send a prompt to the model agent.
        //! @param prompt The prompt to send.
        //! The response will be sent to the OnAIResponse notification.
        virtual void SendPrompt(const AIMessages& prompt) = 0;

        //! Send a prompt to the model agent as a JSON string.
        //! @param prompt The prompt to send as a JSON string.
        //! The prompt is a JSON string that is an array of objects. Each object consists of a role and content.
        //! The role is a string that can be "user", "assistant", or "system". Corrsponding to the user, assistant, and system roles.
        //! The content is an array of strings that are the content of the message.
        virtual void SendPromptAsJsonString(const AZStd::string& prompt)
        {
            AIMessages messages = JsonUtils::JsonStringToAIMessages(prompt);
            SendPrompt(messages);
        }

        //! Get the history of the model agent.
        //! @return The history of the model agent.
        virtual AIHistory GetHistory() const = 0;

        //! Get the history of the model agent as a JSON string.
        //! @return The history of the model agent as a JSON string.
        //! The history is a JSON string that is an array of objects. Each object consists of a role and content.
        //! The role is a string that can be "user", "assistant", or "system". Corrsponding to the user, assistant, and system roles.
        //! The content is an array of strings that are the content of the message.
        virtual AZStd::string GetHistoryAsJsonString() const
        {
            AIHistory history = GetHistory();
            return JsonUtils::AIMessagesToJsonString(history);
        }
    };

    class AIAgentNotifications : public AZ::EBusTraits
    {
    public:
        AZ_RTTI(AIAgentNotifications, AIAgentNotificationsTypeId);
        virtual ~AIAgentNotifications() = default;

        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::ById;
        using BusIdType = AZ::u64;

        //! Notification that the model agent has a response to the prompt.
        //! @param response The response from the model agent.
        virtual void OnAIResponse(ModelAPIExtractedResponse response)
        {
            AZ_UNUSED(response);
        };
    };

    using AIAgentRequestBus = AZ::EBus<AIAgentRequests>;
    using AIAgentNotificationBus = AZ::EBus<AIAgentNotifications>;
} // namespace GenAIFramework
